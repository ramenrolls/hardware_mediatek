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
#include "RmcCommSmsUrcHandler.h"
#include <telephony/mtk_ril.h>
#include "RfxStatusDefs.h"
#include "RfxVoidData.h"

using ::android::String8;

static const char* commUrcList[] = {
        "+EIND: 1",
};

RFX_IMPLEMENT_HANDLER_CLASS(RmcCommSmsUrcHandler, RIL_CMD_PROXY_URC);

/*****************************************************************************
 * Class RfxController
 *****************************************************************************/
RmcCommSmsUrcHandler::RmcCommSmsUrcHandler(int slot_id, int channel_id)
    : RfxBaseHandler(slot_id, channel_id) {
    setTag(String8("RmcCommSmsUrc"));
    const char** p = commUrcList;
    registerToHandleURC(p, sizeof(commUrcList) / sizeof(char*));
}

void RmcCommSmsUrcHandler::onHandleUrc(const sp<RfxMclMessage>& msg) {
    char* ss = msg->getRawUrc()->getLine();

    if (strcmp(ss, "+EIND: 1") == 0) {
        handleSmsReady(msg);
    }
}

void RmcCommSmsUrcHandler::handleSmsReady(const sp<RfxMclMessage>& msg) {
    RFX_UNUSED(msg);
    sendEvent(RFX_MSG_EVENT_SMS_NOTIFY_SMS_READY, RfxVoidData(), RIL_CMD_PROXY_1, m_slot_id);
}
