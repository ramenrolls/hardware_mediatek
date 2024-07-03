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

// Std include
#include <vector>
#include <string>

// MTK Fusion Fwk include
#include "RfxStringsData.h"
#include "RfxIntsData.h"
#include "RfxVoidData.h"

// MWI Local include
#include "RmcMobileWifiInterface.h"
#include "RmcMobileWifiUrcHandler.h"

#define RFX_LOG_TAG "RmcMwi"

// register handler to channel
RFX_IMPLEMENT_HANDLER_CLASS(RmcMobileWifiUrcHandler, RIL_CMD_PROXY_URC);

RFX_REGISTER_DATA_TO_URC_ID(RfxStringsData, RFX_MSG_UNSOL_MOBILE_WIFI_ROVEOUT);
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_UNSOL_MOBILE_WIFI_HANDOVER);
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_UNSOL_ACTIVE_WIFI_PDN_COUNT);
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_UNSOL_WIFI_RSSI_MONITORING_CONFIG);
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_UNSOL_WIFI_PDN_ERROR);
RFX_REGISTER_DATA_TO_URC_ID(RfxStringsData, RFX_MSG_UNSOL_REQUEST_GEO_LOCATION);
RFX_REGISTER_DATA_TO_URC_ID(RfxStringsData, RFX_MSG_UNSOL_NATT_KEEP_ALIVE_CHANGED);
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_UNSOL_WIFI_PING_REQUEST);
RFX_REGISTER_DATA_TO_URC_ID(RfxStringsData, RFX_MSG_UNSOL_WIFI_PDN_OOS);
RFX_REGISTER_DATA_TO_URC_ID(RfxStringsData, RFX_MSG_UNSOL_WIFI_LOCK);
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_UNSOL_SSAC_STATUS);

static const char* urc[] = {
        URC_WFC_WIFI_ROVEOUT,
        URC_PDN_HANDOVER,
        URC_ACTIVE_WIFI_PDN_COUNT,
        URC_WIFI_RSSI_MONITOR_CONFIG,
        URC_WIFI_PDN_ERROR,
        URC_MD_GEO_REQUEST,
        URC_NATT_KEEP_ALIVE_CHANGED,
        URC_WIFI_PING_REQUEST,
        URC_WIFI_PDN_OOS,
        URC_WIFI_LOCK,
};

RmcMobileWifiUrcHandler::RmcMobileWifiUrcHandler(int slot_id, int channel_id)
    : RfxBaseHandler(slot_id, channel_id) {
    registerToHandleURC(urc, sizeof(urc) / sizeof(char*));
}

RmcMobileWifiUrcHandler::~RmcMobileWifiUrcHandler() {}

void RmcMobileWifiUrcHandler::onHandleUrc(const sp<RfxMclMessage>& msg) {
    char* urc = msg->getRawUrc()->getLine();
    // logD(RFX_LOG_TAG, "[onHandleUrc]%s", urc);
    if (strStartsWith(urc, URC_WFC_WIFI_ROVEOUT)) {
        onNotifyWfcWifiRoveout(msg);
    } else if (strStartsWith(urc, URC_PDN_HANDOVER)) {
        onNotifyPdnHandover(msg);
    } else if (strStartsWith(urc, URC_ACTIVE_WIFI_PDN_COUNT)) {
        onNotifyActiveWifiPdnCount(msg);
    } else if (strStartsWith(urc, URC_WIFI_RSSI_MONITOR_CONFIG)) {
        onNotifyWifiRssiMonitoringConfig(msg);
    } else if (strStartsWith(urc, URC_WIFI_PDN_ERROR)) {
        onNotifyWifiPdnError(msg);
    } else if (strStartsWith(urc, URC_MD_GEO_REQUEST)) {
        onNotifyGeoRequest(msg);
    } else if (strStartsWith(urc, URC_NATT_KEEP_ALIVE_CHANGED)) {
        onNotifyNattKeepAliveChanged(msg);
    } else if (strStartsWith(urc, URC_WIFI_PING_REQUEST)) {
        onNotifyWifiPingRequest(msg);
    } else if (strStartsWith(urc, URC_WIFI_PDN_OOS)) {
        onNotifyWifiPdnOOS(msg);
    } else if (strStartsWith(urc, URC_WIFI_LOCK)) {
        onNotifyWifiLock(msg);
    } else if (strStartsWith(urc, URC_ESSAC_NOTIFY)) {
        onNotifySsacStatus(msg);
    }
}

