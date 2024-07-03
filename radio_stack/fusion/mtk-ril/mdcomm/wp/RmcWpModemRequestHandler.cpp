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

#include <telephony/mtk_ril.h>
#include "RfxLog.h"
#include "RfxRilUtils.h"
#include "RmcWpModemRequestHandler.h"

#define WP_LOG_TAG "RmcWp"
#define WP_RELOAD_MODEM_TYPE (1)
#define WP_STORE_MODEM_TYPE (2)

// register handler to channel
RFX_IMPLEMENT_HANDLER_CLASS(RmcWpModemRequestHandler, RIL_CMD_PROXY_9);

RmcWpModemRequestHandler::RmcWpModemRequestHandler(int slot_id, int channel_id)
    : RfxBaseHandler(slot_id, channel_id) {
    const int request[] = {
            RFX_MSG_REQUEST_WORLD_MODE_RELOAD_MODEM_TYPE,
            RFX_MSG_REQUEST_WORLD_MODE_STORE_MODEM_TYPE,
            RFX_MSG_REQUEST_WORLD_MODE_STORE_ID,
            RFX_MSG_REQUEST_WORLD_MODE_RELOAD_ID,
            RFX_MSG_REQUEST_WORLD_MODE_MODIFY_MODEM_TYPE,
    };

    registerToHandleRequest(request, sizeof(request) / sizeof(int));
}

RmcWpModemRequestHandler::~RmcWpModemRequestHandler() {}

void RmcWpModemRequestHandler::onHandleRequest(const sp<RfxMclMessage>& msg) {
    logD(WP_LOG_TAG, "onHandleRequest: %d", msg->getId());
    int request = msg->getId();
    switch (request) {
        case RFX_MSG_REQUEST_WORLD_MODE_RELOAD_ID:
        case RFX_MSG_REQUEST_WORLD_MODE_RELOAD_MODEM_TYPE:
            requestReloadModem(msg);
            break;
        case RFX_MSG_REQUEST_WORLD_MODE_STORE_ID:
        case RFX_MSG_REQUEST_WORLD_MODE_STORE_MODEM_TYPE:
            requestStoreModem(msg);
            break;
        case RFX_MSG_REQUEST_WORLD_MODE_MODIFY_MODEM_TYPE:
            requestModifyModem(msg);
            break;
        default:
            assert(0);
            break;
    }
}

void RmcWpModemRequestHandler::requestModifyModem(const sp<RfxMclMessage>& msg) {
    int* pInt = (int*)msg->getData()->getData();
    int responseInfo[1] = {0};
    int applyType = pInt[0];
    int modemType = pInt[1];
    responseInfo[0] = applyType;
    if (applyType == WP_RELOAD_MODEM_TYPE) {
        RfxRilUtils::triggerCCCIIoctlEx(CCCI_IOC_RELOAD_MD_TYPE, &modemType);
    } else if (applyType == WP_STORE_MODEM_TYPE) {
        RfxRilUtils::triggerCCCIIoctlEx(CCCI_IOC_STORE_MD_TYPE, &modemType);
    }
    logD(WP_LOG_TAG, "requestStoreModem complete:%d, %d", applyType, modemType);
    sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(
            msg->getId(), RIL_E_SUCCESS, RfxIntsData(responseInfo, 1), msg, false);
    // response to TeleCore
    responseToTelCore(response);
    return;
}

void RmcWpModemRequestHandler::requestStoreModem(const sp<RfxMclMessage>& msg) {
    int* pInt = (int*)msg->getData()->getData();
    int modemType = pInt[0] & 0x00FF;

    RfxRilUtils::triggerCCCIIoctlEx(CCCI_IOC_STORE_MD_TYPE, &modemType);
    logD(WP_LOG_TAG, "requestStoreModem complete MDtype = %d", modemType);
    sp<RfxMclMessage> response =
            RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS, RfxVoidData(), msg, false);
    // response to TeleCore
    responseToTelCore(response);

    return;
}

void RmcWpModemRequestHandler::requestReloadModem(const sp<RfxMclMessage>& msg) {
    int* pInt = (int*)msg->getData()->getData();
    int modemType = pInt[0];

    RfxRilUtils::triggerCCCIIoctlEx(CCCI_IOC_RELOAD_MD_TYPE, &modemType);
    logD(WP_LOG_TAG, "requestReloadModem complete MDtype = %d", modemType);
    sp<RfxMclMessage> response =
            RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS, RfxVoidData(), msg, false);
    // response to TeleCore
    responseToTelCore(response);

    return;
}
