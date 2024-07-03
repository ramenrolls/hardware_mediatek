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
#include "RmcOpSCBMRequestHandler.h"
#include <telephony/mtk_ril.h>
#include "RfxStatusDefs.h"
#include "RfxVoidData.h"

using ::android::String8;

static const int reqList[] = {RFX_MSG_REQUEST_EXIT_SCBM};

// Register handler
RFX_IMPLEMENT_HANDLER_CLASS(RmcOpSCBMRequestHandler, RIL_CMD_PROXY_1);

RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxVoidData, RFX_MSG_REQUEST_EXIT_SCBM);

/*****************************************************************************
 * Class RfxController
 *****************************************************************************/
RmcOpSCBMRequestHandler::RmcOpSCBMRequestHandler(int slot_id, int channel_id)
    : RfxBaseHandler(slot_id, channel_id) {
    setTag(String8("RmcOpSCBMRequest"));
    registerToHandleRequest(reqList, sizeof(reqList) / sizeof(int));
    logD(mTag, "RmcOpSCBMRequestHandler: constructor");
}

RmcOpSCBMRequestHandler::~RmcOpSCBMRequestHandler() {}

void RmcOpSCBMRequestHandler::onHandleRequest(const sp<RfxMclMessage>& msg) {
    int id = msg->getId();
    logD(mTag, "onHandleRequest: %s(%d)", idToString(id), id);
    switch (id) {
        case RFX_MSG_REQUEST_EXIT_SCBM:
            requestExitSCBM(msg);
            break;
        default:
            logE(mTag, "Should not be here");
            break;
    }
}

void RmcOpSCBMRequestHandler::requestExitSCBM(const sp<RfxMclMessage>& msg) {
    // When MD is off, respond success directly.
    if (getNonSlotMclStatusManager()->getBoolValue(RFX_STATUS_KEY_MODEM_POWER_OFF, false)) {
        logE(mTag, "%s MD off, just return success!", __FUNCTION__);
        sp<RfxMclMessage> mclResponse =
                RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS, RfxVoidData(), msg);
        responseToTelCore(mclResponse);
        return;
    }
    handleCmdWithVoidResponse(msg, String8("AT+SMEMEXIT=1"));
}
