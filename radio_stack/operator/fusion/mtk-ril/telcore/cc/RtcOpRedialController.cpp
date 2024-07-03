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
#include "RtcOpRedialController.h"
#include "nw/RtcRatSwitchController.h"
#include "rfx_properties.h"

/*****************************************************************************
 * Class RtcOpRedialController
 *****************************************************************************/
RFX_IMPLEMENT_CLASS("RtcOpRedialController", RtcOpRedialController, RtcRedialController);

#define RFX_LOG_TAG "RtcOpRedial"

RtcOpRedialController::RtcOpRedialController() {}

RtcOpRedialController::~RtcOpRedialController() { logD(RFX_LOG_TAG, "RtcOpRedialController"); }

void RtcOpRedialController::onInit() {
    RtcRedialController::onInit();
    logD(RFX_LOG_TAG, "onInit !!");
}

void RtcOpRedialController::onDeinit() {
    logD(RFX_LOG_TAG, "onDeinit");
    RtcRedialController::onDeinit();
}

bool RtcOpRedialController::isEmergencyModeSupported(int slotId) {
    if (RfxRilUtils::isEmciSupport()) {
        return true;
    }

    char optr[RFX_PROPERTY_VALUE_MAX] = {0};
    rfx_property_get("persist.vendor.operator.optr", optr, "0");
    // Vzw CDMA-less , AT&T supports ECM
    if (strcmp(optr, "OP12") == 0 || strcmp(optr, "OP07") == 0) {
        return true;
    }

    RtcModeSwitchController* modeSwitchController =
            (RtcModeSwitchController*)findController(RFX_OBJ_CLASS_INFO(RtcModeSwitchController));
    // non-TMO, 5M
    if (strcmp(optr, "OP08") != 0 && modeSwitchController->getCCapabilitySlotId() == slotId) {
        return true;
    }

    logD(RFX_LOG_TAG, "isEmergencyModeSupported = false ");
    return false;
}
