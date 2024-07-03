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

#include "RfxPhbEntriesData.h"
#include "RfxLog.h"
#include <string.h>

RFX_IMPLEMENT_DATA_CLASS(RfxPhbEntriesData);

RfxPhbEntriesData::RfxPhbEntriesData() : RfxBaseData(NULL, 0) {}

RfxPhbEntriesData::RfxPhbEntriesData(void* data, int length) : RfxBaseData(data, length) {
    m_length = length;
    int countEntries = length / sizeof(RIL_PhbEntryStructure*);
    copyPhbEntries((RIL_PhbEntryStructure**)data, countEntries);
}

RfxPhbEntriesData::RfxPhbEntriesData(RIL_PhbEntryStructure** data, int countEntries)
    : RfxBaseData(data, countEntries * sizeof(RIL_PhbEntryStructure*)) {
    m_length = countEntries * sizeof(RIL_PhbEntryStructure*);
    copyPhbEntries(data, countEntries);
}

void RfxPhbEntriesData::copyPhbEntries(RIL_PhbEntryStructure** data, int countEntries) {
    if (data != NULL) {
        RIL_PhbEntryStructure** pInput = (RIL_PhbEntryStructure**)data;
        RIL_PhbEntryStructure** pData =
                (RIL_PhbEntryStructure**)calloc(1, sizeof(RIL_PhbEntryStructure*) * countEntries);
        RFX_ASSERT(pData != NULL);
        for (int i = 0; i < countEntries; i++) {
            pData[i] = (RIL_PhbEntryStructure*)calloc(1, sizeof(RIL_PhbEntryStructure));
            RFX_ASSERT(pData[i] != NULL);
            pData[i]->type = pInput[i]->type;
            pData[i]->index = pInput[i]->index;
            if (pInput[i]->number != NULL) {
                pData[i]->number = strdup(pInput[i]->number);
            }
            pData[i]->ton = pInput[i]->ton;
            if (pInput[i]->alphaId != NULL) {
                pData[i]->alphaId = strdup(pInput[i]->alphaId);
            }
        }

        m_data = (void*)pData;
    }
}

RfxPhbEntriesData::~RfxPhbEntriesData() {
    // free
    if (m_data != NULL) {
        RIL_PhbEntryStructure** pTmp = (RIL_PhbEntryStructure**)m_data;
        int countEntries = m_length / sizeof(RIL_PhbEntryStructure*);
        for (int i = 0; i < countEntries; i++) {
            if (pTmp[i] != NULL) {
                if (pTmp[i]->number != NULL) {
                    free(pTmp[i]->number);
                }
                if (pTmp[i]->alphaId != NULL) {
                    free(pTmp[i]->alphaId);
                }
                free(pTmp[i]);
            }
        }
        free(m_data);
    }
}
