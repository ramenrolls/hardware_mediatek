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

#include <stdlib.h>
#include <string.h>
#include <mtkconfig.h>
#include <mtk_log.h>
#include <mtk_properties.h>
#include <cutils/properties.h>

char* msgToConfigs(ConfigDef msg);
int mtkGetFeatureByOs(ConfigDef msg, char* value);
int mtkSetFeatureByOs(ConfigDef msg, char* value);
int mtkGetFeatureByDefault(ConfigDef msg, char* value);

static FeatureConfigInfo settings[] = {
#include "featureconfig.h"
};

int mtkGetFeaturePorting(ConfigDef msg, FeatureValue* fv) {
    // Need to decide which is the proper function for your devices.
    return mtkGetFeatureByOs(msg, fv->value);
    // return mtkGetFeatureByDefault(msg, fv->value);
}

int mtkSetFeaturePorting(ConfigDef msg, FeatureValue* fv) {
    return mtkSetFeatureByOs(msg, fv->value);
}

// Indicate that the OS needs to control the config settings itself. (e.g., Android)
// In OS Controlled, do NOT call mtkGetFeatureByDefault function.
int mtkGetFeatureByOs(ConfigDef msg, char* value) {
    switch (msg) {
        case CONFIG_SIM_MODE:
            return property_get("persist.radio.multisim.config", value, "dsds");
        case CONFIG_SS_MODE:
            strncpy(value, "1", 10);  // Android platform supports Supplementary Service by default
            return strlen(value);
        case CONFIG_SMS: {
            char sms_config[2] = "1";
            int len = strlen(sms_config);
            memcpy(value, sms_config, len);
            return len;
        }
        case CONFIG_CC: {
            char cc_config[2] = "1";
            int len = strlen(cc_config);
            memcpy(value, cc_config, len);
            return len;
        }
        case CONFIG_VOLTE:
            return property_get("persist.vendor.volte_support", value, "0");
        case CONFIG_VILTE:
            return property_get("persist.vendor.vilte_support", value, "0");
        case CONFIG_WFC:
            return property_get("persist.vendor.mtk_wfc_support", value, "0");
        case CONFIG_VIWIFI:
            return property_get("persist.vendor.viwifi_support", value, "0");
        case CONFIG_RCSUA:
            return property_get("persist.vendor.mtk_rcs_ua_support", value, "0");
        default:
            break;
    }
    return 0;
}

// Indicate that the OS needs to control the config settings itself. (e.g., Android)
int mtkSetFeatureByOs(ConfigDef msg, char* value) {
    switch (msg) {
        case CONFIG_SIM_MODE:
            return property_set("persist.radio.multisim.config", value);
        case CONFIG_VOLTE:
            return property_set("persist.vendor.volte_support", value);
        case CONFIG_VILTE:
            return property_set("persist.vendor.vilte_support", value);
        case CONFIG_WFC:
            return property_set("persist.vendor.mtk_wfc_support", value);
        case CONFIG_VIWIFI:
            return property_set("persist.vendor.viwifi_support", value);
        case CONFIG_RCSUA:
            return property_set("persist.vendor.mtk_rcs_ua_support", value);
        default:
            break;
    }
    return 0;
}

// Indicate that the OS does not need to handle the config settings. (e.g., IPCAM)
// The settings can be read from a header file(featureconfig.h).
int mtkGetFeatureByDefault(ConfigDef msg, char* value) {
    char* return_value = msgToConfigs(msg);
    int len = strlen(return_value);
    if (len >= MTK_PROPERTY_VALUE_MAX) {
        mtkLogD("mtkGetFeatureByDefault", "Length exceed the limit, return");
        return 0;
    }
    memcpy(value, return_value, len);
    return len;
}

char* msgToConfigs(ConfigDef msg) {
    int i;
    for (i = 0; i < (int)NUM_OF_ELEMS(settings); i++) {
        if (settings[i].requestNumber == msg) {
            return settings[i].value;
        }
    }
    return "";
}
