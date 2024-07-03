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

#ifndef IMS_CONFIG_DEF
#define IMS_CONFIG_DEF

#include <string>
#include "telephony/mtk_ims_config.h"

/*******************************************************************/

/*******************************************************************/

struct ConfigValue {
    int configId;
    int unit;
    std::string provsionValue;
};

struct ConfigSettings {
    int configId;
    ImsCfgMimeType mimeType;
    std::string provsionStr;
};

enum Operator {
    OP_NONE = -1,
    OP_DEFAULT = 0,
    OP_06 = 6,
    OP_08 = 8,
    OP_09 = 9,
    OP_12 = 12,
    OP_236 = 236,
};

#endif /* IMS_CONFIG_DEF */
