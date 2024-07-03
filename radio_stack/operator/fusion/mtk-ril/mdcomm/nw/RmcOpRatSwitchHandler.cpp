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

#include "RmcOpRatSwitchHandler.h"

static const int request[] = {RFX_MSG_REQUEST_SET_DISABLE_2G, RFX_MSG_REQUEST_GET_DISABLE_2G};

// register handler to channel
RFX_IMPLEMENT_OP_HANDLER_CLASS(RmcOpRatSwitchHandler, RIL_CMD_PROXY_9);

RmcOpRatSwitchHandler::RmcOpRatSwitchHandler(int slot_id, int channel_id)
    : RmcNetworkHandler(slot_id, channel_id) {
    registerToHandleRequest(request, sizeof(request) / sizeof(int));
    logV(LOG_TAG, "[RmcOpRatSwitchHandler] init");
}

RmcOpRatSwitchHandler::~RmcOpRatSwitchHandler() {}

void RmcOpRatSwitchHandler::onHandleRequest(const sp<RfxMclMessage>& msg) {
    logD(LOG_TAG, "[onHandleRequest] %s", RFX_ID_TO_STR(msg->getId()));
    int request = msg->getId();
    switch (request) {
        case RFX_MSG_REQUEST_SET_DISABLE_2G:
            requestSetDisable2G(msg);
            break;
        case RFX_MSG_REQUEST_GET_DISABLE_2G:
            requestGetDisable2G(msg);
            break;
        default:
            logE(LOG_TAG, "Should not be here");
            break;
    }
}

void RmcOpRatSwitchHandler::requestSetDisable2G(const sp<RfxMclMessage>& msg) {
    RIL_Errno ril_errno = RIL_E_MODE_NOT_SUPPORTED;
    sp<RfxAtResponse> p_response;
    sp<RfxMclMessage> resp;
    int mode = 0;

    int* pInt = (int*)msg->getData()->getData();
    mode = pInt[0];
    if (mode == 1 || mode == 0) {
        if (isOp07Support()) {
            logD(LOG_TAG, "[OP07] requestSetDisable2G %d", mode);
            p_response = atSendCommand(String8::format("AT+EDRAT=%d", mode));
            if (p_response->getError() >= 0 && p_response->getSuccess() != 0) {
                rfx_property_set("persist.vendor.radio.disable.2g",
                                 String8::format("%d", mode).string());
                ril_errno = RIL_E_SUCCESS;
            }
        } else if (isOp08Support()) {
            logD(LOG_TAG, "[OP08] requestSetDisable2G %d", mode);
            p_response =
                    atSendCommand(String8::format("AT+EGMC=1,\"rat_disable_with_ecc\",%d", mode));
            if (p_response->getError() >= 0 && p_response->getSuccess() != 0) {
                ril_errno = RIL_E_SUCCESS;
            }
        } else {
            logD(LOG_TAG, "NOT SUPPORT, requestSetDisable2G %d", mode);
        }
    }
    p_response = NULL;

error:
    resp = RfxMclMessage::obtainResponse(msg->getId(), ril_errno, RfxVoidData(), msg, false);
    responseToTelCore(resp);
}

void RmcOpRatSwitchHandler::requestGetDisable2G(const sp<RfxMclMessage>& msg) {
    RIL_Errno ril_errno = RIL_E_MODE_NOT_SUPPORTED;
    sp<RfxAtResponse> p_response;
    sp<RfxMclMessage> resp;
    RfxAtLine* line;
    int err = 0, mode = 0;
    char* type = NULL;
    char property_value[RFX_PROPERTY_VALUE_MAX] = {0};

    if (isOp07Support()) {
        rfx_property_get("persist.vendor.radio.disable.2g", property_value, "0");
        mode = atoi(property_value);
        logD(LOG_TAG, "[OP07] requestGetDisable2G %d", mode);
        ril_errno = RIL_E_SUCCESS;
    } else if (isOp08Support()) {
        p_response = atSendCommandSingleline("AT+EGMC=0,\"rat_disable_with_ecc\"", "+EGMC:");

        err = p_response->getError();
        if (err < 0 || p_response->getSuccess() == 0) goto error;

        line = p_response->getIntermediates();

        // go to start position
        line->atTokStart(&err);
        if (err < 0) goto error;

        // get "type"
        type = line->atTokNextstr(&err);
        if (err < 0) goto error;

        if (strncmp(type, "rat_disable_with_ecc", strlen("rat_disable_with_ecc")) != 0) {
            goto error;
        }

        // get <mode>
        mode = line->atTokNextint(&err);
        if (err < 0) goto error;

        logD(LOG_TAG, "[OP08] requestGetDisable2G %d", mode);
        ril_errno = RIL_E_SUCCESS;
    } else {
        logD(LOG_TAG, "NOT SUPPORT, requestGetDisable2G %d", mode);
    }

error:
    resp = RfxMclMessage::obtainResponse(msg->getId(), ril_errno, RfxIntsData(&mode, 1), msg,
                                         false);
    responseToTelCore(resp);
}
