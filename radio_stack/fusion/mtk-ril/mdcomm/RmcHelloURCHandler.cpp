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

#include "RmcHelloURCHandler.h"
#include <telephony/mtk_ril.h>

RFX_IMPLEMENT_HANDLER_CLASS(RmcHelloURCHandler, RIL_CMD_PROXY_5);

// register data
RFX_REGISTER_DATA_TO_EVENT_ID(RfxIntsData, RFX_MSG_EVENT_HELLO_NOTIFY_MODEM_STATE);

RmcHelloURCHandler::RmcHelloURCHandler(int slot_id, int channel_id)
    : RfxBaseHandler(slot_id, channel_id) {
    const char* urc[] = {//     "+EIND:128",
                         //     "+ECSRA",
                         "+EUSIM:"};

    registerToHandleURC(urc, sizeof(urc) / sizeof(char*));
}

RmcHelloURCHandler::~RmcHelloURCHandler() {}

void RmcHelloURCHandler::onHandleUrc(const sp<RfxMclMessage>& msg) {
    if (strStartsWith(msg->getRawUrc()->getLine(), "+EIND:128")) {
        handleModemState(msg);
    } else if (strStartsWith(msg->getRawUrc()->getLine(), "+EUSIM:")) {
        sp<RfxMclMessage> urc = RfxMclMessage::obtainUrc(RIL_UNSOL_TEST, m_slot_id, RfxVoidData());
        responseToTelCore(urc);
    }
}

void RmcHelloURCHandler::handleModemState(const sp<RfxMclMessage>& msg) {
    // do something

    int modemState[1];
    // NOTE: sendEvent with RfxBaseData
    modemState[0] = 1;
    sendEvent(RFX_MSG_EVENT_HELLO_NOTIFY_MODEM_STATE, RfxIntsData(modemState, 1), RIL_CMD_PROXY_1,
              RFX_SLOT_ID_0);

    sp<RfxMclMessage> urc = RfxMclMessage::obtainUrc(RIL_UNSOL_RESPONSE_RADIO_STATE_CHANGED,
                                                     m_slot_id, RfxVoidData());
    responseToTelCore(urc);
}

void RmcHelloURCHandler::onHandleTimer() {
    // do something
}

void RmcHelloURCHandler::onHandleEvent(const sp<RfxMclMessage>& msg) {
    // handle event
}
