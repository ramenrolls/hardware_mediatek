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

#include "RmcCallControlChldRequestHandler.h"
#include <telephony/mtk_ril.h>

#define RFX_LOG_TAG "RmcCCReqHandler"

// register handler to channel
RFX_IMPLEMENT_HANDLER_CLASS(RmcCallControlChldRequestHandler, RIL_CMD_PROXY_7);

RmcCallControlChldRequestHandler::RmcCallControlChldRequestHandler(int slot_id, int channel_id)
    : RmcCallControlBaseHandler(slot_id, channel_id) {
    const int requests[] = {
            RFX_MSG_REQUEST_HANGUP_ALL,                            // AT+CHLD=6
            RFX_MSG_REQUEST_HANGUP,                                // AT+CHLD=1X
            RFX_MSG_REQUEST_HANGUP_WAITING_OR_BACKGROUND,          // AT+CHLD=0
            RFX_MSG_REQUEST_HANGUP_FOREGROUND_RESUME_BACKGROUND,   // AT+CHLD=1
            RFX_MSG_REQUEST_SWITCH_WAITING_OR_HOLDING_AND_ACTIVE,  // AT+CHLD=2
            RFX_MSG_REQUEST_CONFERENCE,                            // AT+CHLD=3
            RFX_MSG_REQUEST_EXPLICIT_CALL_TRANSFER,                // AT+CHLD=4
            RFX_MSG_REQUEST_SEPARATE_CONNECTION,                   // AT+CHLD=2X
            RFX_MSG_REQUEST_HANGUP_WITH_REASON,                    // AT+ECHLD=1X, reason
    };

    registerToHandleRequest(requests, sizeof(requests) / sizeof(int));
}

RmcCallControlChldRequestHandler::~RmcCallControlChldRequestHandler() {}

void RmcCallControlChldRequestHandler::onHandleRequest(const sp<RfxMclMessage>& msg) {
    if (RfxRilUtils::isUserLoad() != 1) {
        logD(RFX_LOG_TAG, "onHandleRequest: %d", msg->getId());
    }
    int request = msg->getId();
    switch (request) {
        case RFX_MSG_REQUEST_HANGUP_ALL:
            requestHangupAll(msg);
            break;

        case RFX_MSG_REQUEST_HANGUP:
            requestHangup(msg);
            break;

        case RFX_MSG_REQUEST_HANGUP_WAITING_OR_BACKGROUND:
            if (getMclStatusManager()->getBoolValue(RFX_STATUS_KEY_HANGUP_FLIP, false)) {
                logD(RFX_LOG_TAG, "flip hangup background to foreground");
                requestHangupForegroundResumeBackground(msg);
                getMclStatusManager()->setBoolValue(RFX_STATUS_KEY_HANGUP_FLIP, false);
                break;
            }
            requestHangupWaitingOrBackground(msg);
            break;

        case RFX_MSG_REQUEST_HANGUP_FOREGROUND_RESUME_BACKGROUND:
            if (getMclStatusManager()->getBoolValue(RFX_STATUS_KEY_HANGUP_FLIP, false)) {
                logD(RFX_LOG_TAG, "flip hangup foreground to background");
                requestHangupWaitingOrBackground(msg);
                getMclStatusManager()->setBoolValue(RFX_STATUS_KEY_HANGUP_FLIP, false);
                break;
            }
            requestHangupForegroundResumeBackground(msg);
            break;

        case RFX_MSG_REQUEST_SWITCH_WAITING_OR_HOLDING_AND_ACTIVE:
            requestSwitchWaitingOrHoldingAndActive(msg);
            break;

        case RFX_MSG_REQUEST_CONFERENCE:
            requestConference(msg);
            break;

        case RFX_MSG_REQUEST_EXPLICIT_CALL_TRANSFER:
            requestExplicitCallTransfer(msg);
            break;

        case RFX_MSG_REQUEST_SEPARATE_CONNECTION:
            requestSeparateConnection(msg);
            break;

        case RFX_MSG_REQUEST_HANGUP_WITH_REASON:
            requestHangupWithReason(msg);
            break;

        default:
            logE(RFX_LOG_TAG, "Should not be here");
            break;
    }
}

void RmcCallControlChldRequestHandler::onHandleTimer() {
    // do something
}

void RmcCallControlChldRequestHandler::requestHangupAll(const sp<RfxMclMessage>& msg) {
    atSendCommand(String8::format("AT+CHLD=6"));

    sp<RfxMclMessage> response =
            RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS, RfxVoidData(), msg);
    responseToTelCore(response);
}

void RmcCallControlChldRequestHandler::requestHangup(const sp<RfxMclMessage>& msg) {
    int* pInt = (int*)msg->getData()->getData();
    sp<RfxAtResponse> p_response;
    RIL_Errno rilErrNo = RIL_E_SUCCESS;

    if (isCdmaRat() && !hasImsCall(m_slot_id)) {
        p_response = atSendCommand(String8::format("AT+CHLD=6"));
    } else {
        // 3GPP 22.030 6.5.5
        // "Releases a specific active call X"
        p_response = atSendCommand(String8::format("AT+CHLD=1%d", pInt[0]));
    }

    // set result
    if (p_response->getError() != 0 || p_response->getSuccess() != 1) {
        rilErrNo = RIL_E_INTERNAL_ERR;
    }

    sp<RfxMclMessage> response =
            RfxMclMessage::obtainResponse(msg->getId(), rilErrNo, RfxIntsData(pInt, 1), msg);
    responseToTelCore(response);
}

