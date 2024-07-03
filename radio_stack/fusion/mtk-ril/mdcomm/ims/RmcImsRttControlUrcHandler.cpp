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

#include "RmcImsRttControlUrcHandler.h"
#include <telephony/mtk_ril.h>

#include "rfx_properties.h"

// register handler to channel
RFX_IMPLEMENT_HANDLER_CLASS(RmcImsRttControlUrcHandler, RIL_CMD_PROXY_URC);

// register data
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_UNSOL_RTT_MODIFY_RESPONSE);
RFX_REGISTER_DATA_TO_URC_ID(RfxStringsData, RFX_MSG_UNSOL_RTT_TEXT_RECEIVE);
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_UNSOL_RTT_CAPABILITY_INDICATION);
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_UNSOL_RTT_MODIFY_REQUEST_RECEIVE);
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_UNSOL_AUDIO_INDICATION);

RmcImsRttControlUrcHandler::RmcImsRttControlUrcHandler(int slot_id, int channel_id)
    : RfxBaseHandler(slot_id, channel_id) {
    logD(RFX_LOG_TAG, "RmcImsRttControlUrcHandler constructor");

    int m_slot_id = slot_id;
    const char* urc[] = {
            "+RTTCALL", "+ERTTSTR", "+EIMSTCAP", "+PRTTCALL", "+EIMSAUDIOSID",
    };

    registerToHandleURC(urc, sizeof(urc) / sizeof(char*));
}

RmcImsRttControlUrcHandler::~RmcImsRttControlUrcHandler() {}

void RmcImsRttControlUrcHandler::onHandleUrc(const sp<RfxMclMessage>& msg) {
    if (strStartsWith(msg->getRawUrc()->getLine(), "+RTTCALL")) {
        logD(RFX_LOG_TAG, "onHandleUrc: %s", msg->getRawUrc()->getLine());
        onRttModifyResponse(msg);
    } else if (strStartsWith(msg->getRawUrc()->getLine(), "+ERTTSTR")) {
        logD(RFX_LOG_TAG, "onHandleUrc: +ERTTSTR");
        onRttTextReceive(msg);
    } else if (strStartsWith(msg->getRawUrc()->getLine(), "+EIMSTCAP")) {
        logD(RFX_LOG_TAG, "onHandleUrc: %s", msg->getRawUrc()->getLine());
        onRttCapabilityIndication(msg);
    } else if (strStartsWith(msg->getRawUrc()->getLine(), "+PRTTCALL")) {
        logD(RFX_LOG_TAG, "onHandleUrc: %s", msg->getRawUrc()->getLine());
        onRttModifyRequestReceive(msg);
    } else if (strStartsWith(msg->getRawUrc()->getLine(), "+EIMSAUDIOSID")) {
        logD(RFX_LOG_TAG, "onHandleUrc: %s", msg->getRawUrc()->getLine());
        onAudioIndication(msg);
    }
}
void RmcImsRttControlUrcHandler::onRttModifyResponse(const sp<RfxMclMessage>& msg) {
    /*
     * +RTTCALL: <call id>,<result>
     * <call id>: call id
     * <result>: 0:success; 1:fail;
     */
    const int maxLen = 2;
    bool appendPhoneId = true;
    int rfxMsg = RFX_MSG_UNSOL_RTT_MODIFY_RESPONSE;

    notifyIntsDataToTcl(msg, rfxMsg, maxLen, appendPhoneId);
}

void RmcImsRttControlUrcHandler::onRttTextReceive(const sp<RfxMclMessage>& msg) {
    // +ERTTSTR= =<callid>, <len>, <text>
    const int maxLen = 3;
    bool appendPhoneId = true;
    int rfxMsg = RFX_MSG_UNSOL_RTT_TEXT_RECEIVE;

    notifyStringsDataToTcl(msg, rfxMsg, maxLen, appendPhoneId);
}

void RmcImsRttControlUrcHandler::onRttCapabilityIndication(const sp<RfxMclMessage>& msg) {
    // +EIMSTCAP:<call id>,<local text capability>,<remote text capability>
    // <local text status>,<real remote text capability>

    const int maxLen = 5;
    bool appendPhoneId = true;
    int rfxMsg = RFX_MSG_UNSOL_RTT_CAPABILITY_INDICATION;

    notifyIntsDataToTcl(msg, rfxMsg, maxLen, appendPhoneId);
}

void RmcImsRttControlUrcHandler::onRttModifyRequestReceive(const sp<RfxMclMessage>& msg) {
    //+PRTTCALL= <call id>, <op>
    const int maxLen = 2;
    bool appendPhoneId = true;
    int rfxMsg = RFX_MSG_UNSOL_RTT_MODIFY_REQUEST_RECEIVE;

    notifyIntsDataToTcl(msg, rfxMsg, maxLen, appendPhoneId);
}

void RmcImsRttControlUrcHandler::onAudioIndication(const sp<RfxMclMessage>& msg) {
    /*
     * +EIMSAUDIOSID:<call id>, <audio>
     * <call id>: call id
     * <audio>: 0:silence; 1:speech;
     */

    const int maxLen = 2;
    bool appendPhoneId = true;
    int rfxMsg = RFX_MSG_UNSOL_AUDIO_INDICATION;

    notifyIntsDataToTcl(msg, rfxMsg, maxLen, appendPhoneId);
}
