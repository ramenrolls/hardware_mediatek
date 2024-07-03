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

// Std include
#include <vector>
#include <string>

// MTK Fusion Fwk include
#include "RfxStringsData.h"
#include "RfxStringData.h"
#include "RfxIntsData.h"
#include "RfxVoidData.h"
#include "RfxMessageId.h"

// Ims Op Control Local include
#include "RmcOpImsControlInterface.h"
#include "RmcOpImsControlUrcHandler.h"

#include "cc/RmcOpCallControlCommonRequestHandler.h"

#define RFX_LOG_TAG "RmcOpImsControl"

// register handler to channel
RFX_IMPLEMENT_OP_HANDLER_CLASS(RmcOpImsControlUrcHandler, RIL_CMD_PROXY_URC);

RFX_REGISTER_DATA_TO_URC_ID(RfxStringsData, RFX_MSG_UNSOL_DIGITS_LINE_INDICATION);
RFX_REGISTER_DATA_TO_URC_ID(RfxStringData, RFX_MSG_UNSOL_RCS_DIGITS_LINE_INFO);

static const char* urc[] = {
        URC_DIGITS_LINE_INDICATION,
};

RmcOpImsControlUrcHandler::RmcOpImsControlUrcHandler(int slot_id, int channel_id)
    : RfxBaseHandler(slot_id, channel_id) {
    registerToHandleURC(urc, sizeof(urc) / sizeof(char*));
}

RmcOpImsControlUrcHandler::~RmcOpImsControlUrcHandler() {}

void RmcOpImsControlUrcHandler::onHandleUrc(const sp<RfxMclMessage>& msg) {
    char* urc = msg->getRawUrc()->getLine();
    logD(RFX_LOG_TAG, "[onHandleUrc]%s", urc);
    if (strStartsWith(urc, URC_DIGITS_LINE_INDICATION)) {
        onDigitsLineIndication(msg);
    }
}

void RmcOpImsControlUrcHandler::onHandleTimer() {
    // do something
}

void RmcOpImsControlUrcHandler::onDigitsLineIndication(const sp<RfxMclMessage>& msg) {
    /*
     * +DIGITLINE=<account_id>,<serial_number>,<msisdn_num>,
       "<msisdn>",<is_active>,"<msisdn>",<is_active>,"<msisdn>",<is_active>,"<msisdn>",<is_active>,"<msisdn>",<is_active>,
       "<msisdn>",<is_active>,"<msisdn>",<is_active>,"<msisdn>",<is_active>,"<msisdn>",<is_active>,"<msisdn>",<is_active>,
     * <account_id>:    0:SIM1=0 ; 2:SIM2=2 ; 4;SIM3
     * <serial_number>: A sequence number copy from AT command
     * <msisdn_num>: the number of msisdn
     * <msisdn>: msisdn
     * <Is_active>: 0: This line registers fail; 1: The line register succeesfully
     */

    const int maxLen = 23;
    bool appendPhoneId = true;
    int rfxMsg = RFX_MSG_UNSOL_DIGITS_LINE_INDICATION;
    notifyStringsDataToTcl(msg, rfxMsg, maxLen, appendPhoneId);
}

void RmcOpImsControlUrcHandler::onRcsDigitsLineInfo(const sp<RfxMclMessage>& msg) {
    const int maxLen = 2;  // TODO: wait confirm
    bool appendPhoneId = true;
    int rfxMsg = RFX_MSG_UNSOL_RCS_DIGITS_LINE_INFO;
    notifyIntsDataToTcl(msg, rfxMsg, maxLen, appendPhoneId);
}
