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

#include <telephony/mtk_ril.h>
#include "RfxHardwareConfigData.h"

#define RFX_LOG_TAG "RfxHardwareConfigData"

RFX_IMPLEMENT_DATA_CLASS(RfxHardwareConfigData);

RfxHardwareConfigData::RfxHardwareConfigData(void* data, int length) : RfxBaseData(data, length) {
    m_length = length;

    int num = length / sizeof(RIL_HardwareConfig);

    m_data = (RIL_HardwareConfig*)calloc(num, sizeof(RIL_HardwareConfig));
    if (m_data == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "OOM");
        return;
    }
    memcpy(m_data, (RIL_HardwareConfig*)data, m_length);
}

RfxHardwareConfigData::~RfxHardwareConfigData() { free(m_data); }
