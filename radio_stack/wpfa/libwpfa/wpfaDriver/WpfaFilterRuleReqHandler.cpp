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

#include "WpfaFilterRuleReqHandler.h"

#define WPFA_D_LOG_TAG "WpfaFilterRuleReqHandler"

using ::android::Mutex;

WpfaFilterRuleReqHandler* WpfaFilterRuleReqHandler::s_self = NULL;

static sem_t sWaitLooperSem;
static bool sNeedWaitLooper = true;
static Mutex sWaitLooperMutex;

/*****************************************************************************
 * Class RuleReqMsgHandler
 *****************************************************************************/
void WpfaFilterRuleReqHandler::RuleReqMsgHandler::handleMessage(const Message& message) {
    WPFA_UNUSED(message);
    sender->processMessage(msg);
}

/*****************************************************************************
 * Class WpfaFilterRuleReqHandler
 *****************************************************************************/
WpfaFilterRuleReqHandler::WpfaFilterRuleReqHandler() {
    mWpfaDriver = NULL;
    mDriverAdapter = NULL;
}

void WpfaFilterRuleReqHandler::init() {
    mtkLogD(WPFA_D_LOG_TAG, "init");
    sem_init(&sWaitLooperSem, 0, 0);
    s_self = new WpfaFilterRuleReqHandler();
    s_self->run("Wpfa Driver control message dispatcher");
}

