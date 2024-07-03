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

#include "RmcSuppServRequestSpecialHandler.h"
#include "GsmUtil.h"
#include "SSUtil.h"
#include "RfxMessageId.h"

#include <telephony/mtk_ril.h>
#include <mtkconfigutils.h>

RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxIntsData, RFX_MSG_REQUEST_QUERY_CALL_WAITING);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxCallForwardInfoData, RfxCallForwardInfosData,
                                RFX_MSG_REQUEST_QUERY_CALL_FORWARD_STATUS);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxCallForwardInfoExData, RfxCallForwardInfosExData,
                                RFX_MSG_REQUEST_QUERY_CALL_FORWARD_IN_TIME_SLOT);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxIntsData, RFX_MSG_REQUEST_GET_CLIR);

RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringsData, RfxIntsData, RFX_MSG_REQUEST_QUERY_CALL_BARRING);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringsData, RfxIntsData, RFX_MSG_REQUEST_SET_CALL_BARRING);

static const int requests[] = {
        RFX_MSG_REQUEST_QUERY_CALL_FORWARD_STATUS, RFX_MSG_REQUEST_QUERY_CALL_FORWARD_IN_TIME_SLOT,
        RFX_MSG_REQUEST_QUERY_CALL_WAITING,        RFX_MSG_REQUEST_GET_CLIR,
        RFX_MSG_REQUEST_QUERY_CALL_BARRING,        RFX_MSG_REQUEST_SET_CALL_BARRING};

// register handler to channel
RFX_IMPLEMENT_HANDLER_CLASS(RmcSuppServRequestSpecialHandler, RIL_CMD_PROXY_6);

RmcSuppServRequestSpecialHandler::RmcSuppServRequestSpecialHandler(int slot_id, int channel_id)
    : RmcSuppServRequestBaseHandler(slot_id, channel_id) {
    registerToHandleRequest(requests, sizeof(requests) / sizeof(int));
}

RmcSuppServRequestSpecialHandler::~RmcSuppServRequestSpecialHandler() {}

void RmcSuppServRequestSpecialHandler::onHandleRequest(const sp<RfxMclMessage>& msg) {
    logD(TAG, "onHandleRequest: %d", msg->getId());
    int request = msg->getId();
    switch (request) {
        case RFX_MSG_REQUEST_QUERY_CALL_FORWARD_STATUS:
            requestQueryCallForwardStatus(msg);
            break;
        case RFX_MSG_REQUEST_QUERY_CALL_FORWARD_IN_TIME_SLOT:
            requestQueryCallForwardInTimeSlotStatus(msg);
            break;
        case RFX_MSG_REQUEST_GET_CLIR:
            requestGetClir(msg);
            break;
        case RFX_MSG_REQUEST_QUERY_CALL_WAITING:
            requestQueryCallWaiting(msg);
            break;
        case RFX_MSG_REQUEST_QUERY_CALL_BARRING:
            requestQueryCallBarring(msg);
            break;
        case RFX_MSG_REQUEST_SET_CALL_BARRING:
            requestSetCallBarring(msg);
            break;
        default:
            logE(TAG, "Should not be here");
            break;
    }
}

void RmcSuppServRequestSpecialHandler::onHandleTimer() {
    // do something
}

void RmcSuppServRequestSpecialHandler::requestQueryCallForwardStatus(const sp<RfxMclMessage>& msg) {
    requestCallForwardOperation(msg, CCFC_E_QUERY);
}

void RmcSuppServRequestSpecialHandler::requestQueryCallForwardInTimeSlotStatus(
        const sp<RfxMclMessage>& msg) {
    requestCallForwardExOperation(msg, CCFC_E_QUERY);
}

void RmcSuppServRequestSpecialHandler::requestGetClir(const sp<RfxMclMessage>& msg) {
    requestClirOperation(msg);
}

void RmcSuppServRequestSpecialHandler::requestQueryCallWaiting(const sp<RfxMclMessage>& msg) {
    requestCallWaitingOperation(msg, CW_E_QUERY);
}

void RmcSuppServRequestSpecialHandler::requestQueryCallBarring(const sp<RfxMclMessage>& msg) {
    requestCallBarring(msg, CB_E_QUERY);
}

void RmcSuppServRequestSpecialHandler::requestSetCallBarring(const sp<RfxMclMessage>& msg) {
    requestCallBarring(msg, CB_E_SET);
}
