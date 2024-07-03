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

#include "ImsCustomizedUtils.h"
#include "rfx_properties.h"
#include <string>
#include <string.h>
#include "RfxRilUtils.h"

/** Customized function - start **/
bool ImsCustomizedUtils::isSupportCustomizedUserAgent() {
    // default value: false, change to true if you would like to enable set user agent.
    return false;
}

String8 ImsCustomizedUtils::getCustomizedUserAgent(String8 format, int slotId) {
    // TODO: Make customized User-Agent by FormatId
    // Format value is came from RFX_STATUS_KEY_CUSTOMIZED_USER_AGENT_FORMAT by carrier config
    // mechanism. Please set the preferred format by MCC/MNC.

    // Carrier config file path is as follow:
    // vendor\mediatek\proprietary\hardware\ril\fusion\libcarrierconfig\config_data\carrier_config_mcc_mnc.h
    // For an example, if you would like set China Mobile SIM to using format "1" parsing rule,
    // then you need to add { RFX_STATUS_KEY_CUSTOMIZED_USER_AGENT_FORMAT, "1"}, into
    // vendor\mediatek\proprietary\hardware\ril\fusion\libcarrierconfig\config_data\carrier_config_460_00.h

    // The parsing rule by format could be customized here.
    // This is only an example code.
    String8 userAgent("XXXOEM");
    String8 cust_prop_key1("ro.aaa.bbb");
    String8 cust_prop_key2("ro.build.version.incremental");
    char cust_prop_value1[RFX_PROPERTY_VALUE_MAX] = {0};
    char cust_prop_value2[RFX_PROPERTY_VALUE_MAX] = {0};

    RfxRilUtils::printLog(DEBUG, String8("ImsCustomizedUtils"),
                          String8::format("getCustomizedUserAgent(), format = %s", format.string()),
                          slotId);

    rfx_property_get(cust_prop_key1.string(), cust_prop_value1, "");
    RfxRilUtils::printLog(DEBUG, String8("ImsCustomizedUtils"),
                          String8::format("getCustomizedUserAgent(), format = %s", format.string()),
                          slotId);

    rfx_property_get(cust_prop_key2.string(), cust_prop_value2, "");
    RfxRilUtils::printLog(DEBUG, String8("ImsCustomizedUtils"),
                          String8::format("getCustomizedUserAgent(), [%s]= %s",
                                          cust_prop_key2.string(), cust_prop_value2),
                          slotId);

    if (String8("1") == format.string()) {
        // AIS
        userAgent.append(String8::format("-%s SIP/2.0", cust_prop_value1));
    } else if (String8("2") == format.string()) {
        // DT & ee-gb && H3G-GB && SFR
        userAgent.append(String8::format("-%s%s SIP/2.0", cust_prop_value1, cust_prop_value2));
    } else if (String8("3") == format.string()) {
        // KDDI
        userAgent.append(String8::format("-%s", cust_prop_value1));
    } else {
        // No matched
        RfxRilUtils::printLog(DEBUG, String8::format("ImsCustomizedUtils"),
                              String8::format("getCustomizedUserAgent(): not match any format"),
                              slotId);
    }

    RfxRilUtils::printLog(DEBUG, String8::format("ImsCustomizedUtils"),
                          String8::format("getCustomizedUserAgent(): %s", userAgent.string()),
                          slotId);

    return userAgent;
}
/** Customized function - end **/
