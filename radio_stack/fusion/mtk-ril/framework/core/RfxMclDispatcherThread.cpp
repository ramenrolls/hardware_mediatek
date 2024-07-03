/*
 * Copyright (C) 2021 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "RfxMclDispatcherThread.h"
#include "RfxLog.h"
#include "RfxHandlerManager.h"
#include "RfxMclStatusManager.h"
#include "RfxFragmentEncoder.h"
#include "utils/Mutex.h"
#include "RfxDispatchThread.h"
#include "RfxVoidData.h"
#include "RfxVersionManager.h"

#define RFX_LOG_TAG "RfxMclDisThread"

using ::android::Mutex;

RfxMclDispatcherThread* RfxMclDispatcherThread::s_self = NULL;

static sem_t sWaitLooperSem;
static bool sNeedWaitLooper = true;
static Mutex sWaitLooperMutex;

/*****************************************************************************
 * Class RfxMclBaseMessenger
 *****************************************************************************/
void RfxMclBaseMessenger::handleMessage(const Message& message) { onHandleMessage(message); }

/*****************************************************************************
 * Class RfxMclMessenger
 *****************************************************************************/
void RfxMclMessenger::onHandleMessage(const Message& message) {
    RFX_UNUSED(message);
    if (STATUS_SYNC == msg->getType()) {
        RfxMclStatusManager* statusMgr = RfxMclStatusManager::getMclStatusManager(msg->getSlotId());
        statusMgr->setValueByRfx(msg->getStatusKey(), msg->getStatusValue(), msg->getForceNotify(),
                                 msg->getIsDefault(), true, msg->getIsUpdateForMock());
    } else {
        int channelId;
        if (msg->getSendToMainProtocol()) {
            int mainSlotId = RfxMclStatusManager::getMclStatusManager(RFX_SLOT_ID_UNKNOWN)
                                     ->getIntValue(RFX_STATUS_KEY_MAIN_CAPABILITY_SLOT, 0);
            RFX_LOG_I("RfxMclMessenger", "%s(%d) dispatchs to main protocol: %d",
                      RFX_ID_TO_STR(msg->getId()), msg->getId(), mainSlotId);
            channelId = RfxHandlerManager::findMsgChannel(
                    msg->getType(), mainSlotId, msg->getId(), msg->getClientId(),
                    (msg->getRawUrc() == NULL ? NULL : msg->getRawUrc()->getLine()));
        } else {
            channelId = RfxHandlerManager::findMsgChannel(
                    msg->getType(), msg->getSlotId(), msg->getId(), msg->getClientId(),
                    (msg->getRawUrc() == NULL ? NULL : msg->getRawUrc()->getLine()));
        }
        msg->setChannelId(channelId);

        // execute message
        if (channelId != -1) {
            RfxChannel* channel = RfxChannelManager::getChannel(channelId);
            if (msg->getAddAtFront()) {
                channel->enqueueMessageFront(msg);
            } else {
                channel->enqueueMessage(msg);
            }
        } else {
            RFX_LOG_D("RfxMclMessenger", "onHandleMessage, no one register id: %d", msg->getId());
            // send response with error code: not support
            sp<RfxMclMessage> resMsg =
                    RfxMclMessage::obtainResponse(RIL_E_REQUEST_NOT_SUPPORTED, RfxVoidData(), msg);
            RfxDispatchThread::enqueueResponseMessage(resMsg);
        }
    }
}

/*****************************************************************************
 * Class RfxMclDispatcherThread
 *****************************************************************************/
RfxMclDispatcherThread::RfxMclDispatcherThread() {}

void RfxMclDispatcherThread::init() {
    RFX_LOG_D(RFX_LOG_TAG, "init");
    sem_init(&sWaitLooperSem, 0, 0);
    s_self = new RfxMclDispatcherThread();
    s_self->run("RILD MCL Dispatcher Thread");
}

bool RfxMclDispatcherThread::threadLoop() {
    RFX_LOG_D(RFX_LOG_TAG, "threadLoop");
    m_looper = Looper::prepare(0);

    RfxVersionManager::init();
    RfxMclStatusManager::init();
    RfxFragmentEncoder::init();
    RfxChannelManager::init();

    sem_post(&sWaitLooperSem);

    int result;
    do {
        result = m_looper->pollAll(-1);
        RFX_LOG_D(RFX_LOG_TAG, "threadLoop, result = %d", result);
    } while (result == Looper::POLL_WAKE || result == Looper::POLL_CALLBACK);

    RFX_ASSERT(0);  // Can't go here
    return true;
}

void RfxMclDispatcherThread::enqueueMclMessage(const sp<RfxMclMessage>& message) {
    if (!RfxRilUtils::isInLogReductionList(message->getId())) {
        RFX_LOG_D(RFX_LOG_TAG, "enqueueMclMessage: %s", message->toString().string());
    }
    sp<MessageHandler> handler = new RfxMclMessenger(message);
    s_self->m_looper->sendMessage(handler, s_self->m_dummy_msg);
}

void RfxMclDispatcherThread::enqueueMclMessageFront(const sp<RfxMclMessage>& message) {
    RFX_LOG_D(RFX_LOG_TAG, "enqueueMclMessage: %s", message->toString().string());
    sp<MessageHandler> handler = new RfxMclMessenger(message);
    s_self->m_looper->sendMessageAtTime(0, handler, s_self->m_dummy_msg);
}

void RfxMclDispatcherThread::enqueueMclMessageDelay(const sp<RfxMclMessage>& message) {
    RFX_LOG_D(RFX_LOG_TAG, "enqueueMclMessage: %s", message->toString().string());
    sp<MessageHandler> handler = new RfxMclMessenger(message);
    s_self->m_looper->sendMessageDelayed(message->getDelayTime(), handler, s_self->m_dummy_msg);
}

sp<Looper> RfxMclDispatcherThread::waitLooper() {
    RFX_ASSERT(s_self != NULL);
    sWaitLooperMutex.lock();
    if (sNeedWaitLooper) {
        RFX_LOG_D(RFX_LOG_TAG, "waitLooper() begin");
        sem_wait(&sWaitLooperSem);
        sNeedWaitLooper = false;
        sem_destroy(&sWaitLooperSem);
        RFX_LOG_D(RFX_LOG_TAG, "waitLooper() end");
    }
    sWaitLooperMutex.unlock();
    return s_self->m_looper;
}

/*void RfxMclDispatcherThread::enqueueMessenger(const sp<RfxMclBaseMessenger> &messenger) {
    sp<MessageHandler> handler = messenger;
    s_self->m_looper->sendMessage(handler, s_self->m_dummy_msg);
}*/
