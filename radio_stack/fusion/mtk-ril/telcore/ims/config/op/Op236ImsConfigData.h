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

#ifndef RIL_OP236IMSCONFIGDATA_H
#define RIL_OP236IMSCONFIGDATA_H

#define CONFIG_VALUE(x, y, z) {ImsConfigItem::x, y, z},

CONFIG_VALUE(IMS_CONFIG_VOICE_DOMAIN_PREFERENCE, IMS_CONFIG_UNIT_NONE, "1")
#endif  // RIL_OP236IMSCONFIGDATA_H
