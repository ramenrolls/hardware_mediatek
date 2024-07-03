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

#include "RmcImsProvisioningRequestHandler.h"

static const int requests[] = {
        RFX_MSG_REQUEST_IMS_CONFIG_GET_MD_PROVISION,  // AT+EPVSGET
        RFX_MSG_REQUEST_IMS_CONFIG_SET_PROVISION,     // AT+EPVSSET
};

RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringData, RfxVoidData,
                                RFX_MSG_REQUEST_IMS_CONFIG_GET_MD_PROVISION);

// register handler to channel
RFX_IMPLEMENT_HANDLER_CLASS(RmcImsProvisioningRequestHandler, RIL_CMD_PROXY_1);

RmcImsProvisioningRequestHandler::RmcImsProvisioningRequestHandler(int slot_id, int channel_id)
    : RfxBaseHandler(slot_id, channel_id) {
    logD(RFX_LOG_TAG, "RmcImsProvisioningRequestHandler constructor");
    registerToHandleRequest(requests, sizeof(requests) / sizeof(int));
}

RmcImsProvisioningRequestHandler::~RmcImsProvisioningRequestHandler() {}

void RmcImsProvisioningRequestHandler::onHandleRequest(const sp<RfxMclMessage>& msg) {
    logD(RFX_LOG_TAG, "onHandleRequest: %d", msg->getId());
    int request = msg->getId();
    switch (request) {
        case RFX_MSG_REQUEST_IMS_CONFIG_GET_MD_PROVISION:
            requestGetProvisionValue(msg);
            break;
        case RFX_MSG_REQUEST_IMS_CONFIG_SET_PROVISION:
            requestSetProvisionValue(msg);
            break;
        default:
            logE(RFX_LOG_TAG, "Should not be here");
            break;
    }
}

void RmcImsProvisioningRequestHandler::requestGetProvisionValue(const sp<RfxMclMessage>& msg) {
    char* pvsString = (char*)msg->getData()->getData();
    sp<RfxAtResponse> p_response;
    sp<RfxMclMessage> response;
    RIL_Errno rilErrNo = RIL_E_SUCCESS;
    // send AT command
    logD(RFX_LOG_TAG, "requestGetProvisionValue AT+EPVSGET=\"%s\"", pvsString);
    p_response = atSendCommand(String8::format("AT+EPVSGET=\"%s\"", pvsString));
    // set result
    if (p_response->getError() != 0 || p_response->getSuccess() != 1) {
        rilErrNo = RIL_E_GENERIC_FAILURE;

        if (p_response->getSuccess() != 1) {
            int err;
            RfxAtLine* p_cur;

            p_cur = p_response->getFinalResponse();
            if (p_cur->isFinalResponseErrorEx(m_channel_id)) {
                logD(RFX_LOG_TAG, "GetProvisionValue error =%s",
                     p_response->getFinalResponse()->getLine());
                if (strStartsWith(p_response->getFinalResponse()->getLine(), "+CME ERROR: 604")) {
                    rilErrNo = RIL_E_OEM_ERROR_24;
                }
            }
        }
    }
    response = RfxMclMessage::obtainResponse(msg->getId(), rilErrNo, RfxVoidData(), msg, false);
    // response to TeleCore
    responseToTelCore(response);
}

void RmcImsProvisioningRequestHandler::requestSetProvisionValue(const sp<RfxMclMessage>& msg) {
    const char** pvsStrings = (const char**)msg->getData()->getData();
    sp<RfxAtResponse> p_response;
    sp<RfxMclMessage> response;
    RIL_Errno rilErrNo = RIL_E_SUCCESS;
    // send AT command
    logD(RFX_LOG_TAG, "requestSetProvisionValue AT+EPVSSET=\"%s\",\"%s\"", pvsStrings[0],
         pvsStrings[1]);
    if (pvsStrings[1] == NULL) {
        p_response = atSendCommand(String8::format("AT+EPVSSET=\"%s\",\"\"", pvsStrings[0]));
    } else {
        p_response = atSendCommand(
                String8::format("AT+EPVSSET=\"%s\",\"%s\"", pvsStrings[0], pvsStrings[1]));
    }
    // set result
    if (p_response->getError() != 0 || p_response->getSuccess() != 1) {
        rilErrNo = RIL_E_GENERIC_FAILURE;
    }
    response = RfxMclMessage::obtainResponse(msg->getId(), rilErrNo, RfxVoidData(), msg, false);
    // response to TeleCore
    responseToTelCore(response);
}
