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

#include "RfxCapabilityData.h"
#include <telephony/mtk_ril.h>

RFX_IMPLEMENT_DATA_CLASS(RfxCapabilityData);

#define RFX_LOG_TAG "RfxCapabilityData"

RfxCapabilityData::RfxCapabilityData(void* data, int length) : RfxBaseData(data, length) {
    if (data != NULL) {
        RIL_Capability* pOriginal = (RIL_Capability*)data;
        RIL_Capability* pData = (RIL_Capability*)calloc(1, sizeof(RIL_Capability));
        if (pData == NULL) {
            RFX_LOG_E(RFX_LOG_TAG, "OOM");
            return;
        }

        pData->uid = pOriginal->uid;
        pData->toActive = pOriginal->toActive;
        if (pOriginal->id != NULL) {
            asprintf(&pData->id, "%s", pOriginal->id);
        }

        m_data = pData;
        m_length = length;
    }
}

RfxCapabilityData::~RfxCapabilityData() {
    // free memory
    if (m_data != NULL) {
        RIL_Capability* pData = (RIL_Capability*)m_data;
        if (pData->id != NULL) {
            free(pData->id);
        }
    }
}
