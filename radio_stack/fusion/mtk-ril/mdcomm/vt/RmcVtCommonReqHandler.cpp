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

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "RfxVoidData.h"
#include "RfxIntsData.h"

#include "RmcVtCommonReqHandler.h"

#include "RfxVtSendMsgData.h"
#include "RmcVtMsgParser.h"

// for send MCL request to myself
#include "RfxMclDispatcherThread.h"
#include "RfxMclMessage.h"
#include "RfxMessageId.h"

// for socket / thread
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <cutils/properties.h>
#include <cutils/sockets.h>
#include <sys/socket.h>
#include <sys/un.h>

#define RFX_LOG_TAG "VT RIL CMN RMC"

/*****************************************************************************
 * Class RmcVtCommonReqHandler
 *****************************************************************************/
RFX_IMPLEMENT_HANDLER_CLASS(RmcVtCommonReqHandler, RIL_CMD_PROXY_2);

RmcVtCommonReqHandler::RmcVtCommonReqHandler(int slot_id, int channel_id)
    : RfxBaseHandler(slot_id, channel_id) {
    RFX_LOG_I(RFX_LOG_TAG, "[RMC VT REQ HDLR] RmcVtCommonReqHandler create (slot_id = %d)",
              slot_id);

    const int CommonEventList[] = {
            RFX_MSG_EVENT_REPORT_ANBR,
    };

    registerToHandleEvent(CommonEventList, sizeof(CommonEventList) / sizeof(int));

    const int requestList[] = {
            RFX_MSG_REQUEST_ENABLE_ANBR,
    };

    registerToHandleRequest(requestList, sizeof(requestList) / sizeof(int));
}

RmcVtCommonReqHandler::~RmcVtCommonReqHandler() {}

void RmcVtCommonReqHandler::onHandleRequest(const sp<RfxMclMessage>& msg) {
    int request = msg->getId();

    RFX_LOG_I(RFX_LOG_TAG, "[Handle REQ] request = %d", request);

    switch (request) {
        case RFX_MSG_REQUEST_ENABLE_ANBR:
            RFX_LOG_I(RFX_LOG_TAG, "[Handle REQ] RFX_MSG_REQUEST_ENABLE_ANBR");
            handleEnableANBR(msg);
            break;
        default:
            RFX_LOG_I(RFX_LOG_TAG, "[Handle REQ] unknown request, ignore!");
            break;
    }
}

void RmcVtCommonReqHandler::onHandleEvent(const sp<RfxMclMessage>& msg) {
    int event = msg->getId();

    RFX_LOG_I(RFX_LOG_TAG, "[Handle EVT] event = %d", event);

    switch (event) {
        case RFX_MSG_EVENT_REPORT_ANBR:
            RFX_LOG_I(RFX_LOG_TAG, "[Handle EVT] RFX_MSG_EVENT_REPORT_ANBR");
            handleReportANBR(msg);
            break;

        default:
            RFX_LOG_I(RFX_LOG_TAG, "[Handle EVT] unknown event, ignore!");
            break;
    }
}

void RmcVtCommonReqHandler::handleReportANBR(const sp<RfxMclMessage>& msg) {
    char* cmd = NULL;
    sp<RfxAtResponse> p_response;

    int* pInt = (int*)msg->getData()->getData();
    int is_ul = pInt[0];
    int ebi = pInt[1];
    int bitrate = pInt[2];
    int bearer_id = pInt[3];
    int pdu_session_id = pInt[4];
    int ext_param = pInt[5];
    ;  // for future usage

    RFX_LOG_I(RFX_LOG_TAG,
              "[handleReportANBR] is_ul = %d, ebi = %d, bitrate = %d, bearer_id = %d, "
              "pdu_session_id = %d, ext_param = %d\n",
              is_ul, ebi, bitrate, bearer_id, pdu_session_id, ext_param);

    asprintf(&cmd, "AT+EANBR=2,%d,%d,%d,%d,%d,%d", ebi, is_ul, bitrate, bearer_id, pdu_session_id,
             ext_param);
    // test
    // asprintf(&cmd, "ATI");

    // send AT command
    p_response = atSendCommand(cmd);

    free(cmd);
    cmd = NULL;

    if (p_response->getSuccess() != 1) {
        RFX_LOG_I(RFX_LOG_TAG, "[handleReportANBR] response error");
    }
}

void RmcVtCommonReqHandler::handleEnableANBR(const sp<RfxMclMessage>& msg) {
    char* cmd = NULL;
    sp<RfxAtResponse> p_response;

    int* pInt = (int*)msg->getData()->getData();
    int enable = pInt[0];

    RFX_LOG_I(RFX_LOG_TAG, "[handleEnableANBR] enable = %d\n", enable);

    asprintf(&cmd, "AT+EANBR=%d", enable);

    // send AT command
    p_response = atSendCommand(cmd);

    free(cmd);
    cmd = NULL;

    if (p_response->getSuccess() != 1) {
        RFX_LOG_I(RFX_LOG_TAG, "[handleEnableANBR] response error");
    }
}
