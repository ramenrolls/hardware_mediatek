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

#ifndef __MTK_LIBCONFIG_H
#define __MTK_LIBCONFIG_H

#include <mtk_properties.h>
#include <mtkconfigdef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define NUM_OF_ELEMS(a) (sizeof(a) / sizeof(a)[0])

/**
 * FeatureConfigInfo: carry the value from featrueconfig.h.
 */
typedef struct FeatureConfigInfo {
    int requestNumber;
    char* value;
} FeatureConfigInfo;

/**
 * FeatureValue: carry the value back to module.
 *
 *     For module initialize, the size of value can be determined
 *
 *     So mtkconfig can block the condition that
 *     if config value lenth > the returning array value
 */
typedef struct FeatureValue {
    char value[MTK_PROPERTY_VALUE_MAX];
} FeatureValue;

int mtkGetFeaturePorting(ConfigDef msg, FeatureValue* fv);
int mtkSetFeaturePorting(ConfigDef msg, FeatureValue* fv);

#ifdef __cplusplus
}
#endif
#endif /* __MTK_LIBCONFIG_H */
