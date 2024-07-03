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
#include "rfx_properties.h"
#include "RfxRilUtils.h"
#include <Vector.h>
#include <RfxLog.h>
#include <string.h>
#include "utils/Mutex.h"

using ::android::Mutex;
using ::android::String8;
using ::android::Vector;

/*****************************************************************************
 * Implementation
 *****************************************************************************/

Mutex s_gt_data_mutex;

class GTDataEntry {
  public:
    GTDataEntry(const char* _key, const char* _value) : key(_key), value(_value) {}

    String8 key;
    String8 value;
};

static Vector<GTDataEntry*>* s_gt_property_info = new Vector<GTDataEntry*>();

static bool rfx_get_value_from_GTPro(const char* key, char* value) {
    Mutex::Autolock autoLock(s_gt_data_mutex);
    Vector<GTDataEntry*>::iterator it;
    if (key == NULL || s_gt_property_info == NULL) {
        return false;
    }
    for (it = s_gt_property_info->begin(); it != s_gt_property_info->end(); it++) {
        if ((*it)->key == String8(key)) {
            strncpy(value, ((*it)->value).string(), strlen(((*it)->value).string()));
            return true;
        }
    }
    return false;
}

int rfx_property_get(const char* key, char* value, const char* default_value) {
    if (RfxRilUtils::getRilRunMode() == RilRunMode::RIL_RUN_MODE_MOCK) {
        if (!rfx_get_value_from_GTPro(key, value)) {
            if (mtk_property_get(key, value, default_value) > 0) {
                rfx_property_set(key, value);
            } else {
                strncpy(value, default_value, strlen(default_value));
            }
        }
        return 1;
    } else {
        return mtk_property_get(key, value, default_value);
    }
}

int rfx_property_set(const char* key, const char* value) {
    if (RfxRilUtils::getRilRunMode() == RilRunMode::RIL_RUN_MODE_MOCK) {
        Mutex::Autolock autoLock(s_gt_data_mutex);
        if (s_gt_property_info != NULL) {
            Vector<GTDataEntry*>::iterator it;
            for (it = s_gt_property_info->begin(); it != s_gt_property_info->end(); it++) {
                if ((*it)->key == String8(key)) {
                    ((*it)->value).setTo(value);
                    return 1;
                }
            }
            GTDataEntry* gtDataEntry = new GTDataEntry(key, value);
            s_gt_property_info->push_back(gtDataEntry);
            return 1;
        }
    } else {
        return mtk_property_set(key, value);
    }
    return false;
}

void rfx_property_release_info() {
    if (RfxRilUtils::getRilRunMode() == RilRunMode::RIL_RUN_MODE_MOCK) {
        Vector<GTDataEntry*>::iterator it;
        for (it = s_gt_property_info->begin(); it != s_gt_property_info->end();) {
            delete (*it);
            it = s_gt_property_info->erase(it);
        }
    }
}
