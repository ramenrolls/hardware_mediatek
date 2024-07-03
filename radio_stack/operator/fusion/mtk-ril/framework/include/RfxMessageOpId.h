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

#ifndef __RFX_MESSAGE_OP_ID_HEADER__
#define __RFX_MESSAGE_OP_ID_HEADER__

RFX_OP_MSG_HELLO_START,
#include "RfxHelloMessageOpId.h"
        RFX_OP_MSG_HELLO_END = RFX_OP_MSG_HELLO_START + DEFAULT_MSG_RANGE, RFX_OP_MSG_IMS_START,
#include "RfxImsMessageOpId.h"
        RFX_OP_MSG_IMS_END = RFX_OP_MSG_IMS_START + DEFAULT_MSG_RANGE, RFX_OP_MSG_NW_START,
#include "RfxNwMessageOpId.h"
        RFX_OP_MSG_NW_END = RFX_OP_MSG_NW_START + DEFAULT_MSG_RANGE, RFX_OP_MSG_CC_START,
#include "RfxCallControlMessageOpId.h"
        RFX_OP_MSG_CC_END = RFX_OP_MSG_CC_START + DEFAULT_MSG_RANGE, RFX_OP_MSG_SMS_START,
#include "RfxSCBMMessageOpId.h"
        RFX_OP_MSG_SMS_END = RFX_OP_MSG_SMS_START + DEFAULT_MSG_RANGE, RFX_OP_MSG_SIM_START,
#include "RfxSimMessageOpId.h"
        RFX_OP_MSG_SIM_END = RFX_OP_MSG_SIM_START + DEFAULT_MSG_RANGE,
#endif
