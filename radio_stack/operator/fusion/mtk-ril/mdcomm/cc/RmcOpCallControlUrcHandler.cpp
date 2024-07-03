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

// MTK Fusion Fwk include
#include "RfxStringsData.h"
#include "RmcOpCallControlUrcHandler.h"

#define RFX_LOG_TAG "RmcOpCCUrcHandler"

RFX_REGISTER_DATA_TO_URC_ID(RfxStringsData, RFX_MSG_UNSOL_QUERY_TRN);

// register handler to channel
RFX_IMPLEMENT_OP_HANDLER_CLASS(RmcOpCallControlUrcHandler, RIL_CMD_PROXY_URC);

RmcOpCallControlUrcHandler::RmcOpCallControlUrcHandler(int slot_id, int channel_id)
    : RfxBaseHandler(slot_id, channel_id) {
    const char* urc[] = {
            "+QUERYTRN",
    };

    registerToHandleURC(urc, sizeof(urc) / sizeof(char*));
}

RmcOpCallControlUrcHandler::~RmcOpCallControlUrcHandler() {}

void RmcOpCallControlUrcHandler::onHandleUrc(const sp<RfxMclMessage>& msg) {
    char* urc = msg->getRawUrc()->getLine();
    // logD(RFX_LOG_TAG, "[onHandleUrc]%s", urc);
    if (strstr(urc, "+QUERYTRN") != NULL) {
        onQueryTrnIndication(msg);
    }
}

void RmcOpCallControlUrcHandler::onHandleTimer() {
    // do something
}

// For Modem to query TRN from AP during CSFB
void RmcOpCallControlUrcHandler::onQueryTrnIndication(const sp<RfxMclMessage>& msg) {
    /*
     * +QUERYTRN: <callId>,<toMsisdn>,<fromMsisdn>
     */
    int ret;
    int callId;
    char* number[2];
    RfxAtLine* line = msg->getRawUrc();

    line->atTokStart(&ret);
    if (ret < 0) {
        return;
    }

    callId = line->atTokNextint(&ret);
    if (ret < 0) {
        logE(RFX_LOG_TAG, "QUERYTRN: invalid callId");
        return;
    }
    getMclStatusManager()->setIntValue(RFX_STATUS_KEY_TRN_CALLID, callId);

    number[1] = line->atTokNextstr(&ret);
    if (ret < 0) {
        logE(RFX_LOG_TAG, "QUERYTRN: invalid to MSISDN");
        return;
    }

    number[0] = line->atTokNextstr(&ret);
    if (ret < 0) {
        logE(RFX_LOG_TAG, "QUERYTRN: invalid from MSISDN");
        return;
    }

    sp<RfxMclMessage> urc =
            RfxMclMessage::obtainUrc(RFX_MSG_UNSOL_QUERY_TRN, m_slot_id, RfxStringsData(number, 2));
    responseToTelCore(urc);
}
