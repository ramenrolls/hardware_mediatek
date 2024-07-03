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
#include "RfxController.h"
#include "RtcCommSimOpController.h"

#include "RfxMessageId.h"

/*****************************************************************************
 * Class RtcCommSimOpController
 *****************************************************************************/

#define RFX_LOG_TAG "RtcCommSimOp"

RFX_IMPLEMENT_CLASS("RtcCommSimOpController", RtcCommSimOpController, RtcCommSimController);

RtcCommSimOpController::RtcCommSimOpController() {}

RtcCommSimOpController::~RtcCommSimOpController() {}

void RtcCommSimOpController::onInit() {
    RtcCommSimController::onInit();
    const int request_id_list[] = {
            RFX_MSG_REQUEST_SEND_RSU_REQUEST,         RFX_MSG_REQUEST_GET_SHARED_KEY,
            RFX_MSG_REQUEST_UPDATE_SIM_LOCK_SETTINGS, RFX_MSG_REQUEST_GET_SIM_LOCK_INFO,
            RFX_MSG_REQUEST_RESET_SIM_LOCK_SETTINGS,  RFX_MSG_REQUEST_GET_MODEM_STATUS,
            RFX_MSG_REQUEST_ATT_GET_SHARED_KEY,       RFX_MSG_REQUEST_ATT_UPDATE_SIM_LOCK_SETTINGS,
            RFX_MSG_REQUEST_ATT_GET_SIM_LOCK_VERSION, RFX_MSG_REQUEST_ATT_RESET_SIM_LOCK_SETTINGS,
            RFX_MSG_REQUEST_ATT_GET_MODEM_STATUS,     RFX_MSG_REQUEST_ATT_GET_MODEM_CONFIG,
            RFX_MSG_REQUEST_ATT_GET_API_VERSION,      RFX_MSG_REQUEST_ATT_GET_SIM_SLOTS,
    };

    registerToHandleRequest(request_id_list, sizeof(request_id_list) / sizeof(const int), DEFAULT);
}

void RtcCommSimOpController::onDeinit() { RfxController::onDeinit(); }

bool RtcCommSimOpController::onHandleRequest(const sp<RfxMessage>& message) {
    int msg_id = message->getId();
    switch (msg_id) {
        case RFX_MSG_REQUEST_SEND_RSU_REQUEST:
        case RFX_MSG_REQUEST_GET_SHARED_KEY:
        case RFX_MSG_REQUEST_UPDATE_SIM_LOCK_SETTINGS:
        case RFX_MSG_REQUEST_GET_SIM_LOCK_INFO:
        case RFX_MSG_REQUEST_RESET_SIM_LOCK_SETTINGS:
        case RFX_MSG_REQUEST_GET_MODEM_STATUS:
        case RFX_MSG_REQUEST_ATT_GET_SHARED_KEY:
        case RFX_MSG_REQUEST_ATT_UPDATE_SIM_LOCK_SETTINGS:
        case RFX_MSG_REQUEST_ATT_GET_SIM_LOCK_VERSION:
        case RFX_MSG_REQUEST_ATT_RESET_SIM_LOCK_SETTINGS:
        case RFX_MSG_REQUEST_ATT_GET_MODEM_STATUS:
        case RFX_MSG_REQUEST_ATT_GET_MODEM_CONFIG:
        case RFX_MSG_REQUEST_ATT_GET_API_VERSION:
        case RFX_MSG_REQUEST_ATT_GET_SIM_SLOTS:
            requestToMcl(message);
            return true;
        default:
            break;
    }
    return RtcCommSimController::onHandleRequest(message);
}

bool RtcCommSimOpController::onHandleResponse(const sp<RfxMessage>& message) {
    int msg_id = message->getId();
    switch (msg_id) {
        case RFX_MSG_REQUEST_SEND_RSU_REQUEST:
        case RFX_MSG_REQUEST_GET_SHARED_KEY:
        case RFX_MSG_REQUEST_UPDATE_SIM_LOCK_SETTINGS:
        case RFX_MSG_REQUEST_GET_SIM_LOCK_INFO:
        case RFX_MSG_REQUEST_RESET_SIM_LOCK_SETTINGS:
        case RFX_MSG_REQUEST_GET_MODEM_STATUS:
        case RFX_MSG_REQUEST_ATT_GET_SHARED_KEY:
        case RFX_MSG_REQUEST_ATT_UPDATE_SIM_LOCK_SETTINGS:
        case RFX_MSG_REQUEST_ATT_GET_SIM_LOCK_VERSION:
        case RFX_MSG_REQUEST_ATT_RESET_SIM_LOCK_SETTINGS:
        case RFX_MSG_REQUEST_ATT_GET_MODEM_STATUS:
        case RFX_MSG_REQUEST_ATT_GET_MODEM_CONFIG:
        case RFX_MSG_REQUEST_ATT_GET_API_VERSION:
        case RFX_MSG_REQUEST_ATT_GET_SIM_SLOTS:
            responseToRilj(message);
            return true;
        default:
            break;
    }
    return RtcCommSimController::onHandleResponse(message);
}

bool RtcCommSimOpController::onCheckIfRejectMessage(const sp<RfxMessage>& message,
                                                    bool isModemPowerOff, int radioState) {
    int msgId = message->getId();
    if (!isModemPowerOff && (radioState == (int)RADIO_STATE_OFF) &&
        (msgId == RFX_MSG_REQUEST_SEND_RSU_REQUEST || msgId == RFX_MSG_REQUEST_GET_SHARED_KEY ||
         msgId == RFX_MSG_REQUEST_UPDATE_SIM_LOCK_SETTINGS ||
         msgId == RFX_MSG_REQUEST_GET_SIM_LOCK_INFO ||
         msgId == RFX_MSG_REQUEST_RESET_SIM_LOCK_SETTINGS ||
         msgId == RFX_MSG_REQUEST_GET_MODEM_STATUS || msgId == RFX_MSG_REQUEST_ATT_GET_SHARED_KEY ||
         msgId == RFX_MSG_REQUEST_ATT_UPDATE_SIM_LOCK_SETTINGS ||
         msgId == RFX_MSG_REQUEST_ATT_GET_SIM_LOCK_VERSION ||
         msgId == RFX_MSG_REQUEST_ATT_RESET_SIM_LOCK_SETTINGS ||
         msgId == RFX_MSG_REQUEST_ATT_GET_MODEM_STATUS ||
         msgId == RFX_MSG_REQUEST_ATT_GET_MODEM_CONFIG ||
         msgId == RFX_MSG_REQUEST_ATT_GET_API_VERSION ||
         msgId == RFX_MSG_REQUEST_ATT_GET_SIM_SLOTS)) {
        return false;
    }
    return RtcCommSimController::onCheckIfRejectMessage(message, isModemPowerOff, radioState);
}
