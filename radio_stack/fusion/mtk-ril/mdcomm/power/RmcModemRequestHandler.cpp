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

#include "RmcModemRequestHandler.h"
#include "RfxVoidData.h"

#define RFX_LOG_TAG "RmcModemRequestHandler"

// register handler to channel
RFX_IMPLEMENT_HANDLER_CLASS(RmcModemRequestHandler, RIL_CMD_PROXY_9);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxVoidData, RFX_MSG_REQUEST_MODEM_POWERON);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxVoidData, RFX_MSG_REQUEST_MODEM_POWEROFF);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxVoidData, RFX_MSG_REQUEST_RESET_RADIO);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxVoidData, RFX_MSG_REQUEST_RESTART_RILD);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxVoidData, RFX_MSG_REQUEST_SHUTDOWN);

RmcModemRequestHandler::RmcModemRequestHandler(int slotId, int channelId)
    : RfxBaseHandler(slotId, channelId) {
    logD(RFX_LOG_TAG, "RmcModemRequestHandler constructor");
    const int request[] = {
            RFX_MSG_REQUEST_MODEM_POWERON, RFX_MSG_REQUEST_MODEM_POWEROFF,
            RFX_MSG_REQUEST_RESET_RADIO,   RFX_MSG_REQUEST_RESTART_RILD,
            RFX_MSG_REQUEST_SHUTDOWN,
    };
    registerToHandleRequest(request, sizeof(request) / sizeof(int));
}

RmcModemRequestHandler::~RmcModemRequestHandler() {}

void RmcModemRequestHandler::onHandleRequest(const sp<RfxMclMessage>& msg) {
    int id = msg->getId();
    logD(RFX_LOG_TAG, "onHandleRequest: %s(%d)", idToString(id), id);
    switch (id) {
        case RFX_MSG_REQUEST_MODEM_POWERON:
            requestModemPowerOn(msg);
            break;
        case RFX_MSG_REQUEST_MODEM_POWEROFF:
        case RFX_MSG_REQUEST_SHUTDOWN:
            requestModemPowerOff(msg);
            break;
        case RFX_MSG_REQUEST_RESET_RADIO:
        case RFX_MSG_REQUEST_RESTART_RILD:
            requestResetRadio(msg);
            break;
        default:
            logE(RFX_LOG_TAG, "Should not be here");
            break;
    }
}

void RmcModemRequestHandler::requestModemPowerOn(const sp<RfxMclMessage>& msg) {
    logD(RFX_LOG_TAG, "%s", __FUNCTION__);
    rfx_property_set("ril.getccci.response", "0");

    bool mdOn = !getNonSlotMclStatusManager()->getBoolValue(RFX_STATUS_KEY_MODEM_POWER_OFF, false);
    if (mdOn) {
        logD(RFX_LOG_TAG, "requestModemPowerOn but already modem powered on");
        sp<RfxMclMessage> responseMsg =
                RfxMclMessage::obtainResponse(RIL_E_SUCCESS, RfxVoidData(), msg);
        responseToTelCore(responseMsg);
        return;
    }
    getNonSlotMclStatusManager()->setBoolValue(RFX_STATUS_KEY_MODEM_POWER_OFF, false);

    logD(RFX_LOG_TAG, "SET vendor.ril.ipo.radiooff to 0");
    rfx_property_set("vendor.ril.ipo.radiooff", "0");

    logD(RFX_LOG_TAG, "Flight mode power on modem, trigger CCCI power on modem (new versio)");
    RfxRilUtils::triggerCCCIIoctl(CCCI_IOC_LEAVE_DEEP_FLIGHT_ENHANCED);

    sp<RfxMclMessage> responseMsg =
            RfxMclMessage::obtainResponse(RIL_E_SUCCESS, RfxVoidData(), msg);
    responseToTelCore(responseMsg);
    rfx_property_set("ril.getccci.response", "1");
}

