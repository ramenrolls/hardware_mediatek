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

#include "RmcOpSuppServRequestHandler.h"
#include "RfxMessageId.h"
#include <telephony/mtk_ril.h>

#include <string.h>
#include <dlfcn.h>

RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringsData, RfxVoidData, RFX_MSG_REQUEST_SEND_USSI_FROM);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringData, RfxVoidData, RFX_MSG_REQUEST_CANCEL_USSI_FROM);

static const int requests[] = {
        RFX_MSG_REQUEST_SEND_USSI_FROM,
        RFX_MSG_REQUEST_CANCEL_USSI_FROM,
};

// register handler to channel
RFX_IMPLEMENT_OP_HANDLER_CLASS(RmcOpSuppServRequestHandler, RIL_CMD_PROXY_2);

RmcOpSuppServRequestHandler::RmcOpSuppServRequestHandler(int slot_id, int channel_id)
    : RfxBaseHandler(slot_id, channel_id) {
    registerToHandleRequest(requests, sizeof(requests) / sizeof(int));
}

RmcOpSuppServRequestHandler::~RmcOpSuppServRequestHandler() {}

void RmcOpSuppServRequestHandler::onHandleRequest(const sp<RfxMclMessage>& msg) {
    logD(TAG, "onHandleRequest: %d", msg->getId());
    int request = msg->getId();
    switch (request) {
        case RFX_MSG_REQUEST_SEND_USSI_FROM:
            requestSendUssiFrom(msg);
            break;

        case RFX_MSG_REQUEST_CANCEL_USSI_FROM:
            requestCancelUssiFrom(msg);
            break;

        default:
            logE(TAG, "Should not be here");
            break;
    }
}

void RmcOpSuppServRequestHandler::onHandleTimer() {
    // do something
}

void RmcOpSuppServRequestHandler::requestSendUssiFrom(const sp<RfxMclMessage>& msg) {
    const char** strings = (const char**)(msg->getData()->getData());
    sp<RfxAtResponse> p_response;
    const char* from = strings[0];
    int action = atoi(strings[1]);
    const char* ussi = strings[2];
    int err;
    RIL_Errno ret = RIL_E_GENERIC_FAILURE;

    logD(TAG, "requestSendUssiFrom: from=%s, action=%d, ussi=%s", from, action, ussi);

    p_response = atSendCommand(
            String8::format("AT+EIUSD=2,1,%d,\"%s\",\"en\",0,\"%s\"", action, ussi, from));

    err = p_response->getError();
    if (err < 0 || p_response->getSuccess() == 0) {
        logE(TAG, "requestSendUssiFrom Fail");
    } else {
        ret = RIL_E_SUCCESS;
    }
    sp<RfxMclMessage> response =
            RfxMclMessage::obtainResponse(msg->getId(), ret, RfxVoidData(), msg, false);
    // response to TeleCore
    responseToTelCore(response);
}

void RmcOpSuppServRequestHandler::requestCancelUssiFrom(const sp<RfxMclMessage>& msg) {
    const char** strings = (const char**)(msg->getData()->getData());
    sp<RfxAtResponse> p_response;
    const char* from = strings[0];
    int err;
    RIL_Errno ret = RIL_E_GENERIC_FAILURE;

    logD(TAG, "requestCancelUssiFrom: from=%s", from);

    p_response = atSendCommand(String8::format("AT+EIUSD=2,2,2,\"\",\"en\",0,\"%s\"", from));

    err = p_response->getError();
    if (err < 0 || p_response->getSuccess() == 0) {
        logD(TAG, "requestCancelUssiFrom failed.");
    } else {
        ret = RIL_E_SUCCESS;
    }
    sp<RfxMclMessage> response =
            RfxMclMessage::obtainResponse(msg->getId(), ret, RfxVoidData(), msg, false);
    // response to TeleCore
    responseToTelCore(response);
}
