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

#include "RfxEmergencyNumberListData.h"

#define RFX_LOG_TAG "RfxEccListData"

RFX_IMPLEMENT_DATA_CLASS(RfxEmergencyNumberListData);

RfxEmergencyNumberListData::RfxEmergencyNumberListData(void* data, int length)
    : RfxBaseData(data, length) {
    m_length = length;
    copyEmergencyNumberListData((RIL_EmergencyNumber*)data);
}

void RfxEmergencyNumberListData::copyEmergencyNumberListData(RIL_EmergencyNumber* data) {
    int num = m_length / sizeof(RIL_EmergencyNumber);
    if (num == 0 || data == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "[%s] The number of responses is %d", __FUNCTION__, num);
        return;
    }
    RIL_EmergencyNumber* pData = (RIL_EmergencyNumber*)calloc(1, m_length);
    RFX_ASSERT(pData != NULL);
    for (int i = 0; i < num; i++) {
        asprintf(&(pData[i].number), "%s", data[i].number);
        asprintf(&(pData[i].mcc), "%s", data[i].mcc);
        asprintf(&(pData[i].mnc), "%s", data[i].mnc);
        pData[i].categories = data[i].categories;
        pData[i].urns = NULL;  // not used yet
        pData[i].sources = data[i].sources;
    }
    m_data = pData;
}

RfxEmergencyNumberListData::~RfxEmergencyNumberListData() {
    // free memory
    if (m_data != NULL) {
        RIL_EmergencyNumber* pData = (RIL_EmergencyNumber*)m_data;
        int num = m_length / sizeof(RIL_EmergencyNumber);
        for (int i = 0; i < num; i++) {
            FREEIF(pData[i].number);
            FREEIF(pData[i].mcc);
            FREEIF(pData[i].mnc);
        }
        free(m_data);
    }
    m_data = NULL;
    m_length = 0;
}