void RmcCallControlChldRequestHandler::requestHangupWaitingOrBackground(
        const sp<RfxMclMessage>& msg) {
    sp<RfxAtResponse> p_response;
    RIL_Errno rilErrNo = RIL_E_SUCCESS;

    // 3GPP 22.030 6.5.5
    // "Releases all held calls or sets User Determined User Busy
    //  (UDUB) for a waiting call."
    p_response = atSendCommand(String8::format("AT+CHLD=0"));

    // set result
    if (p_response->getError() != 0 || p_response->getSuccess() != 1) {
        rilErrNo = RIL_E_INTERNAL_ERR;
    }

    sp<RfxMclMessage> response =
            RfxMclMessage::obtainResponse(msg->getId(), rilErrNo, RfxVoidData(), msg);
    responseToTelCore(response);
}

void RmcCallControlChldRequestHandler::requestHangupForegroundResumeBackground(
        const sp<RfxMclMessage>& msg) {
    sp<RfxAtResponse> p_response;
    RIL_Errno rilErrNo = RIL_E_SUCCESS;

    // 3GPP 22.030 6.5.5
    // "Releases all active calls (if any exist) and accepts
    // the other (held or waiting) call."
    // atSendCommand(String8::format("AT+CHLD=1"));
    p_response = atSendCommandMultiline(String8::format("AT+CHLD=1"), "NO CARRIER");

    // set result
    if (p_response->getError() != 0 || p_response->getSuccess() != 1) {
        rilErrNo = RIL_E_INTERNAL_ERR;
    }

    sp<RfxMclMessage> response =
            RfxMclMessage::obtainResponse(msg->getId(), rilErrNo, RfxVoidData(), msg);
    responseToTelCore(response);
}

void RmcCallControlChldRequestHandler::requestSwitchWaitingOrHoldingAndActive(
        const sp<RfxMclMessage>& msg) {
    sp<RfxAtResponse> p_response;
    RIL_Errno rilErrNo = RIL_E_SUCCESS;

    p_response = atSendCommand(String8::format("AT+CHLD=2"));

    // set result
    if (p_response->getError() != 0 || p_response->getSuccess() != 1) {
        rilErrNo = RIL_E_INTERNAL_ERR;
    }

    sp<RfxMclMessage> response =
            RfxMclMessage::obtainResponse(msg->getId(), rilErrNo, RfxVoidData(), msg);
    responseToTelCore(response);
}

void RmcCallControlChldRequestHandler::requestConference(const sp<RfxMclMessage>& msg) {
    sp<RfxAtResponse> p_response;
    RIL_Errno rilErrNo = RIL_E_SUCCESS;

    p_response = atSendCommand(String8::format("AT+CHLD=3"));

    // set result
    if (p_response->getError() != 0 || p_response->getSuccess() != 1) {
        rilErrNo = RIL_E_INTERNAL_ERR;
    }

    sp<RfxMclMessage> response =
            RfxMclMessage::obtainResponse(msg->getId(), rilErrNo, RfxVoidData(), msg);
    responseToTelCore(response);
}

void RmcCallControlChldRequestHandler::requestExplicitCallTransfer(const sp<RfxMclMessage>& msg) {
    sp<RfxAtResponse> p_response;
    RIL_Errno rilErrNo = RIL_E_SUCCESS;

    p_response = atSendCommand(String8::format("AT+CHLD=4"));

    // set result
    if (p_response->getError() != 0 || p_response->getSuccess() != 1) {
        rilErrNo = RIL_E_INTERNAL_ERR;
    }

    sp<RfxMclMessage> response =
            RfxMclMessage::obtainResponse(msg->getId(), rilErrNo, RfxVoidData(), msg);
    responseToTelCore(response);
}

void RmcCallControlChldRequestHandler::requestSeparateConnection(const sp<RfxMclMessage>& msg) {
    int* pInt = (int*)msg->getData()->getData();
    sp<RfxAtResponse> p_response;
    RIL_Errno rilErrNo = RIL_E_SUCCESS;

    p_response = atSendCommand(String8::format("AT+CHLD=2%d", pInt[0]));

    // set result
    if (p_response->getError() != 0 || p_response->getSuccess() != 1) {
        rilErrNo = RIL_E_INTERNAL_ERR;
    }

    sp<RfxMclMessage> response =
            RfxMclMessage::obtainResponse(msg->getId(), rilErrNo, RfxVoidData(), msg);
    responseToTelCore(response);
}

void RmcCallControlChldRequestHandler::requestHangupWithReason(const sp<RfxMclMessage>& msg) {
    int* pInt = (int*)msg->getData()->getData();
    int hangupCallId = ((int*)msg->getData()->getData())[0];
    int hangupReason = ((int*)msg->getData()->getData())[1];
    sp<RfxAtResponse> p_response;
    RIL_Errno rilErrNo = RIL_E_SUCCESS;

    if (!hasImsCall(m_slot_id)) {
        if (hangupReason == INCOMING_REJECTED_NO_FORWARD) {
            // Map to disconnect cause to Network = 21 (REJECTED)
            p_response = atSendCommand(String8::format("AT+CHLD=8"));
        } else {
            // 3GPP 22.030 6.5.5
            // "Releases a specific active call X"
            p_response = atSendCommand(String8::format("AT+CHLD=1%d", hangupCallId));
        }
    } else {
        p_response = atSendCommand(String8::format("AT+ECHLD=1%d,%d", hangupCallId, hangupReason));
    }

    // set result
    if (p_response->getError() != 0 || p_response->getSuccess() != 1) {
        rilErrNo = RIL_E_INTERNAL_ERR;
    }

    sp<RfxMclMessage> response =
            RfxMclMessage::obtainResponse(msg->getId(), rilErrNo, RfxIntsData(pInt, 2), msg);
    responseToTelCore(response);
}
