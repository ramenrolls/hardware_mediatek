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

#ifndef __MTK_LIBMTKCONFIGUTILS_H
#define __MTK_LIBMTKCONFIGUTILS_H

#include <mtk_properties.h>
#include <mtkconfig.h>

#ifdef __cplusplus
extern "C" {
#endif

int mtkGetFeature(ConfigDef msg, FeatureValue* fv);
int mtkSetFeature(ConfigDef msg, FeatureValue* fv);

#ifdef __cplusplus
}
#endif

#endif /* __MTK_LIBMTKCONFIGUTILS_H */
