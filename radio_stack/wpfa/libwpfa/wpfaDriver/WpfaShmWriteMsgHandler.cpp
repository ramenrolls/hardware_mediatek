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

#include "WpfaDriver.h"
#include "WpfaShmWriteMsgHandler.h"

#define WPFA_D_LOG_TAG "WpfaShmWriteMsgHandler"

using ::android::Mutex;

WpfaShmWriteMsgHandler* WpfaShmWriteMsgHandler::s_self = NULL;

static sem_t sWaitLooperSem;
static bool sNeedWaitLooper = true;
static Mutex sWaitLooperMutex;

/*****************************************************************************
 * Class RuleReqMsgHandler
 *****************************************************************************/
void WpfaShmWriteMsgHandler::ShmWriteMsgHandler::handleMessage(const Message& message) {
    WPFA_UNUSED(message);
    sender->processMessage(msg);
}

/*****************************************************************************
 * Class WpfaShmWriteMsgHandler
 *****************************************************************************/
WpfaShmWriteMsgHandler::WpfaShmWriteMsgHandler() {
    mDriverAdapter = NULL;
    mShmSynchronizer = NULL;
}

void WpfaShmWriteMsgHandler::init() {
    mtkLogD(WPFA_D_LOG_TAG, "init");
    sem_init(&sWaitLooperSem, 0, 0);
    s_self = new WpfaShmWriteMsgHandler();
    s_self->run("WpfaShmWriteMsgHandler init");
}

bool WpfaShmWriteMsgHandler::threadLoop() {
    mtkLogD(WPFA_D_LOG_TAG, "threadLoop");
    int err = 0;
    mLooper = Looper::prepare(0);
    sem_post(&sWaitLooperSem);

    int result;
    do {
        result = mLooper->pollAll(-1);
        mtkLogD(WPFA_D_LOG_TAG, "threadLoop, result = %d, err=%d", result, err);
    } while (result == Looper::POLL_WAKE || result == Looper::POLL_CALLBACK);

    WPFA_D_ASSERT(0);  // Can't go here
    return true;
}

void WpfaShmWriteMsgHandler::enqueueShmWriteMessage(const sp<WpfaDriverMessage>& message) {
    // if (!RfxRilUtils::isInLogReductionList(message->getId())) {
    mtkLogD(WPFA_D_LOG_TAG, "enqueueShmWriteMessage: %s", message->toString().string());
    //}
    WpfaShmWriteMsgHandler* sender = s_self;
    sp<MessageHandler> handler = new ShmWriteMsgHandler(sender, message);
    s_self->mLooper->sendMessage(handler, s_self->mDummyMsg);
}

void WpfaShmWriteMsgHandler::enqueueShmWriteMessageFront(const sp<WpfaDriverMessage>& message) {
    mtkLogD(WPFA_D_LOG_TAG, "enqueueShmWriteMessageFront: %s", message->toString().string());
    WpfaShmWriteMsgHandler* sender = s_self;
    sp<MessageHandler> handler = new ShmWriteMsgHandler(sender, message);
    s_self->mLooper->sendMessageAtTime(0, handler, s_self->mDummyMsg);
}

sp<Looper> WpfaShmWriteMsgHandler::waitLooper() {
    WPFA_D_ASSERT(s_self != NULL);
    sWaitLooperMutex.lock();
    if (sNeedWaitLooper) {
        mtkLogD(WPFA_D_LOG_TAG, "waitLooper() begin");
        sem_wait(&sWaitLooperSem);
        sNeedWaitLooper = false;
        sem_destroy(&sWaitLooperSem);
        mtkLogD(WPFA_D_LOG_TAG, "waitLooper() end");
    }
    sWaitLooperMutex.unlock();
    return s_self->mLooper;
}

void WpfaShmWriteMsgHandler::processMessage(const sp<WpfaDriverMessage>& msg) {
    uint16_t msgId = msg->getMsgId();
    uint16_t tId = msg->getTid();
    mtkLogD(WPFA_D_LOG_TAG, "processMessage, msgId=%d tId=%d", msgId, tId);
    switch (msgId) {
        case MSG_A2M_DATA_READY:
        case MSG_A2M_REQUEST_DATA_ACK:
            if (checkDriverAdapterState() == 0) {
                mDriverAdapter->sendMsgToControlMsgDispatcher(msg);
            }
            break;
        case MSG_M2A_REQUEST_DATA:
        case MSG_M2A_REQUEST_DATA_DONE:
            if (checkShmControllerState() == 0) {
                mShmSynchronizer->processControlMessage(tId, msgId);
            }
            break;
        default:
            mtkLogE(WPFA_D_LOG_TAG, "processMessage, type=%d not support", msgId);
    }
}

int WpfaShmWriteMsgHandler::checkDriverAdapterState() {
    int retValue = 0;
    if (mDriverAdapter != NULL) {
        return retValue;
    } else {
        mDriverAdapter = WpfaDriverAdapter::getInstance();
        if (mDriverAdapter == NULL) {
            mtkLogE(WPFA_D_LOG_TAG, "getInstance of WpfaDriverAdapter fail");
            retValue = -1;
        }
    }
    return retValue;
}

int WpfaShmWriteMsgHandler::checkShmControllerState() {
    int retValue = 0;
    if (mShmSynchronizer != NULL) {
        return retValue;
    } else {
        mShmSynchronizer = WpfaShmSynchronizer::getInstance();
        if (mShmSynchronizer == NULL) {
            mtkLogE(WPFA_D_LOG_TAG, "getInstance of checkShmControllerState fail");
            retValue = -1;
        }
    }
    return retValue;
}
int WpfaShmWriteMsgHandler::sendMessageToModem(uint16_t msgId, uint16_t tId) {
    int retValue = checkDriverAdapterState();
    if (retValue == 0) {
        sp<WpfaDriverMessage> msg = WpfaDriverMessage::obtainMessage(msgId, tId, CCCI_CTRL_MSG, 0);
        retValue = mDriverAdapter->sendMsgToControlMsgDispatcher(msg);
    }
    return retValue;
}
