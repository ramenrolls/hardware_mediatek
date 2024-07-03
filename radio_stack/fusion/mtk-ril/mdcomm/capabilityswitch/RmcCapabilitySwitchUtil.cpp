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

/*****************************************************************************
 * Include
 *****************************************************************************/
#include <string.h>
#include "rfx_properties.h"
#include "RmcCapabilitySwitchUtil.h"
#include "RfxLog.h"
#include "RfxRilUtils.h"
#include "RfxIdToStringUtils.h"
#include "RfxMclMessage.h"
#include "RfxMclStatusManager.h"

#define RFX_LOG_TAG "RmcCapa"
/*****************************************************************************
 * Class RmcCapabilitySwitchUtil
 *****************************************************************************/

int RmcCapabilitySwitchUtil::getMaxRadioGeneration(int radio_capability) {
    RFX_LOG_D(RFX_LOG_TAG, "getMaxRadioGeneration, capability=%d", radio_capability);
    if (radio_capability & RAF_LTE) {
        return RADIO_GENERATION_4G;
    } else if (radio_capability & (RAF_WCDMA_GROUP | RAF_EVDO_GROUP)) {
        return RADIO_GENERATION_3G;
    } else {
        return RADIO_GENERATION_2G;
    }
}

int RmcCapabilitySwitchUtil::getMajorSim() { return RfxRilUtils::getMajorSim(); }

int RmcCapabilitySwitchUtil::isDisableCapabilitySwitch() {
    char tmp[RFX_PROPERTY_VALUE_MAX] = {0};
    rfx_property_get("ro.vendor.mtk_disable_cap_switch", tmp, "0");
    return atoi(tmp);
}

int RmcCapabilitySwitchUtil::getAdjustedRaf(int raf) {
    raf = ((RAF_GSM_GROUP & raf) > 0) ? (RAF_GSM_GROUP | raf) : raf;
    raf = ((RAF_WCDMA_GROUP & raf) > 0) ? (RAF_WCDMA_GROUP | raf) : raf;
    raf = ((RAF_CDMA_GROUP & raf) > 0) ? (RAF_CDMA_GROUP | raf) : raf;
    raf = ((RAF_EVDO_GROUP & raf) > 0) ? (RAF_EVDO_GROUP | raf) : raf;

    return raf;
}

bool RmcCapabilitySwitchUtil::isMessageBeforeCapabilitySwitch(const sp<RfxMclMessage>& msg) {
    if (RfxMclStatusManager::getNonSlotMclStatusManager()->getIntValue(
                RFX_STATUS_KEY_MAIN_CAPABILITY_SLOT, 0) == msg->getMainProtocolSlotId()) {
        RFX_LOG_I(RFX_LOG_TAG, "%s isn't msg before capability switch",
                  RFX_ID_TO_STR(msg->getId()));
        return false;
    }
    RFX_LOG_I(RFX_LOG_TAG, "%s is msg before capability switch", RFX_ID_TO_STR(msg->getId()));
    return true;
}

bool RmcCapabilitySwitchUtil::isDssNoResetSupport() {
    char tmp[RFX_PROPERTY_VALUE_MAX] = {0};
    rfx_property_get("vendor.ril.simswitch.no_reset_support", tmp, "0");
    if (strcmp(tmp, "1") == 0) {
        return true;
    }
    return false;
}

bool RmcCapabilitySwitchUtil::isDisableC2kCapability() {
    char tmp[RFX_PROPERTY_VALUE_MAX] = {0};
    rfx_property_get("persist.vendor.radio.disable_c2k_cap", tmp, "0");
    if (strcmp(tmp, "1") == 0) {
        RFX_LOG_I(RFX_LOG_TAG, "isDisableC2kCapability == true");
        return true;
    }
    return false;
}
