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

#include "RmcHardwareConfigRequestHandler.h"
#include "RfxRilUtils.h"
#include "RfxVoidData.h"
#include <string.h>

#define RFX_LOG_TAG "RmcHdcHdr"

#define DEV_MODEM_RIL_MODEL_SINGLE 0;

// register data
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxHardwareConfigData,
                                RFX_MSG_REQUEST_GET_HARDWARE_CONFIG);
RFX_REGISTER_DATA_TO_URC_ID(RfxHardwareConfigData, RFX_MSG_UNSOL_HARDWARE_CONFIG_CHANGED);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxVoidData, RFX_MSG_EVENT_CAPABILITY_INIT_DONE);
// register handler to channel
RFX_IMPLEMENT_HANDLER_CLASS(RmcHardwareConfigRequestHandler, RIL_CMD_PROXY_1);

RmcHardwareConfigRequestHandler::RmcHardwareConfigRequestHandler(int slot_id, int channel_id)
    : RfxBaseHandler(slot_id, channel_id) {
    const int request[] = {RFX_MSG_REQUEST_GET_HARDWARE_CONFIG};

    const int eventList[] = {RFX_MSG_EVENT_CAPABILITY_INIT_DONE};
    registerToHandleRequest(request, sizeof(request) / sizeof(int));
    registerToHandleEvent(eventList, sizeof(eventList) / sizeof(int));
}

RmcHardwareConfigRequestHandler::~RmcHardwareConfigRequestHandler() {}

void RmcHardwareConfigRequestHandler::onHandleRequest(const sp<RfxMclMessage>& msg) {
    int id = msg->getId();
    logD(RFX_LOG_TAG, "onHandleRequest: %d", id);
    switch (id) {
        case RFX_MSG_REQUEST_GET_HARDWARE_CONFIG:
            handleHardwareConfigRequest(msg);
            break;
        default:
            logE(RFX_LOG_TAG, "Should not be here");
            break;
    }
}

void RmcHardwareConfigRequestHandler::onHandleEvent(const sp<RfxMclMessage>& msg) {
    int id = msg->getId();
    logD(RFX_LOG_TAG, "onHandleEvent: %d", id);
    switch (id) {
        case RFX_MSG_EVENT_CAPABILITY_INIT_DONE:
            sendHardwareConfigUrc();
            break;
        default:
            logE(RFX_LOG_TAG, "should not be here");
            break;
    }
}

void RmcHardwareConfigRequestHandler::handleHardwareConfigRequest(const sp<RfxMclMessage>& msg) {
    RIL_HardwareConfig hardwareConfigs[2];
    logD(RFX_LOG_TAG, "handleHardwareConfigRequest: m_slot_id:%d", m_slot_id);
    getHardwareConfig(hardwareConfigs);

    sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(
            msg->getId(), RIL_E_SUCCESS,
            RfxHardwareConfigData((void*)hardwareConfigs, sizeof(hardwareConfigs)), msg, false);
    responseToTelCore(response);
    sendHardwareConfigUrc();
}

void RmcHardwareConfigRequestHandler::sendHardwareConfigUrc() {
    RIL_HardwareConfig hardwareConfigs[2];
    logD(RFX_LOG_TAG, "sendHardwareConfigUrc: m_slot_id:%d", m_slot_id);
    getHardwareConfig(hardwareConfigs);

    sp<RfxMclMessage> urc = RfxMclMessage::obtainUrc(
            RFX_MSG_UNSOL_HARDWARE_CONFIG_CHANGED, m_slot_id,
            RfxHardwareConfigData((void*)hardwareConfigs, sizeof(hardwareConfigs)));
    responseToTelCore(urc);
}

void RmcHardwareConfigRequestHandler::getHardwareConfig(RIL_HardwareConfig* hardwareConfigs) {
    char logicalModemId[MAX_UUID_LENGTH];
    RfxRilUtils::getLogicalModemId(logicalModemId, sizeof(logicalModemId), m_slot_id);

    // type modem
    hardwareConfigs[0].type = RIL_HARDWARE_CONFIG_MODEM;
    strncpy(hardwareConfigs[0].uuid, logicalModemId, MAX_UUID_LENGTH);
    hardwareConfigs[0].state = RIL_HARDWARE_CONFIG_STATE_ENABLED;
    hardwareConfigs[0].cfg.modem.rilModel = DEV_MODEM_RIL_MODEL_SINGLE;

    int slotCapability =
            getMclStatusManager(m_slot_id)->getIntValue(RFX_STATUS_KEY_SLOT_CAPABILITY, 0);
    hardwareConfigs[0].cfg.modem.rat = slotCapability;
    hardwareConfigs[0].cfg.modem.maxVoice = 1;
    hardwareConfigs[0].cfg.modem.maxData = 1;
    hardwareConfigs[0].cfg.modem.maxStandby = RfxRilUtils::rfxGetSimCount();

    // type sim
    hardwareConfigs[1].type = RIL_HARDWARE_CONFIG_SIM;
    strncpy(hardwareConfigs[1].uuid, logicalModemId, MAX_UUID_LENGTH);
    hardwareConfigs[1].state = RIL_HARDWARE_CONFIG_STATE_ENABLED;
    strncpy(hardwareConfigs[1].cfg.sim.modemUuid, logicalModemId, MAX_UUID_LENGTH);
}
