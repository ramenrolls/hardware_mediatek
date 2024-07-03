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
#include "RtcCapabilityGetController.h"
#include "RtcCapabilitySwitchController.h"
#include "RtcCapabilitySwitchUtil.h"
#include "RfxRadioCapabilityData.h"
#include "rfx_properties.h"
#include "RfxRilUtils.h"
/*****************************************************************************
 * Class RtcCapabilityGetController
 *****************************************************************************/
#define RFX_LOG_TAG "RtcCapa"

RFX_IMPLEMENT_CLASS("RtcCapabilityGetController", RtcCapabilityGetController, RfxController);

RFX_REGISTER_DATA_TO_URC_ID(RfxRadioCapabilityData, RFX_MSG_URC_RADIO_CAPABILITY);

RtcCapabilityGetController::RtcCapabilityGetController() : m_cslot(0), m_first_urc(true) {
    logI(RFX_LOG_TAG, "constructor entered");
}

RtcCapabilityGetController::~RtcCapabilityGetController() {}

void RtcCapabilityGetController::onInit() {
    RfxController::onInit();

    const int urc_id_list[] = {
            RFX_MSG_URC_RADIO_CAPABILITY,
    };
    logD(RFX_LOG_TAG, "onInit");

    // register request & URC id list
    // NOTE. one id can only be registered by one controller
    registerToHandleUrc(urc_id_list, sizeof(urc_id_list) / sizeof(const int));
    char tempstr[RFX_PROPERTY_VALUE_MAX] = {0};
    memset(tempstr, 0, sizeof(tempstr));
    rfx_property_get("persist.vendor.radio.c_capability_slot", tempstr, "1");
    m_cslot = atoi(tempstr) - 1;
}

bool RtcCapabilityGetController::onHandleUrc(const sp<RfxMessage>& message) {
    int msg_id = message->getId();
    RIL_RadioCapability* capability = (RIL_RadioCapability*)message->getData()->getData();

    logD(RFX_LOG_TAG, "onHandleUrc, handle: %s", idToString(msg_id));
    logI(RFX_LOG_TAG, "RadioCapability version=%d, session=%d, phase=%d, rat=%d, uuid=%s, state=%d",
         capability->version, capability->session, capability->phase, capability->rat,
         capability->logicalModemUuid, capability->status);
    if (msg_id == RFX_MSG_URC_RADIO_CAPABILITY) {
        // 3. if the URC need to be sent to RILJ, send it,
        //    be able to update parceled data if required
        responseToRilj(message);
    }
    return true;
}

// Only allow to be called by RtcModeSwitchController for once after SIM switch
void RtcCapabilityGetController::updateRadioCapability(int cslot) {
    char property_value[RFX_PROPERTY_VALUE_MAX] = {0};
    int session_id;

    rfx_property_get("vendor.ril.rc.session.id1", property_value, "-1");
    session_id = atoi(property_value);
    if (session_id != -1 ||
        getNonSlotScopeStatusManager()->getIntValue(RFX_STATUS_KEY_CAPABILITY_SWITCH_STATE) ==
                CAPABILITY_SWITCH_STATE_ENDING) {
        updateRadioCapability(cslot, session_id);
        // Clear session id and set radio state if SIM switch has been done successfully
        rfx_property_set("vendor.ril.rc.session.id1", "-1");
        if (RtcCapabilitySwitchUtil::isDssNoResetSupport() == false) {
            for (int i = 0; i < RfxRilUtils::rfxGetSimCount(); i++) {
                getStatusManager(i)->setIntValue(RFX_STATUS_KEY_RADIO_STATE, RADIO_STATE_OFF, false,
                                                 false);
            }
        }
        getNonSlotScopeStatusManager()->registerStatusChanged(
                RFX_STATUS_KEY_MODESWITCH_FINISHED,
                RfxStatusChangeCallback(this, &RtcCapabilityGetController::onModeSwitchFinished));
    } else if (cslot != m_cslot || m_first_urc) {
        updateRadioCapability(cslot, session_id);
    } else {
        logI(RFX_LOG_TAG, "updateRadioCapability cslot=%d is same with before.", cslot);
    }
}

void RtcCapabilityGetController::onModeSwitchFinished(RfxStatusKeyEnum key, RfxVariant old_value,
                                                      RfxVariant value) {
    RFX_UNUSED(key);
    logD(RFX_LOG_TAG, "onModeSwitchFinished (%d, %d)", old_value.asInt(), value.asInt());
    RtcCapabilitySwitchController* switchController =
            (RtcCapabilitySwitchController*)findController(
                    RFX_OBJ_CLASS_INFO(RtcCapabilitySwitchController));
    switchController->notifySetRatDone();
    getNonSlotScopeStatusManager()->unRegisterStatusChanged(
            RFX_STATUS_KEY_MODESWITCH_FINISHED,
            RfxStatusChangeCallback(this, &RtcCapabilityGetController::onModeSwitchFinished));
}

