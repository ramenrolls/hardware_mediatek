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
#include "RfxStringData.h"
#include "RfxMessageId.h"
#include "RmcAtciRequestHandler.h"

/*****************************************************************************
 * Class RmcAtciRequestHandler
 *****************************************************************************/
RFX_IMPLEMENT_HANDLER_CLASS(RmcAtciRequestHandler, RIL_CMD_PROXY_6);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxStringData, RFX_MSG_EVENT_RAW_URC);

const char* RmcAtciRequestHandler::ERROR_RESPONSE = "\r\nERROR\r\n";
const char* RmcAtciRequestHandler::ENABLE_URC_PROP = "persist.vendor.service.atci_urc.enable";
const char* RmcAtciRequestHandler::LOG_TAG_ATCI = "RmcAtciRequestHandler";

RmcAtciRequestHandler::RmcAtciRequestHandler(int slot_id, int channel_id)
    : RfxBaseHandler(slot_id, channel_id) {}

void RmcAtciRequestHandler::handleOemHookAtciInternalRequest(const sp<RfxMclMessage>& msg) {
    const char* data = (const char*)msg->getData()->getData();
    logD(LOG_TAG_ATCI, "[%s] enter", __FUNCTION__);

    if (strncasecmp(data, "ATV0", 4) == 0) {
        logD(LOG_TAG_ATCI, "[%s] set isNumericSet on", __FUNCTION__);
        getMclStatusManager()->setBoolValue(RFX_STATUS_KEY_ATCI_IS_NUMERIC, true);
    } else if (strncasecmp(data, "ATV1", 4) == 0) {
        logD(LOG_TAG_ATCI, "[%s] set isNumericSet off", __FUNCTION__);
        getMclStatusManager()->setBoolValue(RFX_STATUS_KEY_ATCI_IS_NUMERIC, false);
    }

    sp<RfxAtResponse> atResponse = atSendCommandRaw(data);
    if (atResponse->getError() < 0) {
        logE(LOG_TAG_ATCI, "[%s] atSendCommandRaw fail: %d", __FUNCTION__, atResponse->getError());
        sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(
                RIL_E_GENERIC_FAILURE, RfxStringData((char*)ERROR_RESPONSE), msg);
        responseToTelCore(response);
        return;
    }

    String8 ret = String8("\r\n");
    for (RfxAtLine* line = atResponse->getIntermediates(); line != NULL; line = line->getNext()) {
        ret.appendFormat("%s\r\n", line->getLine());
    }
    ret.appendFormat("%s\r\n", atResponse->getFinalResponse()->getLine());

    sp<RfxMclMessage> response =
            RfxMclMessage::obtainResponse(RIL_E_SUCCESS, RfxStringData((char*)ret.string()), msg);
    responseToTelCore(response);
}

void RmcAtciRequestHandler::handleMsgEventRawUrc(const sp<RfxMclMessage>& msg) {
    char enabled[RFX_PROPERTY_VALUE_MAX] = {0};
    rfx_property_get(ENABLE_URC_PROP, enabled, "0");
    const char* data = (const char*)msg->getData()->getData();
    if (NULL != data && atoi(enabled) == 1) {
        sp<RfxMclMessage> urc = RfxMclMessage::obtainUrc(
                RFX_MSG_UNSOL_ATCI_RESPONSE, m_slot_id,
                RfxStringData((char*)String8::format("\r\n%s\r\n", data).string()));
        responseToTelCore(urc);
    }
}
