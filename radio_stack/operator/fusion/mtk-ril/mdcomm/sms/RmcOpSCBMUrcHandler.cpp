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
#include "RmcOpSCBMUrcHandler.h"
#include <telephony/mtk_ril.h>
#include "RfxStatusDefs.h"
#include "RfxVoidData.h"
#include "RfxIntsData.h"

using ::android::String8;

static const char* urcList[] = {"+CIEV: 103"};

RFX_IMPLEMENT_HANDLER_CLASS(RmcOpSCBMUrcHandler, RIL_CMD_PROXY_URC);
RFX_REGISTER_DATA_TO_URC_ID(RfxVoidData, RFX_MSG_UNSOL_ENTER_SCBM);
RFX_REGISTER_DATA_TO_URC_ID(RfxVoidData, RFX_MSG_UNSOL_EXIT_SCBM);

/*****************************************************************************
 * Class RfxController
 *****************************************************************************/
RmcOpSCBMUrcHandler::RmcOpSCBMUrcHandler(int slot_id, int channel_id)
    : RfxBaseHandler(slot_id, channel_id) {
    setTag(String8("RmcOpSCBMUrc"));
    const char** p = urcList;
    registerToHandleURC(p, sizeof(urcList) / sizeof(char*));
}

RmcOpSCBMUrcHandler::~RmcOpSCBMUrcHandler() {}

void RmcOpSCBMUrcHandler::onHandleUrc(const sp<RfxMclMessage>& msg) {
    char* urc = msg->getRawUrc()->getLine();

    if (strstr(urc, "+CIEV: 103") != NULL) {
        handleSCBMStatusMessage(msg);
    }
}

void RmcOpSCBMUrcHandler::handleSCBMStatusMessage(const sp<RfxMclMessage>& msg) {
    int type = 0, value = 0, ret = 0, urcType = 0;
    RfxAtLine* line = msg->getRawUrc();

    line->atTokStart(&ret);
    if (ret < 0) {
        return;
    }

    type = line->atTokNextint(&ret);
    if (ret < 0) {
        return;
    }

    value = line->atTokNextint(&ret);
    if (ret < 0) {
        return;
    }

    /**
     * 103: SCBM Mode Indicator
     * value = 0: Mode off
     * value = 1: Mode on success
     * value = 2: Mode on fail
     * value = 3: Mode on restart
     */
    if (type == 103) {
        if (value == 1 || value == 3) {
            urcType = RFX_MSG_UNSOL_ENTER_SCBM;
        } else if (value == 0) {
            urcType = RFX_MSG_UNSOL_EXIT_SCBM;
        }
    }
    sp<RfxMclMessage> urc = RfxMclMessage::obtainUrc(urcType, m_slot_id, RfxVoidData());
    responseToTelCore(urc);
}
