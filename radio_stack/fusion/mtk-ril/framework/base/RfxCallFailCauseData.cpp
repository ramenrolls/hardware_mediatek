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

#include "RfxCallFailCauseData.h"

RFX_IMPLEMENT_DATA_CLASS(RfxCallFailCauseData);

RfxCallFailCauseData::RfxCallFailCauseData(void* data, int length) : RfxBaseData(data, length) {
    if (data == NULL) {
        m_data = NULL;
        return;
    }

    m_length = length;
    RIL_LastCallFailCauseInfo* pData = (RIL_LastCallFailCauseInfo*)data;
    RIL_LastCallFailCauseInfo* dupData;

    // Copy cause code
    dupData = (RIL_LastCallFailCauseInfo*)calloc(1, sizeof(RIL_LastCallFailCauseInfo));
    RFX_ASSERT(dupData != NULL);
    memset(dupData, 0x00, sizeof(RIL_LastCallFailCauseInfo));
    dupData->cause_code = pData->cause_code;

    // Copy vendor cause
    if (pData->vendor_cause != NULL) {
        int strLen = strlen(pData->vendor_cause);
        // Allocate and zero-initialize array
        dupData->vendor_cause = (char*)calloc(strLen + 1, sizeof(char));
        RFX_ASSERT(dupData->vendor_cause != NULL);
        strncpy(dupData->vendor_cause, pData->vendor_cause, strLen);
    }

    m_data = dupData;
}

RfxCallFailCauseData::~RfxCallFailCauseData() {
    // free
    RIL_LastCallFailCauseInfo* tmp = (RIL_LastCallFailCauseInfo*)m_data;
    if (tmp != NULL) {
        if (tmp->vendor_cause != NULL) {
            free(tmp->vendor_cause);
        }
        free(tmp);
    }
    m_data = NULL;
}
