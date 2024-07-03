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

#include "RfxPhbEntryData.h"
#include "RfxLog.h"
#include <string.h>

RFX_IMPLEMENT_DATA_CLASS(RfxPhbEntryData);

RfxPhbEntryData::RfxPhbEntryData(void* data, int length) : RfxBaseData(data, length) {
    if (data != NULL) {
        RIL_PhbEntryStructure* pInput = (RIL_PhbEntryStructure*)data;
        RIL_PhbEntryStructure* pData =
                (RIL_PhbEntryStructure*)calloc(1, sizeof(RIL_PhbEntryStructure));
        RFX_ASSERT(pData != NULL);
        pData->type = pInput->type;
        pData->index = pInput->index;
        if (pInput->number != NULL) {
            pData->number = strdup(pInput->number);
        }
        pData->ton = pInput->ton;
        if (pInput->alphaId != NULL) {
            pData->alphaId = strdup(pInput->alphaId);
        }

        m_data = (void*)pData;
        m_length = length;
    }
}

RfxPhbEntryData::~RfxPhbEntryData() {
    // free
    if (m_data != NULL) {
        RIL_PhbEntryStructure* pTmp = (RIL_PhbEntryStructure*)m_data;
        if (pTmp->number != NULL) {
            free(pTmp->number);
            pTmp->number = NULL;
        }
        if (pTmp->alphaId != NULL) {
            free(pTmp->alphaId);
            pTmp->alphaId = NULL;
        }
        free(m_data);
        m_data = NULL;
    }
}
