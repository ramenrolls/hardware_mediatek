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

#ifndef __RFX_MESSAGE_ID_HEADER__
#define __RFX_MESSAGE_ID_HEADER__

#define RFX_MSG_ID_EXPN(id) id,
#define DEFAULT_MSG_RANGE 500

typedef enum {
    RFX_MESSAGE_ID_BEGIN = 50000,
    /* Please add your event id file below this line */
    RFX_MSG_HELLO_START,
#include "RfxHelloMessageId.h"
    RFX_MSG_HELLO_END = RFX_MSG_HELLO_START + DEFAULT_MSG_RANGE,
    RFX_MSG_FOO_START,
#include "RfxFooMessageId.h"
    RFX_MSG_FOO_END = RFX_MSG_FOO_START + DEFAULT_MSG_RANGE,
    RFX_MSG_SIM_START,
#include "RfxSimMessageId.h"
    RFX_MSG_SIM_END = RFX_MSG_SIM_START + DEFAULT_MSG_RANGE,
    RFX_MSG_NW_START,
#include "RfxNwMessageId.h"
    RFX_MSG_NW_END = RFX_MSG_NW_START + DEFAULT_MSG_RANGE,
    RFX_MSG_CC_START,
#include "RfxCallControlMessageId.h"
    RFX_MSG_CC_END = RFX_MSG_CC_START + DEFAULT_MSG_RANGE,
    RFX_MSG_CAPABILITY_SWITCH_START,
#include "RfxCapabilitySwitchMessageId.h"
    RFX_MSG_CAPABILITY_SWITCH_END = RFX_MSG_CAPABILITY_SWITCH_START + DEFAULT_MSG_RANGE,
    RFX_MSG_DATA_START,
#include "RfxDataMessageId.h"
    RFX_MSG_DATA_END = RFX_MSG_DATA_START + DEFAULT_MSG_RANGE,
    RFX_MSG_IMS_START,
#include "RfxImsMessageId.h"
    RFX_MSG_IMS_END = RFX_MSG_IMS_START + DEFAULT_MSG_RANGE,
    RFX_MSG_ATCI_START,
#include "RfxAtciMessageId.h"
    RFX_MSG_ATCI_END = RFX_MSG_ATCI_START + DEFAULT_MSG_RANGE,
    RFX_MSG_SS_START,
#include "RfxSuppServMessageId.h"
    RFX_MSG_SS_END = RFX_MSG_SS_START + DEFAULT_MSG_RANGE,
    RFX_MSG_GBA_START,
#include "RfxGbaMessageId.h"
    RFX_MSG_GBA_END = RFX_MSG_GBA_START + DEFAULT_MSG_RANGE,
    RFX_MSG_OEM_START,
#include "RfxOemMessageId.h"
    RFX_MSG_OEM_END = RFX_MSG_OEM_START + DEFAULT_MSG_RANGE,
    // M: eMBMS
    RFX_MSG_EMBMS_START,
#include "RfxEmbmsMessageId.h"
    RFX_MSG_EMBMS_END = RFX_MSG_EMBMS_START + DEFAULT_MSG_RANGE,
    RFX_MSG_POWER_START,
#include "RfxPowerMessageId.h"
    RFX_MSG_POWER_END = RFX_MSG_POWER_START + DEFAULT_MSG_RANGE,
    RFX_MSG_MODE_CONTROLLER_START,
#include "RfxModeControllerMessageId.h"
    RFX_MSG_MODE_CONTROLLER_END = RFX_MSG_MODE_CONTROLLER_START + DEFAULT_MSG_RANGE,
    RFX_MSG_SMS_START,
#include "RfxGsmSmsMessageId.h"
#include "RfxImsSmsMessageId.h"
    RFX_MSG_SMS_END = RFX_MSG_SMS_START + DEFAULT_MSG_RANGE,
    RFX_MSG_CDMA_SMS_START,
#include "RfxCdmaSmsMessageId.h"
    RFX_MSG_CDMA_SMS_END = RFX_MSG_CDMA_SMS_START + DEFAULT_MSG_RANGE,
    RFX_MSG_PHB_START,
#include "RfxPhbMessageId.h"
    RFX_MSG_PHB_END = RFX_MSG_PHB_START + DEFAULT_MSG_RANGE,
    RDX_MSG_WORLD_MODE_START,
#include "RfxWpMessageId.h"
    RFX_MSG_WORLD_MODE_END = RDX_MSG_WORLD_MODE_START + DEFAULT_MSG_RANGE,
    RFX_MSG_VT_START,
#include "RfxVtMessageId.h"
    RFX_MSG_VT_END = RFX_MSG_VT_START + DEFAULT_MSG_RANGE,
    RFX_MSG_CAT_START,
#include "RfxCatMessageId.h"
    RFX_MSG_CAT_END = RFX_MSG_CAT_START + DEFAULT_MSG_RANGE,
    RFX_MSG_MWI_START,
#include "RfxMwisMessageId.h"
    RFX_MSG_MWI_END = RFX_MSG_MWI_START + DEFAULT_MSG_RANGE,
    RFX_MSG_AGPS_START,
#include "RfxAgpsMessageId.h"
    RFX_MSG_AGPS_END = RFX_MSG_AGPS_START + DEFAULT_MSG_RANGE,
    RDX_MSG_PHB_SIM_IO_START,
#include "RfxPhbSimIoMessageId.h"
    RFX_MSG_PHB_SIM_IO_END = RDX_MSG_PHB_SIM_IO_START + DEFAULT_MSG_RANGE,
    RFX_MSG_RCS_START,
#include "RfxRcsMessageId.h"
    RFX_MSG_RCS_END = RFX_MSG_RCS_START + DEFAULT_MSG_RANGE,

/* Please add your default value above this line */
#ifdef MTK_OPERATOR_ADDON
    RFX_MSG_OP_ID_START,
#include "RfxMessageOpId.h"
    RFX_MSG_OP_END = RFX_MSG_OP_ID_START + DEFAULT_MSG_RANGE,
#endif
    RFX_MESSAGE_ID_END
} RFX_MESSAGE_ID;

#endif
