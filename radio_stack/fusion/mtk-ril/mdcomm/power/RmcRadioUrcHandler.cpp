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

#include "RmcRadioUrcHandler.h"
#include "RfxIntsData.h"

#define RFX_LOG_TAG "RmcRadUrc"

// register handler to channel
RFX_IMPLEMENT_HANDLER_CLASS(RmcRadioUrcHandler, RIL_CMD_PROXY_URC);

RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_UNSOL_DSBP_CHANGED_INDICATION);

RmcRadioUrcHandler::RmcRadioUrcHandler(int slot_id, int channel_id)
    : RfxBaseHandler(slot_id, channel_id) {
    const char* urc[] = {
            (char*)"+EDSBP",
    };

    registerToHandleURC(urc, sizeof(urc) / sizeof(char*));
    mPreRadioState = RADIO_STATE_OFF;
}

RmcRadioUrcHandler::~RmcRadioUrcHandler() {}

void RmcRadioUrcHandler::onHandleUrc(const sp<RfxMclMessage>& msg) {
    int err = 0;
    RfxAtLine* line = msg->getRawUrc();
    char* data = line->getLine();
    if (strStartsWith(data, "+EDSBP")) {
        handleDSBPEnhancement(line);
    } else {
        RFX_LOG_E(RFX_LOG_TAG, "we can not handle this raw urc?! %s", data);
    }
}

void RmcRadioUrcHandler::handleDSBPEnhancement(RfxAtLine* line) {
    int err = 0;
    int sbp, state;
    line->atTokStart(&err);

    // SBP value
    sbp = line->atTokNextint(&err);
    if (err < 0) {
        RFX_LOG_E(RFX_LOG_TAG, "parser +EDSBP error!");
        return;
    }
    // SBP state
    state = line->atTokNextint(&err);
    if (err < 0) {
        RFX_LOG_E(RFX_LOG_TAG, "parser +EDSBP error!");
        return;
    }
    logI(RFX_LOG_TAG, "handleDSBPEnhancement: SBP: %d, state: %d", sbp, state);

    if (DSBP_ENHANCEMENT_START == state) {
        getMclStatusManager()->setIntValue(RFX_STATUS_KEY_DSBP_ENHANCEMENT_STATE,
                                           DSBP_ENHANCEMENT_START);
    } else {
        getMclStatusManager()->setIntValue(RFX_STATUS_KEY_DSBP_ENHANCEMENT_STATE,
                                           DSBP_ENHANCEMENT_END);
    }
    sp<RfxMclMessage> urc = RfxMclMessage::obtainUrc(RFX_MSG_UNSOL_DSBP_CHANGED_INDICATION,
                                                     m_slot_id, RfxIntsData(&state, 1));
    responseToTelCore(urc);
}