void RmcModemRequestHandler::requestModemPowerOff(const sp<RfxMclMessage>& msg) {
    int mdOff = getNonSlotMclStatusManager()->getBoolValue(RFX_STATUS_KEY_MODEM_POWER_OFF, false);
    logD(RFX_LOG_TAG, "%s(): mdOff:%d, rid:%d.", __FUNCTION__, mdOff, m_slot_id);

    if (mdOff) {
        logD(RFX_LOG_TAG, "Flight mode power off modem but already modem powered off");
        sp<RfxMclMessage> responseMsg =
                RfxMclMessage::obtainResponse(RIL_E_SUCCESS, RfxVoidData(), msg);
        responseToTelCore(responseMsg);
        return;
    }

    atSendCommand("AT+EFUN=0");
    // Normal AT may be blocked if s_md_off is 1
    getNonSlotMclStatusManager()->setBoolValue(RFX_STATUS_KEY_MODEM_POWER_OFF, true);

    logD(RFX_LOG_TAG, "ENTER requestRadio PowerOff, set vendor.ril.ipo.radiooff to -1");
    rfx_property_set("vendor.ril.ipo.radiooff", "-1");

    atSendCommand("AT+ECUSD=2,2");
    atSendCommand("AT+EMDT=0");
    atSendCommand("AT+EPOF");
    RfxRilUtils::triggerCCCIIoctl(CCCI_IOC_ENTER_DEEP_FLIGHT_ENHANCED);

    logD(RFX_LOG_TAG, "requestRadioPowerOff SET vendor.ril.ipo.radiooff to 1");
    rfx_property_set("vendor.ril.ipo.radiooff", "1");

    for (int i = 0; i < RfxRilUtils::rfxGetSimCount(); i++) {
        getMclStatusManager(i)->setIntValue(RFX_STATUS_KEY_RADIO_STATE, RADIO_STATE_OFF);
    }
    logD(RFX_LOG_TAG, "Flight mode power off modem, trigger CCCI level 2 power off");
    int param = -1;

    sp<RfxMclMessage> responseMsg =
            RfxMclMessage::obtainResponse(RIL_E_SUCCESS, RfxVoidData(), msg);
    responseToTelCore(responseMsg);

    /*if (queryBtSapStatus(rid) != BT_SAP_INIT) {
        LOGD("Flight mode power off modem, during SAP connection => disconnect SAP connection");
        disconnectSapConnection(rid);
    }*/
}

void RmcModemRequestHandler::requestResetRadio(const sp<RfxMclMessage>& msg) {
    logD(RFX_LOG_TAG, "start to reset radio - requestResetRadio");
    int mainSlotId =
            getNonSlotMclStatusManager()->getIntValue(RFX_STATUS_KEY_MAIN_CAPABILITY_SLOT, 0);
    int mdOff = getNonSlotMclStatusManager()->getBoolValue(RFX_STATUS_KEY_MODEM_POWER_OFF, false);

    // only do power off when it is on
    if (!mdOff) {
        getNonSlotMclStatusManager()->setBoolValue(RFX_STATUS_KEY_MODEM_POWER_OFF, true);
        atSendCommand("AT+EMDT=0");
        atSendCommand("AT+EFUN=0");
        logD(RFX_LOG_TAG, "%s(): set radio off before power off MD.", __FUNCTION__);
        atSendCommand("AT+EPOF");

        // power off modem
        logD(RFX_LOG_TAG, "requestRadioPowerOff SET vendor.ril.ipo.radiooff to 1");
        rfx_property_set("vendor.ril.ipo.radiooff", "1");
        for (int i = 0; i < RfxRilUtils::rfxGetSimCount(); i++) {
            getMclStatusManager(i)->setIntValue(RFX_STATUS_KEY_RADIO_STATE, RADIO_STATE_OFF);
        }
        logD(RFX_LOG_TAG, "Flight mode power off modem, trigger CCCI level 2 power off");
        int param = -1;
        RfxRilUtils::triggerCCCIIoctl(CCCI_IOC_ENTER_DEEP_FLIGHT_ENHANCED);
    }

    // power on modem
    logD(RFX_LOG_TAG, "SET vendor.ril.ipo.radiooff to 0");
    rfx_property_set("vendor.ril.ipo.radiooff", "0");
    logD(RFX_LOG_TAG, "Flight mode power on modem, trigger CCCI power on modem (new versio)");
    RfxRilUtils::triggerCCCIIoctl(CCCI_IOC_LEAVE_DEEP_FLIGHT_ENHANCED);

    sp<RfxMclMessage> responseMsg =
            RfxMclMessage::obtainResponse(RIL_E_SUCCESS, RfxVoidData(), msg);
    responseToTelCore(responseMsg);
}
