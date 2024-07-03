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
#include "RfxMessageId.h"
#include "RmcCommSmsRequestHandler.h"
#include <telephony/mtk_ril.h>
#include "RfxStatusDefs.h"
#include "RfxVoidData.h"

#define ENABLE_GEO_FENCING "ro.vendor.enable.geo.fencing"

using ::android::String8;

static const int event_list[] = {
        RFX_MSG_EVENT_SMS_NOTIFY_SMS_READY,
};

// Register handler
RFX_IMPLEMENT_HANDLER_CLASS(RmcCommSmsRequestHandler, RIL_CMD_PROXY_1);

// Register unsolicited message
RFX_REGISTER_DATA_TO_URC_ID(RfxVoidData, RFX_MSG_URC_SMS_READY_NOTIFICATION);

// Register event
RFX_REGISTER_DATA_TO_EVENT_ID(RfxVoidData, RFX_MSG_EVENT_SMS_NOTIFY_SMS_READY);

/*****************************************************************************
 * Class RfxController
 *****************************************************************************/
RmcCommSmsRequestHandler::RmcCommSmsRequestHandler(int slot_id, int channel_id)
    : RfxBaseHandler(slot_id, channel_id) {
    setTag(String8("RmcCommSmsReq"));
    registerToHandleEvent(event_list, sizeof(event_list) / sizeof(int));
}

void RmcCommSmsRequestHandler::onHandleEvent(const sp<RfxMclMessage>& msg) {
    int event = msg->getId();

    switch (event) {
        case RFX_MSG_EVENT_SMS_NOTIFY_SMS_READY:
            notifySmsReady(msg);
            break;
        default:
            logE(mTag, "Not support the event!");
            break;
    }
}

void RmcCommSmsRequestHandler::notifySmsReady(const sp<RfxMclMessage>& msg) {
    logD(mTag, "notifySmsReady");

    RFX_UNUSED(msg);
    /*
     * If modem turns off, RILD should not send any AT command to modem.
     * If RILD sends the at commands to Modem(off), it will block the
     * MUXD/CCCI channels
     */
    if (getNonSlotMclStatusManager()->getBoolValue(RFX_STATUS_KEY_MODEM_POWER_OFF, false)) {
        logD(mTag, "Modem turns off");
        return;
    }

    sp<RfxMclMessage> unsol =
            RfxMclMessage::obtainUrc(RFX_MSG_URC_SMS_READY_NOTIFICATION, m_slot_id, RfxVoidData());
    responseToTelCore(unsol);

    // To set PDU mode must be when SMS task is ready in modem.
    atSendCommand("AT+CMGF=0");
    /*
     * Always send SMS messages directly to the TE
     *
     * mode = 1 // discard when link is reserved (link should never be
     *             reserved)
     * mt = 2   // most messages routed to TE
     * bm = 2   // new cell BM's routed to TE
     * ds = 1   // Status reports routed to TE
     * bfr = 1  // flush buffer
     */
    atSendCommandSingleline("AT+CSMS=1", "+CSMS:");

    atSendCommand("AT+CNMI=1,2,2,1,1");

    atSendCommand("AT+CPMS=\"SM\", \"SM\", \"SM\"");
    atSendCommand("AT+EC2KCPMS=\"SM\", \"SM\", \"SM\"");

    // Enable WEA 3.0.
    char value[RFX_PROPERTY_VALUE_MAX] = {0};
    int ret = 0;
    rfx_property_get(ENABLE_GEO_FENCING, value, "0");
    ret = atoi(value);
    if (ret == 1) {
        atSendCommand("AT+EWAC=1");
    }
}
