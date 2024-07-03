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

#include "RfxPhbEntryExtData.h"
#include "RfxLog.h"
#include <string.h>

RFX_IMPLEMENT_DATA_CLASS(RfxPhbEntryExtData);

RfxPhbEntryExtData::RfxPhbEntryExtData(void* data, int length) : RfxBaseData(data, length) {
    if (data != NULL) {
        RIL_PHB_ENTRY* pInput = (RIL_PHB_ENTRY*)data;
        RIL_PHB_ENTRY* pData = (RIL_PHB_ENTRY*)calloc(1, sizeof(RIL_PHB_ENTRY));
        RFX_ASSERT(pData != NULL);
        pData->index = pInput->index;
        if (pInput->number != NULL) {
            pData->number = strdup(pInput->number);
        }
        pData->type = pInput->type;
        if (pInput->text != NULL) {
            pData->text = strdup(pInput->text);
        }
        pData->hidden = pInput->hidden;
        if (pInput->group != NULL) {
            pData->group = strdup(pInput->group);
        }
        if (pInput->adnumber != NULL) {
            pData->adnumber = strdup(pInput->adnumber);
        }
        pData->adtype = pInput->adtype;
        if (pInput->secondtext != NULL) {
            pData->secondtext = strdup(pInput->secondtext);
        }
        if (pInput->email != NULL) {
            pData->email = strdup(pInput->email);
        }

        m_data = (void*)pData;
        m_length = length;
    }
}

RfxPhbEntryExtData::~RfxPhbEntryExtData() {
    // free
    if (m_data != NULL) {
        RIL_PHB_ENTRY* pTmp = (RIL_PHB_ENTRY*)m_data;
        if (pTmp != NULL) {
            if (pTmp->number != NULL) {
                free(pTmp->number);
                pTmp->number = NULL;
            }
            if (pTmp->text != NULL) {
                free(pTmp->text);
                pTmp->text = NULL;
            }
            if (pTmp->group != NULL) {
                free(pTmp->group);
                pTmp->group = NULL;
            }
            if (pTmp->adnumber != NULL) {
                free(pTmp->adnumber);
                pTmp->adnumber = NULL;
            }
            if (pTmp->secondtext != NULL) {
                free(pTmp->secondtext);
                pTmp->secondtext = NULL;
            }
            if (pTmp->email != NULL) {
                free(pTmp->email);
                pTmp->email = NULL;
            }
        }
        free(m_data);
        m_data = NULL;
    }
}
