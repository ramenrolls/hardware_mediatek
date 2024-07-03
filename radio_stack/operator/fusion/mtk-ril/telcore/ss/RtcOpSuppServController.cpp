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
#include "RtcOpSuppServController.h"
#include "RfxRilUtils.h"

#define RFX_LOG_TAG "RtcOpSuppServController"
/*****************************************************************************
 * Class RfxController
 *****************************************************************************/

RFX_IMPLEMENT_CLASS("RtcOpSuppServController", RtcOpSuppServController, RfxController);

RtcOpSuppServController::RtcOpSuppServController() {}

RtcOpSuppServController::~RtcOpSuppServController() {}

void RtcOpSuppServController::onInit() {
    // Required: invoke super class implementation
    RfxController::onInit();

    const int request_id_list[] = {
            RFX_MSG_REQUEST_SEND_USSI_FROM,
            RFX_MSG_REQUEST_CANCEL_USSI_FROM,
    };

    const int urc_id_list[] = {};

    // register request & URC id list
    // NOTE. one id can only be registered by one controller
    registerToHandleRequest(request_id_list, sizeof(request_id_list) / sizeof(const int));
    registerToHandleUrc(urc_id_list, sizeof(request_id_list) / sizeof(const int));
}

bool RtcOpSuppServController::onHandleRequest(const sp<RfxMessage>& message) {
    requestToMcl(message);
    return true;
}

bool RtcOpSuppServController::onHandleUrc(const sp<RfxMessage>& message) {
    return responseToRilj(message);
}

bool RtcOpSuppServController::onHandleResponse(const sp<RfxMessage>& message) {
    return responseToRilj(message);
}

bool RtcOpSuppServController::onCheckIfRejectMessage(const sp<RfxMessage>& message,
                                                     bool isModemPowerOff, int radioState) {
    RFX_UNUSED(message);
    logD(RFX_LOG_TAG, "onCheckIfRejectMessage isModemPowerOff %d, radioState: %d",
         (isModemPowerOff == false) ? 0 : 1, radioState);

    /* If WFC is enabled, bypass all SS requests. */
    if (RfxRilUtils::isWfcEnable(message->getSlotId())) {
        return false;
    }

    /* If WFC is not enabled and radio is off. reject the request in request_id_list*/
    if (radioState == (int)RADIO_STATE_UNAVAILABLE || radioState == (int)RADIO_STATE_OFF ||
        isModemPowerOff == true) {
        return true;
    }

    /* If WFC is not enabled and radio is not off. Bypass all SS request */
    return false;
}
