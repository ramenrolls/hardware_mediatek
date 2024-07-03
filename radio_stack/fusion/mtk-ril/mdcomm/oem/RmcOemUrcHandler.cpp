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

#include "RmcOemUrcHandler.h"
#include "RfxIntsData.h"
#include "RfxStringData.h"
#include "RfxRawData.h"
#include <libmtkrilutils.h>
#include "RfxMclDispatcherThread.h"

#define RFX_LOG_TAG "RmcOemUrcHandler"

// register handler to channel
RFX_IMPLEMENT_HANDLER_CLASS(RmcOemUrcHandler, RIL_CMD_PROXY_URC);

RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_UNSOL_TX_POWER);
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_UNSOL_TX_POWER_STATUS);
RFX_REGISTER_DATA_TO_URC_ID(RfxRawData, RFX_MSG_UNSOL_OEM_HOOK_RAW);
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_UNSOL_ON_DSDA_CHANGED);

RmcOemUrcHandler::RmcOemUrcHandler(int slot_id, int channel_id)
    : RfxBaseHandler(slot_id, channel_id) {
    const char* urc[] = {
            (char*)"+EWARNING",  (char*)"+ETXPWR", (char*)"+ETXPWRSTUS", (char*)"+EMCFRPT",
            (char*)"+ENVSIGERR", (char*)"+EDSDAU", (char*)"+EDMFAPP",
            /* For oemurc function, please replace "+XXXX" with the
             at command that you want to send to framework. */
            //(char *) "+XXXX",
    };

    registerToHandleURC(urc, sizeof(urc) / sizeof(char*));
}

RmcOemUrcHandler::~RmcOemUrcHandler() {}

void RmcOemUrcHandler::onHandleUrc(const sp<RfxMclMessage>& msg) {
    int err = 0;
    RfxAtLine* line = msg->getRawUrc();
    char* data = line->getLine();
    if (strStartsWith(data, "+EWARNING")) {
        handleEWarning(line);
    } else if (strStartsWith(data, "+ETXPWRSTUS")) {
        handleTXPowerStatus(line);
    } else if (strStartsWith(data, "+ETXPWR")) {
        handleTXPower(line);
    } else if (strStartsWith(data, "+ENVSIGERR")) {
        handleNvErr(line);
    } else if (strStartsWith(data, "+EMCFRPT") || strStartsWith(data, "+EDMFAPP")) {
        handleOemUrc(line);
    } else if (strStartsWith(data, "+EDSDAU")) {
        handleDsdaUrc(line);
    } /* For oemurc function, please replace "+XXXX" with the
         at command that you want to send to framework.
       */
      /*
        else if (strStartsWith(data, "+XXXX")) {
        handleOemUrc(line);
    } */
    else {
        RFX_LOG_E(RFX_LOG_TAG, "we can not handle this raw urc?! %s", data);
    }
}

void RmcOemUrcHandler::handleEWarning(RfxAtLine* line) {
    int err = 0;
    line->atTokStart(&err);

    if (line->atTokHasmore()) {
        RFX_LOG_D(RFX_LOG_TAG, "handleEWarning, get warning message");
        char warningMessage[255] = {0};
        char* s = line->getLine();
        strncpy(warningMessage, s, 254);

        char modemVersion[256] = {0};
        getMSimProperty(m_slot_id, (char*)"gsm.version.baseband", modemVersion);
        RFX_LOG_D(RFX_LOG_TAG, "handleEWarning, modem version = %s", modemVersion);

        RFX_LOG_D(RFX_LOG_TAG, "handleEWarning, warningMessage = %s\n", warningMessage);
        RfxRilUtils::handleAee(warningMessage, modemVersion);
    }
}

/**
 * Modem will send +ETXPWR after receiving AT+ERFTX
 */
void RmcOemUrcHandler::handleTXPower(RfxAtLine* line) {
    RFX_LOG_D(RFX_LOG_TAG, "handleTXPower: %s", line->getLine());

    int err = 0;
    // response[0]: act (rat);
    // resposne[1]: txPower;
    int response[2] = {0, 0};
    sp<RfxMclMessage> urc;

    line->atTokStart(&err);
    if (err < 0) goto error;

    response[0] = line->atTokNextint(&err);
    if (err < 0) goto error;

    response[1] = line->atTokNextint(&err);
    if (err < 0) goto error;

    RFX_LOG_D(RFX_LOG_TAG, "handleTXPower: %d, %d", response[0], response[1]);
    urc = RfxMclMessage::obtainUrc(RFX_MSG_UNSOL_TX_POWER, m_slot_id, RfxIntsData(response, 2));
    responseToTelCore(urc);
    return;
error:
    RFX_LOG_E(RFX_LOG_TAG, "There is something wrong with the onNotifyTXPower URC");
}

