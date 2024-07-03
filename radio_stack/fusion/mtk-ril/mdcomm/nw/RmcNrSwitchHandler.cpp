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

#include "RmcNrSwitchHandler.h"

static const int request[] = {RFX_MSG_REQUEST_NR_SWITCH};

#define MAX_NR_SWITCH_RETRY_TIME 40

// register data
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_NR_SWITCH);

// register handler to channel
RFX_IMPLEMENT_HANDLER_CLASS(RmcNrSwitchHandler, RIL_CMD_PROXY_9);

RmcNrSwitchHandler::RmcNrSwitchHandler(int slot_id, int channel_id)
    : RfxBaseHandler(slot_id, channel_id) {
    m_slot_id = slot_id;
    m_channel_id = channel_id;
    mRetryCount = 1;
    registerToHandleRequest(request, sizeof(request) / sizeof(int));
}

RmcNrSwitchHandler::~RmcNrSwitchHandler() {}

void RmcNrSwitchHandler::onHandleRequest(const sp<RfxMclMessage>& msg) {
    // logD(LOG_TAG, "[onHandleRequest] %s", RFX_ID_TO_STR(msg->getId()));
    int request = msg->getId();
    switch (request) {
        case RFX_MSG_REQUEST_NR_SWITCH:
            requestNrSwitch(msg);
            break;
        default:
            logE(LOG_TAG, "Should not be here");
            break;
    }
}

void RmcNrSwitchHandler::requestNrSwitch(const sp<RfxMclMessage>& msg) {
    RIL_Errno ril_errno = RIL_E_SUCCESS;
    sp<RfxAtResponse> p_response;
    sp<RfxMclMessage> resp;
    int slot;

    int* pInt = (int*)msg->getData()->getData();
    slot = pInt[0];
    slot++;
    p_response = atSendCommand(String8::format("AT+EPSCONFIG=1,\"nr_ps\",%d", slot));
    if (p_response->getError() >= 0 && p_response->getSuccess() != 0) {
        logV(LOG_TAG, "requestNrSwitch ok");
        rfx_property_set("persist.vendor.radio.nrslot", String8::format("%d", slot).string());
        ril_errno = RIL_E_SUCCESS;
    } else {
        logE(LOG_TAG, "requestNrSwitch fail, count=%d, error=%d", mRetryCount,
             p_response->atGetCmeError());
        if (p_response->atGetCmeError() == CME_OPERATION_NOT_ALLOWED_ERR) {
            ril_errno = RIL_E_GENERIC_FAILURE;
            logE(LOG_TAG, "requestNrSwitch: fail, modem report no retry");
        } else {  // retry 40 times if Nr switch fail.
            if (mRetryCount < MAX_NR_SWITCH_RETRY_TIME) {
                mRetryCount++;
                sp<RfxMclMessage> mclMessage = RfxMclMessage::obtainRequest(
                        msg->getId(), msg->getData(), msg->getSlotId(), msg->getToken(),
                        msg->getSendToMainProtocol(), msg->getRilToken(), ms2ns(100),
                        msg->getTimeStamp(), msg->getAddAtFront());
                RfxMclDispatcherThread::enqueueMclMessageDelay(mclMessage);
                return;
            } else {
                char pErrMsg[] = "requestNrSwitch: retry max asserion!";
                mtkAssert(pErrMsg);
            }
        }
    }
    p_response = NULL;

finish:
    mRetryCount = 1;
    sp<RfxMclMessage> response =
            RfxMclMessage::obtainResponse(msg->getId(), ril_errno, RfxVoidData(), msg, false);
    // response to TeleCore
    responseToTelCore(response);
}
