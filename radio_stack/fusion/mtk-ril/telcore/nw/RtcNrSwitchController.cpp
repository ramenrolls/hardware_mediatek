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
#include "RtcNrSwitchController.h"

/*****************************************************************************
 * Class RfxController
 *****************************************************************************/

#define NRSW_CTRL_TAG "RtcNrSwCtrl"

RFX_IMPLEMENT_CLASS("RtcNrSwitchController", RtcNrSwitchController, RfxController);

RtcNrSwitchController::RtcNrSwitchController() : mNrSlot(0), mPreferredDataSlot(0) {}

RtcNrSwitchController::~RtcNrSwitchController() {}

void RtcNrSwitchController::onInit() {
    // Required: invoke super class implementation
    RfxController::onInit();
    logV(NRSW_CTRL_TAG, "[onInit]");
    char feature[] = "GEMINI_NR_PLUS_SUPPORTED";
    char NR_VER[] = "5G_SUPPORTED";

    const int request_id_list[] = {RFX_MSG_REQUEST_NR_SWITCH};

    if (getFeatureVersion(NR_VER, 0) == 1 && getFeatureVersion(feature, 0) == 0) {
        registerToHandleRequest(request_id_list, sizeof(request_id_list) / sizeof(const int),
                                DEFAULT);
        char stgBuf[RFX_PROPERTY_VALUE_MAX] = {0};
        int val = 0;
        rfx_property_get("persist.vendor.radio.nrslot", stgBuf, "0");
        val = strtol(stgBuf, NULL, 10);
        if (val == 0) {
            rfx_property_set("persist.vendor.radio.nrslot", String8::format("%d", 1).string());
        }
        getStatusManager()->registerStatusChanged(
                RFX_STATUS_KEY_PREFERRED_DATA_SIM,
                RfxStatusChangeCallback(this, &RtcNrSwitchController::onPreferredDataChanged));

        for (int slotId = RFX_SLOT_ID_0; slotId < RFX_SLOT_COUNT; slotId++) {
            getStatusManager(slotId)->registerStatusChangedEx(
                    RFX_STATUS_KEY_AP_VOICE_CALL_COUNT,
                    RfxStatusChangeCallbackEx(this,
                                              &RtcNrSwitchController::onApVoiceCallCountChanged));
        }
    }
}

bool RtcNrSwitchController::onHandleResponse(const sp<RfxMessage>& response) {
    int msg_id = response->getId();

    switch (msg_id) {
        case RFX_MSG_REQUEST_NR_SWITCH:
            responseNrSwitch(response);
            return true;
        default:
            break;
    }
    return true;
}

void RtcNrSwitchController::responseNrSwitch(const sp<RfxMessage>& response) {
    logD(NRSW_CTRL_TAG, "[responseNrSwitch] error: %d ", response->getError());
    if (response->getError() == RIL_E_SUCCESS) {
        mNrSlot = getNonSlotScopeStatusManager()->getIntValue(RFX_STATUS_KEY_PREFERRED_DATA_SIM);
    }
}

int RtcNrSwitchController::getNrSlot() { return mNrSlot; }

void RtcNrSwitchController::onPreferredDataChanged(RfxStatusKeyEnum key, RfxVariant old_value,
                                                   RfxVariant value) {
    RFX_UNUSED(key);
    int old_mode = old_value.asInt();
    int new_mode = value.asInt();

    if (new_mode != -1 && old_mode != new_mode) {
        int mainSlot =
                getNonSlotScopeStatusManager()->getIntValue(RFX_STATUS_KEY_MAIN_CAPABILITY_SLOT, 0);
        sp<RfxMessage> reqToRild = RfxMessage::obtainRequest(mainSlot, RFX_MSG_REQUEST_NR_SWITCH,
                                                             RfxIntsData(&new_mode, 1));
        requestToMcl(reqToRild);
    }
    mPreferredDataSlot = new_mode;
}

void RtcNrSwitchController::onApVoiceCallCountChanged(int slotId, RfxStatusKeyEnum key,
                                                      RfxVariant old_value, RfxVariant value) {
    int oldMode = old_value.asInt();
    int mode = value.asInt();

    logV(NRSW_CTRL_TAG, "%s, slotId:%d, key:%d oldMode:%d, mode:%d", __FUNCTION__, slotId, key,
         oldMode, mode);
    if (mode == 0 && oldMode > 0) {
        logV(NRSW_CTRL_TAG, "mNrSlot:%d, mPreferredDataSlot:%d", mNrSlot, mPreferredDataSlot);
        if (mNrSlot != mPreferredDataSlot) {
            int mainSlot = getNonSlotScopeStatusManager()->getIntValue(
                    RFX_STATUS_KEY_MAIN_CAPABILITY_SLOT, 0);
            sp<RfxMessage> reqToRild = RfxMessage::obtainRequest(
                    mainSlot, RFX_MSG_REQUEST_NR_SWITCH, RfxIntsData(&mPreferredDataSlot, 1));
            requestToMcl(reqToRild);
        }
    }
}