void RmcMobileWifiUrcHandler::onHandleTimer() {
    // do something
}

void RmcMobileWifiUrcHandler::onNotifyWfcWifiRoveout(const sp<RfxMclMessage>& msg) {
    /*
     * +EWFCRVOUT: <ifname>,<rvout>,<mobike_ind>
     * <ifname>: "wlan0"
     * <rvout>: 1 = Wifi roveout, no LTE for H.O.; 0 = Wifi RSSI acceptable
     * <mobike_ind>: Mobike trigger indication. 0 = not trigger mobike, 1= trigger mobike
     */
    logD(RFX_LOG_TAG, "onNotifyWfcWifiRoveout()");
    const int maxLen = 3;
    int rfxMsg = RFX_MSG_UNSOL_MOBILE_WIFI_ROVEOUT;
    notifyStringsDataToTcl(msg, rfxMsg, maxLen);
}

void RmcMobileWifiUrcHandler::onNotifyPdnHandover(const sp<RfxMclMessage>& msg) {
    /*
     * +EPDNHANDOVER: <pdn_type>,<status>,<src_rat>,<des_rat>
     * <pdn_type>: 0 = IMS, 1 = SMS, 2 = MMS
     * <status>: 0 = start, 1 = success, -1 = failed
     * <src_rat>: source rat, 1 = LTE, 2 = WIFI
     * <des_rat>: destination rat, 1 = LTE, 2 = WIFI
     * <phoneId>: add by MCL
     */
    const int maxLen = 4;
    bool appendPhoneId = true;
    int rfxMsg = RFX_MSG_UNSOL_MOBILE_WIFI_HANDOVER;
    notifyIntsDataToTcl(msg, rfxMsg, maxLen, appendPhoneId);
}

void RmcMobileWifiUrcHandler::onNotifyActiveWifiPdnCount(const sp<RfxMclMessage>& msg) {
    /*
     * +EWIFIPDNACT: <cnt>
     * <cnt>: Wifi PDN count
     * <phoneId>: add by MCL
     */
    logD(RFX_LOG_TAG, "onNotifyActiveWifiPdnCount()");
    const int maxLen = 1;
    bool appendPhoneId = true;
    int rfxMsg = RFX_MSG_UNSOL_ACTIVE_WIFI_PDN_COUNT;
    notifyIntsDataToTcl(msg, rfxMsg, maxLen, appendPhoneId);
}

void RmcMobileWifiUrcHandler::onNotifyWifiRssiMonitoringConfig(const sp<RfxMclMessage>& msg) {
    /*
     * +EWIFIRSSITHRCFG: <en>,<thr_cnt>,<rssi_thr_1>,< rssi_thr_2>,...
     * <en>: enabled, 0 = disable; 1 = enabled
     * <thr_cnt>: threshold count
     * <rssi_thr_1>,< rssi_thr_2>,...: threshold
     * <phoneId>: add by MCL
     */
    const int maxLen = 0;  // dynamic
    bool appendPhoneId = true;
    int rfxMsg = RFX_MSG_UNSOL_WIFI_RSSI_MONITORING_CONFIG;
    notifyIntsDataToTcl(msg, rfxMsg, maxLen, appendPhoneId);
}

void RmcMobileWifiUrcHandler::onNotifyWifiPdnError(const sp<RfxMclMessage>& msg) {
    /*
     * +EWOPDNERR: <cnt>,<cause>,<sub_cause>
     * <cnt>: error cause count
     * <cause>: main cause
     * <sub_cause>: sub cause
     * <phoneId>: add by MCL
     */
    const int maxLen = 3;
    bool appendPhoneId = true;
    int rfxMsg = RFX_MSG_UNSOL_WIFI_PDN_ERROR;
    notifyIntsDataToTcl(msg, rfxMsg, maxLen, appendPhoneId);
}

