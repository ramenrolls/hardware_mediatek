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

#include "RfxQueryCapData.h"
#include <telephony/mtk_ril.h>
#include <mtk_log.h>

RFX_IMPLEMENT_DATA_CLASS(RfxQueryCapData);

#define RFX_LOG_TAG "RfxQueryCapData"

RfxQueryCapData::RfxQueryCapData(void* data, int length) : RfxBaseData(data, length) {
    if (data != NULL) {
        RIL_QueryCap* pOriginal = (RIL_QueryCap*)data;
        RIL_QueryCap* pData = (RIL_QueryCap*)calloc(1, sizeof(RIL_QueryCap));
        if (pData == NULL) {
            RFX_LOG_E(RFX_LOG_TAG, "OOM");
            return;
        }

        pData->id = pOriginal->id;
        if (pOriginal->name != NULL) {
            pData->name = strdup(pOriginal->name);
            if (pData->name == NULL) {
                mtkLogE(RFX_LOG_TAG, "OOM");
                return;
            }
        }

        m_data = (void*)pData;
        m_length = length;
    }
}

RfxQueryCapData::~RfxQueryCapData() {
    // free memory
    if (m_data != NULL) {
        RIL_QueryCap* pData = (RIL_QueryCap*)m_data;
        if (pData->name != NULL) {
            free(pData->name);
        }
        free(pData);
    }
}
