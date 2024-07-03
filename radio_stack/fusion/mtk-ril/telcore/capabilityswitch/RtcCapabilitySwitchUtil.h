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

#ifndef __RTC_CAPABILITY_SWITCH_UTIL_H__
#define __RTC_CAPABILITY_SWITCH_UTIL_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include <telephony/mtk_ril.h>
#include <string>
#include "RfxStringData.h"
#include "RfxStringsData.h"

/*****************************************************************************
 * Define
 *****************************************************************************/

#define RAF_GSM_GROUP (RAF_GSM | RAF_GPRS | RAF_EDGE)
#define RAF_HS_GROUP (RAF_HSUPA | RAF_HSDPA | RAF_HSPA | RAF_HSPAP)
#define RAF_CDMA_GROUP (RAF_IS95A | RAF_IS95B | RAF_1xRTT)
#define RAF_EVDO_GROUP (RAF_EVDO_0 | RAF_EVDO_A | RAF_EVDO_B | RAF_EHRPD)
#define RAF_WCDMA_GROUP (RAF_HS_GROUP | RAF_UMTS | RAF_TD_SCDMA)
#define RAF_LTE_GROUP (RAF_LTE | RAF_LTE_CA)

/*****************************************************************************
 * Class RtcCapabilitySwitchUtil
 *****************************************************************************/

class RtcCapabilitySwitchUtil {
  public:
    static int getRafFromNetworkType(int type);
    static int getNetworkTypeFromRaf(int raf);
    static bool isDisableC2kCapability();
    static bool isDssNoResetSupport();
    static bool isOp01Sim(const String8& imsi);
    static bool isOp02Sim(const String8& imsi);
    static bool isOp09Sim(const String8& imsi);
    static bool isOp18Sim(const String8& imsi);
    static bool isHvolteSupport();
    static bool isSimSwitchEnabled();

  private:
    static int getAdjustedRaf(int raf);
};

#endif /* __RTC_CAPABILITY_SWITCH_UTIL_H__ */
