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

#ifndef __RFX_PROPERTIES_H__
#define __RFX_PROPERTIES_H__

/*****************************************************************************
 * Include
 *****************************************************************************/

#include <sys/cdefs.h>
#include <stddef.h>
#include <stdint.h>
#include <mtk_properties.h>

#define RFX_PROPERTY_VALUE_MAX MTK_PROPERTY_VALUE_MAX

#ifdef __cplusplus
extern "C" {
#endif

extern int rfx_property_get(const char* key, char* value, const char* default_value);

extern int rfx_property_set(const char* key, const char* value);

extern void rfx_property_release_info();

#ifdef __cplusplus
}  // extern "C"
#endif

#endif /* __RFX_PROPERTIES_H__ */
