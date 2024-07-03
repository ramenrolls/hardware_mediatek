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

#ifndef __RMC_CAPABILITY_SWITCH_UTIL_H__
#define __RMC_CAPABILITY_SWITCH_UTIL_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include <telephony/mtk_ril.h>
#include "RfxMclMessage.h"

/*****************************************************************************
 * Define
 *****************************************************************************/

#define RAF_GSM_GROUP (RAF_GSM | RAF_GPRS | RAF_EDGE)
#define RAF_HS_GROUP (RAF_HSUPA | RAF_HSDPA | RAF_HSPA | RAF_HSPAP)
#define RAF_CDMA_GROUP (RAF_IS95A | RAF_IS95B | RAF_1xRTT)
#define RAF_EVDO_GROUP (RAF_EVDO_0 | RAF_EVDO_A | RAF_EVDO_B | RAF_EHRPD)
#define RAF_WCDMA_GROUP (RAF_HS_GROUP | RAF_UMTS | RAF_TD_SCDMA)

typedef enum {
    RADIO_GENERATION_2G = 0,
    RADIO_GENERATION_3G = 1,
    RADIO_GENERATION_4G = 2
} Rmc_RadioGenerationType;

/*****************************************************************************
 * Class RmcCapabilitySwitchUtil
 *****************************************************************************/

class RmcCapabilitySwitchUtil {
  public:
    static int getMaxRadioGeneration(int radio_capability);
    static int getMajorSim();
    static int isDisableCapabilitySwitch();
    static int getAdjustedRaf(int raf);
    static bool isMessageBeforeCapabilitySwitch(const sp<RfxMclMessage>& msg);
    static bool isDssNoResetSupport();
    static bool isDisableC2kCapability();
};

#endif /* __RMC_CAPABILITY_SWITCH_UTIL_H__ */
