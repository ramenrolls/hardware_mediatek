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
#include "RtcImsRttController.h"
#include "RfxStatusDefs.h"
#include "rfx_properties.h"

/*****************************************************************************
 * Class RfxController
 *****************************************************************************/
#define RFX_LOG_TAG "RtcImsRtt"

RFX_IMPLEMENT_CLASS("RtcImsRttController", RtcImsRttController, RfxController);

RtcImsRttController::RtcImsRttController() {}

RtcImsRttController::~RtcImsRttController() {}

void RtcImsRttController::onInit() {
    // Required: invoke super class implementation
    RfxController::onInit();
    logD(RFX_LOG_TAG, "onInit()");

    const int request_id_list[] = {
            RFX_MSG_REQUEST_SEND_RTT_MODIFY_REQUEST,
            RFX_MSG_REQUEST_SEND_RTT_TEXT,
            RFX_MSG_REQUEST_RTT_MODIFY_REQUEST_RESPONSE,
            RFX_MSG_REQUEST_TOGGLE_RTT_AUDIO_INDICATION,
    };

    const int urc_id_list[] = {
            RFX_MSG_UNSOL_RTT_CAPABILITY_INDICATION, RFX_MSG_UNSOL_RTT_MODIFY_RESPONSE,
            RFX_MSG_UNSOL_RTT_TEXT_RECEIVE,          RFX_MSG_UNSOL_RTT_MODIFY_REQUEST_RECEIVE,
            RFX_MSG_UNSOL_AUDIO_INDICATION,
    };

    // register request & URC id list
    // NOTE. one id can only be registered by one controller
    registerToHandleRequest(request_id_list, sizeof(request_id_list) / sizeof(const int));
    registerToHandleUrc(urc_id_list, sizeof(urc_id_list) / sizeof(const int));
}

void RtcImsRttController::onDeinit() {
    logD(RFX_LOG_TAG, "onDeinit");
    RfxController::onDeinit();
}

bool RtcImsRttController::onHandleRequest(const sp<RfxMessage>& message) {
    requestToMcl(message);
    return true;
}

bool RtcImsRttController::onHandleUrc(const sp<RfxMessage>& message) {
    responseToRilj(message);
    return true;
}

bool RtcImsRttController::onHandleResponse(const sp<RfxMessage>& message) {
    responseToRilj(message);
    return true;
}

bool RtcImsRttController::responseToRilj(const sp<RfxMessage>& msg) {
    return RfxController::responseToRilj(msg);
}

bool RtcImsRttController::onCheckIfRejectMessage(const sp<RfxMessage>& message,
                                                 bool isModemPowerOff, int radioState) {
    if (isModemPowerOff == false) {
        return false;
    }
    return RfxController::onCheckIfRejectMessage(message, isModemPowerOff, radioState);
}
