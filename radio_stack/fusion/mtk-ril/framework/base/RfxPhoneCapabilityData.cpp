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

#include "RfxBaseData.h"
#include <telephony/ril.h>
#include "RfxPhoneCapabilityData.h"

RFX_IMPLEMENT_DATA_CLASS(RfxPhoneCapabilityData);

RfxPhoneCapabilityData::RfxPhoneCapabilityData(void* data, int length) : RfxBaseData(data, length) {
    m_length = length;
    m_data = NULL;
    if (data != NULL) {
        RIL_PhoneCapability* dupData;
        RIL_PhoneCapability* srcData = (RIL_PhoneCapability*)data;

        dupData = (RIL_PhoneCapability*)calloc(1, sizeof(RIL_PhoneCapability));
        RFX_ASSERT(dupData != NULL);
        memcpy(dupData, srcData, sizeof(RIL_PhoneCapability));
        m_data = dupData;
    }
}

RfxPhoneCapabilityData::~RfxPhoneCapabilityData() {
    RIL_PhoneCapability* pCur = (RIL_PhoneCapability*)m_data;

    if (m_data != NULL) {
        free(pCur);
        m_data = NULL;
    }
}
