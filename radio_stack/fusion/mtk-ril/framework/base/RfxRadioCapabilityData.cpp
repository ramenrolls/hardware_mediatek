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

#include "RfxRadioCapabilityData.h"

RFX_IMPLEMENT_DATA_CLASS(RfxRadioCapabilityData);

RfxRadioCapabilityData::RfxRadioCapabilityData(void* data, int length) : RfxBaseData(data, length) {
    m_length = length;
    if (data == NULL) {
        m_data = NULL;
        return;
    }
    m_data = (RIL_RadioCapability*)calloc(1, sizeof(RIL_RadioCapability));
    if (m_data != NULL) {
        memcpy(m_data, data, sizeof(RIL_RadioCapability));
    }
}

RfxRadioCapabilityData::~RfxRadioCapabilityData() {
    // free
    if (m_data != NULL) {
        free(m_data);
    }
}
