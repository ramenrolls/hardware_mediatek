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

#ifndef __RMC_MOBILE_WIFI_INTERFACE_H__
#define __RMC_MOBILE_WIFI_INTERFACE_H__

// AT cmd
#define AT_SET_WIFI_ENABLE (char*)"AT+EWIFIEN"
#define AT_SET_WIFI_ASSOCIATED (char*)"AT+EWIFIASC"
#define AT_SET_WIFI_SIGNAL_LEVEL (char*)"AT+EWIFISIGLVL"
#define AT_SET_WIFI_IP_ADDRESS (char*)"AT+EWIFIADDR"
#define AT_SET_GEO_LOCATION (char*)"AT+EIMSGEO"
#define AT_SET_LOCATION_ENABLE (char*)"AT+ELOCAEN"
#define AT_SET_ECC_AID (char*)"AT+EIMSAID"
#define AT_SET_NATT_KEEP_ALIVE_STATUS (char*)"AT+EWIFINATT"
#define AT_SET_WIFI_PING_RESULT (char*)"AT+EIWLPING"
#define AT_SET_AIRPLANE_MODE (char*)"AT+EAPMODE"
/// M: Notify ePDG screen state
#define AT_SET_EPDG_SCREEN_STATE (char*)"AT+ESCREENSTATE"
#define AT_QUERY_SSAC (char*)"AT+CSSAC"

// URC
#define URC_WFC_WIFI_ROVEOUT (char*)"+EWFCRVOUT"
#define URC_PDN_HANDOVER (char*)"+EPDNHANDOVER"
#define URC_ACTIVE_WIFI_PDN_COUNT (char*)"+EWIFIPDNACT"
#define URC_WIFI_RSSI_MONITOR_CONFIG (char*)"+EWIFIRSSITHRCFG"
#define URC_WIFI_PDN_ERROR (char*)"+EWOPDNERR"
#define URC_MD_GEO_REQUEST (char*)"+EIMSGEO"
#define URC_NATT_KEEP_ALIVE_CHANGED (char*)"+EWOKEEPALIVE"
#define URC_WIFI_PING_REQUEST (char*)"+EIWLPING"
#define URC_WIFI_PDN_OOS (char*)"+EIMSPDNOOS"
#define URC_WIFI_LOCK (char*)"+EWIFILOCK"
#define URC_ESSAC_NOTIFY (char*)"+ESSAC"

#endif
