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
#include <ctype.h>
#include <unistd.h>

#include <cutils/properties.h>
#include <inttypes.h>
#include <mtk_properties.h>
#include <mtk_log.h>
static char* s_feature_properties[] = {
#include <mtkfeatureproperty.h>
};

#define NUM_ELEMS(a) (sizeof(a) / sizeof(a)[0])

#undef LOG_TAG
#define LOG_TAG "MTK_PROPERTY"

int mtk_property_set(const char* key, const char* value) {
    unsigned int i;

    for (i = 0; i < NUM_ELEMS(s_feature_properties); i++) {
        if (strcmp(s_feature_properties[i], key) == 0) {
            mtkLogE(LOG_TAG, "invalid operation, please use feature utils");
            return -1;
        }
    }
    return property_set(key, value);
}

int mtk_property_get(const char* key, char* value, const char* default_value) {
    unsigned int i;

    for (i = 0; i < NUM_ELEMS(s_feature_properties); i++) {
        if (strcmp(s_feature_properties[i], key) == 0) {
            mtkLogE(LOG_TAG, "invalid operation, please use feature utils");
            return -1;
        }
    }
    return property_get(key, value, default_value);
}