/**
 * +ETXPWRSTUS: <event>, <sar_scenario_index>
 * <event>: Integer; Indicates the TX Power high or low
 *              0    TX power low indication
 *              1    TX power high indication
 *              FF  default value when there is no indication received from MMRF yet.
 * <sar_scenario_index>: Integer; Indicates the SAR scenario index
 *              FF ¨C If the SAR is not configured yet.
 */
void RmcOemUrcHandler::handleTXPowerStatus(RfxAtLine* line) {
    RFX_LOG_D(RFX_LOG_TAG, "handleTXPowerStatus: %s", line->getLine());

    int err = 0;
    // response[0]: event (Indicates the TX Power high or low)
    // resposne[1]: sar_scenario_index(Indicates the SAR scenario index)
    int response[2] = {0, 0};
    sp<RfxMclMessage> urc;

    line->atTokStart(&err);
    if (err < 0) goto error;

    response[0] = line->atTokNextint(&err);
    if (err < 0) goto error;

    response[1] = line->atTokNextint(&err);
    if (err < 0) goto error;

    RFX_LOG_D(RFX_LOG_TAG, "handleTXPowerStatus: %d, %d", response[0], response[1]);
    urc = RfxMclMessage::obtainUrc(RFX_MSG_UNSOL_TX_POWER_STATUS, m_slot_id,
                                   RfxIntsData(response, 2));
    responseToTelCore(urc);
    return;
error:
    RFX_LOG_E(RFX_LOG_TAG, "There is something wrong with the handleTXPowerStatus URC");
}

void RmcOemUrcHandler::handleOemUrc(RfxAtLine* line) {
    int err = 0;
    line->atTokStart(&err);
    RFX_LOG_D(RFX_LOG_TAG, "handleOemUrc");

    if (line->atTokHasmore()) {
        sp<RfxMclMessage> urc;
        char* s = line->getLine();
        RFX_LOG_D(RFX_LOG_TAG, "handleOemUrc, urc = %s\n", s);
        urc = RfxMclMessage::obtainUrc(RFX_MSG_UNSOL_OEM_HOOK_RAW, m_slot_id,
                                       RfxRawData(s, strlen(s)));
        responseToTelCore(urc);

        if (strStartsWith(s, "+EMCFRPT: 3,1")) {
            char value[RFX_PROPERTY_VALUE_MAX] = {0};
            rfx_property_get("ro.vendor.mtk_mcf_support", value, "0");
            if (atoi(value) == 1) {
                // Critical OTA apply flow, reset modem
                RFX_LOG_E(RFX_LOG_TAG, "+EMCFRPT: 3,1, reset modem");
                sp<RfxMclMessage> mclMessage = RfxMclMessage::obtainRequest(
                        RFX_MSG_REQUEST_RESET_RADIO, NULL, m_slot_id, 0, true, 0, 0, false);
                RfxMclDispatcherThread::enqueueMclMessage(mclMessage);
                return;
            }
        }
    }
}

/**
 * Modem will send +ENVSIGERR when NVRAM verified failed.
 */
void RmcOemUrcHandler::handleNvErr(RfxAtLine* line) {
    RFX_LOG_D(RFX_LOG_TAG, "handleNvErr: %s", line->getLine());

    int err = 0;
    // response[0]: 1:NVRAM error
    int response = 0;

    line->atTokStart(&err);
    if (err < 0) goto error;

    response = line->atTokNextint(&err);
    if (err < 0) goto error;

    RFX_LOG_D(RFX_LOG_TAG, "handleNvErr: %d", response);
    // enable recovery mode
    if (response == 1) {
        rfx_property_set("sys.powerctl", "reboot,recovery");
    }
    return;
error:
    RFX_LOG_E(RFX_LOG_TAG, "There is something wrong with the handleNvErr URC");
}

/**
 * Modem will send +EDSDAU when DSDA mode changed
 */
void RmcOemUrcHandler::handleDsdaUrc(RfxAtLine* line) {
    int err = 0;
    // response[0]: 1:DSDA mode
    int response = 0;
    sp<RfxMclMessage> urc;

    RFX_LOG_D(RFX_LOG_TAG, "handleDsdaUrc: %s", line->getLine());

    line->atTokStart(&err);
    if (err < 0) goto error;

    response = line->atTokNextint(&err);
    if (err < 0) goto error;

    RFX_LOG_D(RFX_LOG_TAG, "handleDsdaUrc: %d", response);
    getNonSlotMclStatusManager()->setIntValue(RFX_STATUS_DSDA_MODE, response);
    urc = RfxMclMessage::obtainUrc(RFX_MSG_UNSOL_ON_DSDA_CHANGED, m_slot_id,
                                   RfxIntsData(&response, 1));
    responseToTelCore(urc);
    return;
error:
    RFX_LOG_E(RFX_LOG_TAG, "There is something wrong with the handleNvErr URC");
}
