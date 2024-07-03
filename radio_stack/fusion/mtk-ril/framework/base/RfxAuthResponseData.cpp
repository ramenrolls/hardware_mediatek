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

#include "RfxAuthResponseData.h"
#include <telephony/mtk_ril.h>

RFX_IMPLEMENT_DATA_CLASS(RfxAuthResponseData);

#define RFX_LOG_TAG "RfxAuthResponseData"

RfxAuthResponseData::RfxAuthResponseData(void* data, int length) : RfxBaseData(data, length) {
    if (data != NULL) {
        RIL_AuthResponse* pOriginal = (RIL_AuthResponse*)data;
        RIL_AuthResponse* pData = (RIL_AuthResponse*)calloc(1, sizeof(RIL_AuthResponse));
        if (pData == NULL) {
            RFX_LOG_E(RFX_LOG_TAG, "OOM");
            return;
        }

        pData->error = pOriginal->error;
        pData->capMask = pOriginal->capMask;
        pData->devIdLength = pOriginal->devIdLength;
        if (pOriginal->devId != NULL) {
            pData->devId = (char*)calloc(1, pData->devIdLength + 1);
            if (pData->devId == NULL) {
                mtkLogD(RFX_LOG_TAG, "OOM");
                return;
            }
            memcpy(pData->devId, pOriginal->devId, pData->devIdLength);
            pData->devId[pData->devIdLength] = '\0';
        }

        m_data = pData;
        m_length = length;
    }
}

RfxAuthResponseData::~RfxAuthResponseData() {
    // free memory
    if (m_data != NULL) {
        RIL_AuthResponse* pData = (RIL_AuthResponse*)m_data;
        if (pData->devId != NULL) {
            free(pData->devId);
        }
    }
}
