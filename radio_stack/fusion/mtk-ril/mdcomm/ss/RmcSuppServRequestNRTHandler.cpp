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

#include "RmcSuppServRequestNRTHandler.h"
#include "RfxMessageId.h"

#include <telephony/mtk_ril.h>
#include <mtkconfigutils.h>

RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxVoidData, RFX_MSG_REQUEST_GET_XCAP_STATUS);

static const int requests[] = {RFX_MSG_REQUEST_GET_XCAP_STATUS};

// register handler to non real time channel
RFX_IMPLEMENT_HANDLER_CLASS(RmcSuppServRequestNRTHandler, RIL_CMD_PROXY_8);

RmcSuppServRequestNRTHandler::RmcSuppServRequestNRTHandler(int slot_id, int channel_id)
    : RfxBaseHandler(slot_id, channel_id) {
    registerToHandleRequest(requests, sizeof(requests) / sizeof(int));
}

RmcSuppServRequestNRTHandler::~RmcSuppServRequestNRTHandler() {}

void RmcSuppServRequestNRTHandler::onHandleRequest(const sp<RfxMclMessage>& msg) {
    logD(TAG, "onHandleRequest: %d", msg->getId());
    int request = msg->getId();
    switch (request) {
        case RFX_MSG_REQUEST_GET_XCAP_STATUS:
            requestGetXcapStatus(msg);
            break;
        default:
            logE(TAG, "Should not be here");
            break;
    }
}

void RmcSuppServRequestNRTHandler::requestGetXcapStatus(const sp<RfxMclMessage>& msg) {
    sp<RfxAtResponse> p_response;
    int err;
    char* cmd = NULL;
    RIL_Errno ret = RIL_E_GENERIC_FAILURE;

    asprintf(&cmd, "AT+ECUSD=1,1,\"*#21#\",,,1");

    p_response = atSendCommandSingleline(cmd, "+CCFC:");
    free(cmd);

    err = p_response->getError();

    if (err < 0 || p_response == NULL) {
        logD(TAG, "requestGetXcapStatus failed.");
        goto error;
    }

    switch (p_response->atGetCmeError()) {
        case CME_SUCCESS:
            break;
        case CME_CALL_BARRED:
        case CME_OPR_DTR_BARRING:
            ret = RIL_E_CALL_BARRED;
            goto error;
            break;
        case CME_PHB_FDN_BLOCKED:
            ret = RIL_E_FDN_CHECK_FAILURE;
            goto error;
            break;
        case CME_403_FORBIDDEN:
            ret = RIL_E_UT_XCAP_403_FORBIDDEN;
            goto error;
            break;
        case CME_404_NOT_FOUND:
            ret = RIL_E_404_NOT_FOUND;
            goto error;
            break;
        case CME_409_CONFLICT: {
            ret = RIL_E_409_CONFLICT;
            goto error;
            break;
        }
        case CME_412_PRECONDITION_FAILED:
            ret = RIL_E_412_PRECONDITION_FAILED;
            goto error;
            break;
        case CME_NETWORK_TIMEOUT:
            ret = RIL_E_UT_UNKNOWN_HOST;
            goto error;
            break;
        case CME_OPERATION_NOT_SUPPORTED:
            ret = RIL_E_REQUEST_NOT_SUPPORTED;
            goto error;
            break;
        case CME_844_UNPROVISIONED:
            ret = RIL_E_REQUEST_NOT_SUPPORTED;
            goto error;
            break;
        case CME_845_NO_DATA_CONNECTION:
            ret = RIL_E_845_NO_DATA_CONNECTION;
            goto error;
            break;
        default:
            goto error;
    }

    /* return success here */
    ret = RIL_E_SUCCESS;

error:
    sp<RfxMclMessage> response =
            RfxMclMessage::obtainResponse(msg->getId(), ret, RfxVoidData(), msg, false);

    // response to TeleCore
    responseToTelCore(response);
}
