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

#include "RfxSsrcData.h"

RFX_IMPLEMENT_DATA_CLASS(RfxSsrcData);

RfxSsrcData::RfxSsrcData(void* data, int length) : RfxBaseData(data, length) {
    RIL_SignalStrength_Reporting_Criteria* pTmp = (RIL_SignalStrength_Reporting_Criteria*)data;
    RIL_SignalStrength_Reporting_Criteria* pData = (RIL_SignalStrength_Reporting_Criteria*)calloc(
            1, sizeof(RIL_SignalStrength_Reporting_Criteria));

    if (pData != NULL) {
        memcpy(pData, pTmp, sizeof(RIL_SignalStrength_Reporting_Criteria));
        m_data = pData;
        m_length = length;
    }
}

RfxSsrcData::~RfxSsrcData() {
    if (m_data != NULL) free(m_data);
    m_data = NULL;
}
