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

#include "RmcOpNetworkUrcHandler.h"
#include "ViaBaseHandler.h"
#include "RfxViaUtils.h"

// register data
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_URC_MODULATION_INFO);

// register handler to channel
RFX_IMPLEMENT_OP_HANDLER_CLASS(RmcOpNetworkUrcHandler, RIL_CMD_PROXY_URC);

RmcOpNetworkUrcHandler::RmcOpNetworkUrcHandler(int slot_id, int channel_id)
    : RmcNetworkHandler(slot_id, channel_id) {
    logV(LOG_TAG, "[RmcOpNetworkUrcHandler] init");
}

RmcOpNetworkUrcHandler::~RmcOpNetworkUrcHandler() {}

void RmcOpNetworkUrcHandler::onHandleUrc(const sp<RfxMclMessage>& msg) {
    ViaBaseHandler* mViaHandler = RfxViaUtils::getViaHandler();
    if (strStartsWith(msg->getRawUrc()->getLine(), "+EMODCFG:")) {
        handleModulationInfoReceived(msg);
    } else if (mViaHandler != NULL) {
        mViaHandler->handleViaUrc(msg, this, m_slot_id);
    }
}

void RmcOpNetworkUrcHandler::handleModulationInfoReceived(const sp<RfxMclMessage>& msg) {
    /* +EMODCFG: <mode> */
    int err;
    int response = 0;
    sp<RfxMclMessage> urc;
    RfxAtLine* line = msg->getRawUrc();

    line->atTokStart(&err);
    if (err < 0) goto error;

    // get <mode>
    response = line->atTokNextint(&err);
    if (err < 0) goto error;

    if (response > 0xFF) goto error;

    logD(LOG_TAG, "handleModulationInfoReceived: <mode>:%d", response);
    urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_MODULATION_INFO, m_slot_id,
                                   RfxIntsData(&response, 1));
    responseToTelCore(urc);
    return;

error:
    logE(LOG_TAG, "There is something wrong with the URC +EMODCFG");
}
