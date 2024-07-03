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

#include "RmcRadioRelatedRequestHandler.h"
#include "RfxStringsData.h"
#include "RfxVoidData.h"

#define RFX_LOG_TAG "RmcRadioRelReqHandler"

// register handler to channel
RFX_IMPLEMENT_HANDLER_CLASS(RmcRadioRelatedRequestHandler, RIL_CMD_PROXY_9);

RmcRadioRelatedRequestHandler::RmcRadioRelatedRequestHandler(int slot_id, int channel_id)
    : RfxBaseHandler(slot_id, channel_id) {
    logD(RFX_LOG_TAG, "RmcRadioRelatedRequestHandler constructor");
    // For commands that need to send on same channel as EFUN, usally before EFUN=1
    const int requests[] = {
            RFX_MSG_REQUEST_SET_ECC_MODE,  // AT+EMCS
    };

    registerToHandleRequest(requests, sizeof(requests) / sizeof(int));
}

RmcRadioRelatedRequestHandler::~RmcRadioRelatedRequestHandler() {}

void RmcRadioRelatedRequestHandler::onHandleRequest(const sp<RfxMclMessage>& msg) {
    logD(RFX_LOG_TAG, "onHandleRequest: %d", msg->getId());
    int request = msg->getId();
    switch (request) {
        case RFX_MSG_REQUEST_SET_ECC_MODE:
            requestSetEccMode(msg);
            break;

        default:
            logE(RFX_LOG_TAG, "Should not be here");
            break;
    }
}

void RmcRadioRelatedRequestHandler::requestSetEccMode(const sp<RfxMclMessage>& msg) {
    sp<RfxAtResponse> p_response;
    RIL_Errno rilErrNo = RIL_E_SUCCESS;

    RfxStringsData* data = (RfxStringsData*)msg->getData();
    char** params = (char**)data->getData();
    int enable = atoi(params[1]);
    int airplaneMode = atoi(params[2]);
    // int imsReg = atoi(params[3]);
    /*
    AT+EMCS:<emc_session>,<airplane_mode>
    <emc_session> :  1 : emergency session begin
    <airplaine_mode> : 0 : off, 1 : on
    */

    bool currentMode = getNonSlotMclStatusManager()->getBoolValue(
            RFX_STATUS_KEY_EMERGENCY_MODE_IN_FLIGHT_MODE, false);
    logD(RFX_LOG_TAG, "currentMode=%d, enable=%d, airplane=%d", currentMode, enable, airplaneMode);

    if (airplaneMode == 0 || (enable != 0 && enable != 1)) {
        logD(RFX_LOG_TAG, "invalid param");
        rilErrNo = RIL_E_INTERNAL_ERR;
    } else if ((currentMode && enable == 1) || (!currentMode && enable == 0)) {
        logD(RFX_LOG_TAG, "duplicate request");
        rilErrNo = RIL_E_INTERNAL_ERR;
    } else {
        if (enable == 1) {
            if (RfxRilUtils::isEmciSupport()) {
                p_response = atSendCommand(String8::format("AT+EMCI=1"));
            } else {
                p_response = atSendCommand(String8::format("AT+EMCS=1,%d", airplaneMode));
            }
            getNonSlotMclStatusManager()->setBoolValue(RFX_STATUS_KEY_EMERGENCY_MODE_IN_FLIGHT_MODE,
                                                       true);
        } else {
            if (RfxRilUtils::isEmciSupport()) {
                p_response = atSendCommand(String8::format("AT+EMCI=0"));
            } else {
                p_response = atSendCommand(String8::format("AT+EMCS=0"));
            }
            getNonSlotMclStatusManager()->setBoolValue(RFX_STATUS_KEY_EMERGENCY_MODE_IN_FLIGHT_MODE,
                                                       false);
        }
        // set result
        if (p_response->getError() != 0 || p_response->getSuccess() != 1) {
            rilErrNo = RIL_E_INTERNAL_ERR;
        }
    }

    sp<RfxMclMessage> response =
            RfxMclMessage::obtainResponse(msg->getId(), rilErrNo, RfxVoidData(), msg);
    responseToTelCore(response);
}