bool WpfaFilterRuleReqHandler::threadLoop() {
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

void WpfaFilterRuleReqHandler::enqueueFilterRuleReqMessage(const sp<WpfaDriverMessage>& message) {
    // if (!RfxRilUtils::isInLogReductionList(message->getId())) {
    mtkLogD(WPFA_D_LOG_TAG, "enqueueFilterRuleReqMessage: %s", message->toString().string());
    //}
    WpfaFilterRuleReqHandler* sender = s_self;
    sp<MessageHandler> handler = new RuleReqMsgHandler(sender, message);
    s_self->mLooper->sendMessage(handler, s_self->mDummyMsg);
}

void WpfaFilterRuleReqHandler::enqueueFilterRuleReqMessageFront(
        const sp<WpfaDriverMessage>& message) {
    mtkLogD(WPFA_D_LOG_TAG, "enqueueFilterRuleReqMessageFront: %s", message->toString().string());
    WpfaFilterRuleReqHandler* sender = s_self;
    sp<MessageHandler> handler = new RuleReqMsgHandler(sender, message);
    s_self->mLooper->sendMessageAtTime(0, handler, s_self->mDummyMsg);
}

sp<Looper> WpfaFilterRuleReqHandler::waitLooper() {
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

void WpfaFilterRuleReqHandler::processMessage(const sp<WpfaDriverMessage>& msg) {
    int eventId = -1;
    uint16_t msgId = msg->getMsgId();
    uint32_t retValue = 0;

    mtkLogD(WPFA_D_LOG_TAG, "processMessage() msgId=%d", msgId);
    eventId = DriverMsgIdToEventId(msgId);
    if (eventId > 0 && eventId < EVENT_M2A_MAX) {
        if (checkDriverState() == 0) {
            retValue = mWpfaDriver->notifyCallback(eventId, msg->getData()->getData());
            mtkLogD(WPFA_D_LOG_TAG, "notifyCallback() msgId=%d eventId=%d ret=%d", msgId, eventId,
                    retValue);

            if (msgId == MSG_M2A_REG_DL_FILTER) {
                wifiproxy_m2a_reg_dl_filter_t* pReqFilter =
                        (wifiproxy_m2a_reg_dl_filter_t*)msg->getData()->getData();
                sendRegAcceptResponse(msg->getTid(), pReqFilter->fid, retValue);
            } else if (msgId == MSG_M2A_DEREG_DL_FILTER) {
                wifiproxy_m2a_dereg_dl_filter_t* pDeregFilter =
                        (wifiproxy_m2a_dereg_dl_filter_t*)msg->getData()->getData();
                sendDeregAcceptResponse(msg->getTid(), pDeregFilter->fid, retValue);
            } else {
                // no need to handle "MSG_M2A_WPFA_VERSION"
            }
        }
    } else {
        mtkLogE(WPFA_D_LOG_TAG, "can't handle msgId=%d", msgId);
    }
}

int WpfaFilterRuleReqHandler::sendRegAcceptResponse(uint16_t tId, uint32_t fId, int32_t errCause) {
    mtkLogD(WPFA_D_LOG_TAG, "sendRegAcceptResponse() tid=%d fId=%d errCause=%d", tId, fId,
            errCause);
    int retValue = checkDriverAdapterState();
    if (retValue == 0) {
        wifiproxy_a2m_reg_reply_t* pAcceptData =
                (wifiproxy_a2m_reg_reply_t*)calloc(1, sizeof(wifiproxy_a2m_reg_reply_t));
        pAcceptData->fid = fId;
        pAcceptData->error_cause = errCause;

        sp<WpfaDriverMessage> msg = WpfaDriverMessage::obtainMessage(
                MSG_A2M_REG_REPLY, tId, CCCI_IP_TABLE_MSG, 0, WpfaDriverAcceptData(pAcceptData, 1));
        retValue = mDriverAdapter->sendMsgToControlMsgDispatcher(msg);
        free(pAcceptData);
    }
    return retValue;
}

int WpfaFilterRuleReqHandler::sendDeregAcceptResponse(uint16_t tId, uint32_t fId,
                                                      int32_t errCause) {
    mtkLogD(WPFA_D_LOG_TAG, "sendDeregAcceptResponse() tid=%d fId=%d errCause=%d", tId, fId,
            errCause);
    int retValue = checkDriverAdapterState();
    if (retValue == 0) {
        wifiproxy_a2m_dereg_reply_t* pDeregAcceptData =
                (wifiproxy_a2m_dereg_reply_t*)calloc(1, sizeof(wifiproxy_a2m_dereg_reply_t));
        pDeregAcceptData->fid = fId;
        pDeregAcceptData->error_cause = errCause;

        sp<WpfaDriverMessage> msg =
                WpfaDriverMessage::obtainMessage(MSG_A2M_DEREG_REPLY, tId, CCCI_IP_TABLE_MSG, 0,
                                                 WpfaDriverAcceptData(pDeregAcceptData, 1));
        retValue = mDriverAdapter->sendMsgToControlMsgDispatcher(msg);
        free(pDeregAcceptData);
    }
    return retValue;
}

int WpfaFilterRuleReqHandler::checkDriverState() {
    int retValue = 0;
    if (mWpfaDriver != NULL) {
        return retValue;
    } else {
        mWpfaDriver = WpfaDriver::getInstance();
        if (mWpfaDriver == NULL) {
            mtkLogE(WPFA_D_LOG_TAG, "getInstance of WpfaDriver fail");
            retValue = -1;
        }
    }
    return retValue;
}

int WpfaFilterRuleReqHandler::checkDriverAdapterState() {
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

int WpfaFilterRuleReqHandler::DriverMsgIdToEventId(uint16_t msgId) {
    int eventId = -1;
    switch (msgId) {
        case MSG_M2A_WPFA_VERSION:
            eventId = EVENT_M2A_WPFA_VERSION;
            break;
        case MSG_M2A_REG_DL_FILTER:
            eventId = EVENT_M2A_REG_DL_FILTER;
            break;
        case MSG_M2A_DEREG_DL_FILTER:
            eventId = EVENT_M2A_DEREG_DL_FILTER;
            break;
        default:
            mtkLogE(WPFA_D_LOG_TAG, "DriverMsgIdToEventId, msgId=%d not support", msgId);
    }
    return eventId;
}
