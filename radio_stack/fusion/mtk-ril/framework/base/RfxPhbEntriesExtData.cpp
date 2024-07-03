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

#include "RfxPhbEntriesExtData.h"
#include "RfxLog.h"
#include <string.h>

RFX_IMPLEMENT_DATA_CLASS(RfxPhbEntriesExtData);

RfxPhbEntriesExtData::RfxPhbEntriesExtData() : RfxBaseData(NULL, 0) {}

RfxPhbEntriesExtData::RfxPhbEntriesExtData(void* data, int length) : RfxBaseData(data, length) {
    m_length = length;
    int countEntries = length / sizeof(RIL_PHB_ENTRY*);
    copyPhbEntries((RIL_PHB_ENTRY**)data, countEntries);
}

RfxPhbEntriesExtData::RfxPhbEntriesExtData(RIL_PHB_ENTRY** data, int countEntries)
    : RfxBaseData(data, countEntries * sizeof(RIL_PHB_ENTRY*)) {
    m_length = countEntries * sizeof(RIL_PHB_ENTRY*);
    copyPhbEntries(data, countEntries);
}

void RfxPhbEntriesExtData::copyPhbEntries(RIL_PHB_ENTRY** data, int countEntries) {
    if (data != NULL) {
        RIL_PHB_ENTRY** pInput = (RIL_PHB_ENTRY**)data;
        RIL_PHB_ENTRY** pData = (RIL_PHB_ENTRY**)calloc(1, sizeof(RIL_PHB_ENTRY*) * countEntries);
        RFX_ASSERT(pData != NULL);
        for (int i = 0; i < countEntries; i++) {
            pData[i] = (RIL_PHB_ENTRY*)calloc(1, sizeof(RIL_PHB_ENTRY));
            RFX_ASSERT(pData[i] != NULL);
            pData[i]->index = pInput[i]->index;
            if (pInput[i]->number != NULL) {
                pData[i]->number = strdup(pInput[i]->number);
            }
            pData[i]->type = pInput[i]->type;
            if (pInput[i]->text != NULL) {
                pData[i]->text = strdup(pInput[i]->text);
            }
            pData[i]->hidden = pInput[i]->hidden;
            if (pInput[i]->group != NULL) {
                pData[i]->group = strdup(pInput[i]->group);
            }
            if (pInput[i]->adnumber != NULL) {
                pData[i]->adnumber = strdup(pInput[i]->adnumber);
            }
            pData[i]->adtype = pInput[i]->adtype;
            if (pInput[i]->secondtext != NULL) {
                pData[i]->secondtext = strdup(pInput[i]->secondtext);
            }
            if (pInput[i]->email != NULL) {
                pData[i]->email = strdup(pInput[i]->email);
            }
        }

        m_data = (void*)pData;
    }
}

RfxPhbEntriesExtData::~RfxPhbEntriesExtData() {
    // free
    if (m_data != NULL) {
        RIL_PHB_ENTRY** pTmp = (RIL_PHB_ENTRY**)m_data;
        int countEntries = m_length / sizeof(RIL_PHB_ENTRY*);
        for (int i = 0; i < countEntries; i++) {
            if (pTmp[i] != NULL) {
                if (pTmp[i]->number != NULL) {
                    free(pTmp[i]->number);
                }
                if (pTmp[i]->text != NULL) {
                    free(pTmp[i]->text);
                }
                if (pTmp[i]->group != NULL) {
                    free(pTmp[i]->group);
                }
                if (pTmp[i]->adnumber != NULL) {
                    free(pTmp[i]->adnumber);
                }
                if (pTmp[i]->secondtext != NULL) {
                    free(pTmp[i]->secondtext);
                }
                if (pTmp[i]->email != NULL) {
                    free(pTmp[i]->email);
                }
                free(pTmp[i]);
            }
        }
        free(m_data);
    }
}
