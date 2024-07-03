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

#ifndef __RMC_OP_IMS_CONTROL_INTERFACE_H__
#define __RMC_OP_IMS_CONTROL_INTERFACE_H__

// AT cmd
#define AT_SEND_RTT_MODIFY_REQUEST (char*)"AT+RTTCALL"
#define AT_SEND_RTT_TEXT (char*)"AT+ERTTSTR"
#define AT_MODIFY_REQUEST_RESPONSE (char*)"AT+PRTTCALL"
#define AT_SET_DIGITS_LINE (char*)"AT+DIGITLINE"

// URC
#define URC_RTT_MODIFY_RESPONSE (char*)"+RTTCALL"
#define URC_RTT_TEXT_RECEIVE (char*)"+ERTTSTR"
#define URC_GTT_CAPABILITY_INDICATION (char*)"+EIMSTCAP"
#define URC_RTT_MODIFY_REQUEST_RECEIVE (char*)"+PRTTCALL"
#define URC_DIGITS_LINE_INDICATION (char*)"+DIGITLINE"

#endif
