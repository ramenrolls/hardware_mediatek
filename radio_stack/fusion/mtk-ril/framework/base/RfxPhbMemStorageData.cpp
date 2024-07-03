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

#include "RfxPhbMemStorageData.h"
#include "RfxLog.h"
#include <telephony/mtk_ril.h>
#include <string.h>

RFX_IMPLEMENT_DATA_CLASS(RfxPhbMemStorageData);

RfxPhbMemStorageData::RfxPhbMemStorageData(void* _data, int _length) : RfxBaseData(_data, _length) {
    if (_data != NULL) {
        RIL_PHB_MEM_STORAGE_RESPONSE* pInput = (RIL_PHB_MEM_STORAGE_RESPONSE*)_data;
        RIL_PHB_MEM_STORAGE_RESPONSE* pData =
                (RIL_PHB_MEM_STORAGE_RESPONSE*)calloc(1, sizeof(RIL_PHB_MEM_STORAGE_RESPONSE));
        RFX_ASSERT(pData != NULL);
        if (pInput->storage != NULL) {
            pData->storage = strdup(pInput->storage);
        }

        pData->used = pInput->used;
        pData->total = pInput->total;

        m_data = (RIL_PHB_MEM_STORAGE_RESPONSE*)pData;
        m_length = _length;
    }
}

RfxPhbMemStorageData::~RfxPhbMemStorageData() {
    // free memory
    if (m_data != NULL) {
        RIL_PHB_MEM_STORAGE_RESPONSE* pData = (RIL_PHB_MEM_STORAGE_RESPONSE*)m_data;
        if (pData->storage != NULL) {
            free(pData->storage);
            pData->storage = NULL;
        }
        free(m_data);
        m_data = NULL;
    }
}