void RtcCapabilityGetController::updateRadioCapability(int cslot, int session_id) {
    char property_value[RFX_PROPERTY_VALUE_MAX] = {0};
    int radio_capability;

    m_cslot = cslot;
    m_first_urc = false;

    if (RtcCapabilitySwitchUtil::isDisableC2kCapability()) {
        for (int i = 0; i < RfxRilUtils::rfxGetSimCount(); i++) {
            radio_capability = getStatusManager(i)->getIntValue(RFX_STATUS_KEY_SLOT_CAPABILITY, 0);
            logI(RFX_LOG_TAG, "updateRadioCapability, no c2k, session=%d, capability[%d] = %d",
                 session_id, i, radio_capability);
            sendRadioCapabilityURC(i, session_id, radio_capability);
        }
    } else {
        for (int i = 0; i < RfxRilUtils::rfxGetSimCount(); i++) {
            radio_capability = getStatusManager(i)->getIntValue(RFX_STATUS_KEY_SLOT_CAPABILITY, 0);
            if (i == cslot) {
                radio_capability |= (RAF_CDMA_GROUP | RAF_EVDO_GROUP);
            } else {
                radio_capability &= ~(RAF_CDMA_GROUP | RAF_EVDO_GROUP);
            }
            logI(RFX_LOG_TAG, "updateRadioCapability, cslot=%d, session=%d, capability[%d] = %d",
                 cslot, session_id, i, radio_capability);
            getStatusManager(i)->setIntValue(RFX_STATUS_KEY_SLOT_CAPABILITY, radio_capability,
                                             false, false);

            // always send CDMA capability from Android Q
            radio_capability |= (RAF_CDMA_GROUP | RAF_EVDO_GROUP);
            sendRadioCapabilityURC(i, session_id, radio_capability);
        }
    }
}

void RtcCapabilityGetController::sendRadioCapabilityURC(int slot_id, int session_id,
                                                        int radio_capability) {
    RIL_RadioCapability rc;

    memset(&rc, 0, sizeof(RIL_RadioCapability));
    rc.version = RIL_RADIO_CAPABILITY_VERSION;
    rc.session = session_id;
    rc.phase = RC_PHASE_UNSOL_RSP;
    rc.status = RC_STATUS_SUCCESS;
    rc.rat = radio_capability;
    RfxRilUtils::getLogicalModemId(rc.logicalModemUuid, MAX_UUID_LENGTH, slot_id);
    sp<RfxMessage> urc =
            RfxMessage::obtainUrc(slot_id, RFX_MSG_URC_RADIO_CAPABILITY,
                                  RfxRadioCapabilityData(&rc, sizeof(RIL_RadioCapability)));
    responseToRilj(urc);
}

void RtcCapabilityGetController::updateRadioCapabilityForWMChange(int world_mode) {
    int fixed_radio_capability, radio_capability;
    int major_slot =
            getNonSlotScopeStatusManager()->getIntValue(RFX_STATUS_KEY_MAIN_CAPABILITY_SLOT, 0);

    if (RfxRilUtils::isTplusWSupport() || RfxRilUtils::getKeep3GMode() != 0) {
        return;
    }

    for (int i = 0; i < RfxRilUtils::rfxGetSimCount(); i++) {
        if (i == major_slot) {
            continue;
        }
        fixed_radio_capability =
                getStatusManager(i)->getIntValue(RFX_STATUS_KEY_SLOT_FIXED_CAPABILITY, 0);
        radio_capability = getStatusManager(i)->getIntValue(RFX_STATUS_KEY_SLOT_CAPABILITY, 0);
        if (world_mode == 2) {
            // Remove 3G raf for non major SIMs in TDD mode
            radio_capability &= ~RAF_UMTS;
        } else {
            radio_capability |= (fixed_radio_capability & RAF_UMTS);
        }
        getStatusManager(i)->setIntValue(RFX_STATUS_KEY_SLOT_CAPABILITY, radio_capability, false,
                                         false);
        if (RtcCapabilitySwitchUtil::isDisableC2kCapability() == false) {
            // always send CDMA capability from Android Q
            radio_capability |= (RAF_CDMA_GROUP | RAF_EVDO_GROUP);
        }
        sendRadioCapabilityURC(i, -1, radio_capability);
    }
}