void RmcMobileWifiUrcHandler::onNotifyGeoRequest(const sp<RfxMclMessage>& msg) {
    /*
     * +EIMSGEO: <account_id>,<broadcast_flag>,<latitude>,<longitude>,<accurate>
     * <account_id>: request id, 0~7
     * <broadcast_flag>: 0, 1
     * <latitude>: latitude from GPS, 0 as failed
     * <longitude>: longitude from GPS, 0 as failed
     * <accurate>: accurate from GPS, 0 as failed
     */
    const int maxLen = 5;
    int rfxMsg = RFX_MSG_UNSOL_REQUEST_GEO_LOCATION;
    notifyStringsDataToTcl(msg, rfxMsg, maxLen);
}

void RmcMobileWifiUrcHandler::onNotifyNattKeepAliveChanged(const sp<RfxMclMessage>& msg) {
    /*
     * +EWOKEEPALIVE: <enable>,<interval>,<src_ip>,<src_port>,<dst_ip>,<dst_port>
     * <enable>: enabled, 0 = disable; 1 = enabled
     * <interval>: NATT interval, if interval < 20s, wifi NATT keep alive will not start
     * <src_ip>: source IP
     * <src_port>: source port
     * <dst_ip>: destination IP
     * <dst_port>: destination port, if dst_port != 4500, wifi NATT keep alive will not start
     */
    logD(RFX_LOG_TAG, "onNotifyNattKeepAliveChanged()");
    const int maxLen = 6;
    int rfxMsg = RFX_MSG_UNSOL_NATT_KEEP_ALIVE_CHANGED;
    notifyStringsDataToTcl(msg, rfxMsg, maxLen);
}

void RmcMobileWifiUrcHandler::onNotifyWifiPingRequest(const sp<RfxMclMessage>& msg) {
    /*
     * +EIWLPING: <rat_type>
     * <rat_type>: rat type on which ping is requested
     * <phoneId>: add by MCL
     */
    // char *urc = msg->getRawUrc()->getLine();
    const int maxLen = 1;
    bool appendPhoneId = true;
    int rfxMsg = RFX_MSG_UNSOL_WIFI_PING_REQUEST;
    notifyIntsDataToTcl(msg, rfxMsg, maxLen, appendPhoneId);
}

void RmcMobileWifiUrcHandler::onNotifyWifiPdnOOS(const sp<RfxMclMessage>& msg) {
    /*
     * +EIMSPDNOOS: <apn>, <cid>, <oos_state>
     * <apn>: apn name
     * <cid>: call id
     * <oos_state>:
     * 0: OOS ended & PDN disconnected
     * 1: OOS started
     * 2: OOS ended & PDN resumed to use
     */
    logD(RFX_LOG_TAG, "onNotifyWifiPdnOOS()");
    const int maxLen = 3;
    int rfxMsg = RFX_MSG_UNSOL_WIFI_PDN_OOS;
    notifyStringsDataToTcl(msg, rfxMsg, maxLen);
}

void RmcMobileWifiUrcHandler::onNotifyWifiLock(const sp<RfxMclMessage>& msg) {
    /*
     * +EIMSPDNOOS: <iface>, <enable>
     * <iface>: interface name
     * <enable>:
     * 0: modem disable wifi lock
     * 1: modem enable wifi lock
     */
    logD(RFX_LOG_TAG, "onNotifyWifiLock()");
    const int maxLen = 2;
    int rfxMsg = RFX_MSG_UNSOL_WIFI_LOCK;
    notifyStringsDataToTcl(msg, rfxMsg, maxLen);
}

void RmcMobileWifiUrcHandler::onNotifySsacStatus(const sp<RfxMclMessage>& msg) {
    /*
     * +ESSAC: <BFVoice>,<BFVideo>,<BTVoice>,<BTVideo>
     * <iface>: interface name
     * <enable>:
     * 0: modem disable wifi lock
     * 1: modem enable wifi lock
     */
    const int maxLen = 4;
    bool appendPhoneId = true;
    int rfxMsg = RFX_MSG_UNSOL_SSAC_STATUS;
    notifyIntsDataToTcl(msg, rfxMsg, maxLen, appendPhoneId);
}
