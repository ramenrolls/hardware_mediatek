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
#include "RfxStringData.h"
#include "RmcAtciCommonRequestHandler.h"

/*****************************************************************************
 * Class RmcAtciCommonRequestHandler
 *****************************************************************************/
RFX_IMPLEMENT_HANDLER_CLASS(RmcAtciCommonRequestHandler, RIL_CMD_PROXY_6);

RmcAtciCommonRequestHandler::RmcAtciCommonRequestHandler(int slot_id, int channel_id)
    : RmcAtciRequestHandler(slot_id, channel_id) {
    const int request[] = {RFX_MSG_REQUEST_OEM_HOOK_ATCI_INTERNAL};
    const int event[] = {
            RFX_MSG_EVENT_RAW_URC,
    };
    const char* urc[] = {
            (char*)"+ERXRSSI",
            (char*)"+ERSSI",
    };
    registerToHandleRequest(request, sizeof(request) / sizeof(int));
    registerToHandleEvent(event, sizeof(event) / sizeof(int));
    registerToHandleURC(urc, sizeof(urc) / sizeof(char*));
}

void RmcAtciCommonRequestHandler::onHandleRequest(const sp<RfxMclMessage>& msg) {
    int messageId = msg->getId();
    logD(LOG_TAG_ATCI, "[%s] message: %d %s", __FUNCTION__, messageId, idToString(messageId));
    switch (messageId) {
        case RFX_MSG_REQUEST_OEM_HOOK_ATCI_INTERNAL:
            handleOemHookAtciInternalRequest(msg);
            break;

        default:
            logE(LOG_TAG_ATCI, "[%s] should not be here", __FUNCTION__);
            break;
    }
}

void RmcAtciCommonRequestHandler::onHandleEvent(const sp<RfxMclMessage>& msg) {
    int messageId = msg->getId();
    switch (messageId) {
        case RFX_MSG_EVENT_RAW_URC:
            handleMsgEventRawUrc(msg);
            break;

        default:
            logE(LOG_TAG_ATCI, "[%s] should not be here", __FUNCTION__);
            break;
    }
}
