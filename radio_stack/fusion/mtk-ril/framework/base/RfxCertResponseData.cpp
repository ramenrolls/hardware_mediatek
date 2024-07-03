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

#include "RfxCertResponseData.h"
#include <telephony/mtk_ril.h>

RFX_IMPLEMENT_DATA_CLASS(RfxCertResponseData);

#define RFX_LOG_TAG "RfxCertResponseData"

RfxCertResponseData::RfxCertResponseData(void* data, int length) : RfxBaseData(data, length) {
    if (data != NULL) {
        RIL_CertResponse* pOriginal = (RIL_CertResponse*)data;
        RIL_CertResponse* pData = (RIL_CertResponse*)calloc(1, sizeof(RIL_CertResponse));
        if (pData == NULL) {
            RFX_LOG_E(RFX_LOG_TAG, "OOM");
            return;
        }

        pData->error = pOriginal->error;
        pData->custId = pOriginal->custId;
        pData->rndLength = pOriginal->rndLength;
        if (pOriginal->rnd != NULL) {
            pData->rnd = (char*)calloc(1, pData->rndLength + 1);
            if (pData->rnd == NULL) {
                mtkLogD(RFX_LOG_TAG, "OOM");
                return;
            }
            memcpy(pData->rnd, pOriginal->rnd, pData->rndLength);
            pData->rnd[pData->rndLength] = '\0';
        }

        m_data = pData;
        m_length = length;
    }
}

RfxCertResponseData::~RfxCertResponseData() {
    // free memory
    if (m_data != NULL) {
        RIL_CertResponse* pData = (RIL_CertResponse*)m_data;
        if (pData->rnd != NULL) {
            free(pData->rnd);
        }
    }
}
