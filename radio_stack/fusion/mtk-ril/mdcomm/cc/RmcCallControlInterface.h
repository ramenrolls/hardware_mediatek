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

#ifndef __RMC_CALL_CONTROL_INTERFACE_H__
#define __RMC_CALL_CONTROL_INTERFACE_H__

// AT cmd
#define AT_CONF_DIAL (char*)"AT+EDCONF"
#define AT_SIP_URI_DIAL (char*)"AT+CDU"
#define AT_CONF_MODIFY (char*)"AT+ECONF"
#define AT_IMS_CALL_CTRL (char*)"AT+ECCTRL"
#define AT_VDS_MODE (char*)"AT+EVADSMOD"
#define AT_FORCE_RELEASE (char*)"AT+ECHUP"
#define AT_EMERGENCY_DIAL (char*)"ATDE"
#define AT_DIAL (char*)"ATD"
#define AT_ANSWER (char*)"ATA"
#define AT_ALLOW_MT (char*)"AT+EAIC"
#define AT_EXIT_ECBM (char*)"AT+VMEMEXIT"
#define AT_ENABLE_VDS_REPORT (char*)"AT+EVADSREP"
#define AT_ANSWER_VT_FUNCTIONAL (char*)"AT+EVTA"
#define AT_APPROVE_ECC_REDIAL (char*)"AT+ERDECCAPV"
#define AT_PULL_CALL (char*)"AT+ECALLPULL"
#define AT_IMS_ECT (char*)"AT+ECT"
#define AT_SET_RTT_MODE (char*)"AT+EIMSRTT"
#define AT_SET_SIP_HEADER (char*)"AT+ESIPHEADER"
#define AT_SIP_HEADER_REPORT (char*)"AT+EIMSHEADER"
#define AT_SET_IMS_CALL_MODE (char*)"AT+EIMSCALLMODE"
#define AT_SET_IMS_CALL_ADDITIONAL (char*)"AT+EIMSCAI"

// URC
#define URC_CONF_SRVCC (char*)"+ECONFSRVCC"
#define URC_CONF_MODIFIED_RESULT (char*)"+ECONF:"
#define URC_VDM_SELECT_RESULT (char*)"+EVADSREP"
#define URC_IMS_CALL_MODE (char*)"+EIMSCMODE"
#define URC_IMS_VIDEO_CAP (char*)"+EIMSVCAP"
#define URC_ECPLICIT_CALL_TRANSFER (char*)"+ECT:"
#define URC_IMS_CALL_CONTROL_RESULT (char*)"+ECCTRL"
#define URC_IMS_EVENT_PACKAGE (char*)"+EIMSEVTPKG"
#define URC_SRVCC (char*)"+CIREPH"
#define URC_CONF_CHECK (char*)"+ECONFCHECK"
#define URC_ECC_BEARER (char*)"+CNEMS1"
#define URC_REDIAL_ECC_INDICATION (char*)"+ERDECCIND"
#define URC_IMS_HEADER (char*)"+EIMSHEADER"
#define URC_CALL_ADDITIONAL_INFO (char*)"+EIMSCAI"

#endif
