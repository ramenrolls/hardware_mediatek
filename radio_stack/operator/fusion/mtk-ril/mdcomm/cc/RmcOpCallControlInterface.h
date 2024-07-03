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

#ifndef __RMC_OP_CALL_CONTROL_INTERFACE_H__
#define __RMC_OP_CALL_CONTROL_INTERFACE_H__

// AT cmd
#define AT_DIAL_FROM (char*)"ATDF"
#define AT_SET_ECC_CONFIG (char*)"AT+ESCI"
#define AT_DEVICE_SWITCH (char*)"AT+ECT"
#define AT_CANCEL_DEVICE_SWITCH (char*)"AT+ECT"
#define AT_SET_RTT_MODE (char*)"AT+EIMSRTT"
// URC

// Constants
#define MSG_TYPE_DEVICE_SWITCH 3
#define MSG_TYPE_CANCEL_DEVICE_SWITCH 4

#endif
