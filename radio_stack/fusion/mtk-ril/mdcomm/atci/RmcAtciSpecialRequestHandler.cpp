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
#include "RmcAtciSpecialRequestHandler.h"

/*****************************************************************************
 * Class RmcAtciSpecialRequestHandler
 *****************************************************************************/
RFX_IMPLEMENT_HANDLER_CLASS(RmcAtciSpecialRequestHandler, RIL_CMD_PROXY_7);

RmcAtciSpecialRequestHandler::RmcAtciSpecialRequestHandler(int slot_id, int channel_id)
    : RmcAtciRequestHandler(slot_id, channel_id) {
    const int request[] = {RFX_MSG_REQUEST_ATCI_SPECIAL_COMMAND};
    registerToHandleRequest(request, sizeof(request) / sizeof(int));
}

void RmcAtciSpecialRequestHandler::onHandleRequest(const sp<RfxMclMessage>& msg) {
    int messageId = msg->getId();
    logD(LOG_TAG_ATCI, "[%s] message: %d %s", __FUNCTION__, messageId, idToString(messageId));
    switch (messageId) {
        case RFX_MSG_REQUEST_ATCI_SPECIAL_COMMAND:
            handleOemHookAtciInternalRequest(msg);
            break;

        default:
            logE(LOG_TAG_ATCI, "[%s] should not be here", __FUNCTION__);
            break;
    }
}
