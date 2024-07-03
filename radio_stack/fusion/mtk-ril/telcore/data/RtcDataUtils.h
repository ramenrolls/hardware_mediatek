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

#ifndef __RTC_DATA_UTILS_H__
#define __RTC_DATA_UTILS_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "rfx_properties.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OPERATOR_OP12 "OP12"
#define SUPPORT_TEMPORARY_DISABLE_IMS 0

/*****************************************************************************
 * Class RtcDataUtils
 *****************************************************************************/

class RtcDataUtils {
  public:
    RtcDataUtils() {}
    virtual ~RtcDataUtils() {}
    static bool isOp12Support();
    static bool isSupportTemporaryDisableIms();
    static bool isSupportTempDataSwitchFromOem();
    static bool isPreferredDataMode();
};

inline bool RtcDataUtils::isOp12Support() {
    bool ret = false;
    char optr_value[RFX_PROPERTY_VALUE_MAX] = {0};
    rfx_property_get("persist.vendor.operator.optr", optr_value, "0");
    if (strcmp(optr_value, OPERATOR_OP12) == 0) {
        ret = true;
    }
    return ret;
}

inline bool RtcDataUtils::isSupportTemporaryDisableIms() {
    bool ret = false;
    if (SUPPORT_TEMPORARY_DISABLE_IMS) {
        ret = true;
    }
    return ret;
}

// Customized data incall: Switch default data to voice call sim
// It will be enable ONLY for platform which support this feature by:
// 1. Java framework will switch default data to voice call sim through setDefaultData
// 2. When call start, it will send allow data to voice call sim without disallow original data SIM
// 3. When call end, it will send disallow data to voice call sim without allow data on original
//    data SIM
// So RILD need to handle this scenario specially:
// 1. Support error handling for allow data on both slot (disallow for one slot then allow
//    data on another).
// 2. Check call state and set temp data switch state for special handling.
// 3. Support restore data allow state and data connection when call end.
inline bool RtcDataUtils::isSupportTempDataSwitchFromOem() {
    char support_value[RFX_PROPERTY_VALUE_MAX] = {0};
    rfx_property_get("persist.vendor.radio.data_incall_ext", support_value, "0");
    int value = atoi(support_value);
    return value == 1;
}

inline bool RtcDataUtils::isPreferredDataMode() {
    char preferredDataMode[RFX_PROPERTY_VALUE_MAX] = {0};
    rfx_property_get("vendor.ril.data.preferred_data_mode", preferredDataMode, "0");
    return atoi(preferredDataMode) == 1 ? true : false;
}

#endif /* __RTC_DATA_UTILS_H__ */
