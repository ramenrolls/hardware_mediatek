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

#include "RfxCertMsgData.h"
#include <telephony/mtk_ril.h>
#include <mtk_log.h>

RFX_IMPLEMENT_DATA_CLASS(RfxCertMsgData);

#define RFX_LOG_TAG "RfxCertMsgData"

RfxCertMsgData::RfxCertMsgData(void* data, int length) : RfxBaseData(data, length) {
    if (data != NULL) {
        RIL_CertMsg* pOriginal = (RIL_CertMsg*)data;
        RIL_CertMsg* pData = (RIL_CertMsg*)calloc(1, sizeof(RIL_CertMsg));
        if (pData == NULL) {
            RFX_LOG_E(RFX_LOG_TAG, "OOM");
            return;
        }

        pData->uid = pOriginal->uid;
        pData->certLength = pOriginal->certLength;
        if (pOriginal->cert != NULL) {
            pData->cert = (char*)calloc(1, pData->certLength + 1);
            if (pData->cert == NULL) {
                mtkLogD(RFX_LOG_TAG, "OOM");
                return;
            }
            memcpy(pData->cert, pOriginal->cert, pData->certLength);
            pData->cert[pData->certLength] = '\0';
        }
        pData->msgLength = pOriginal->msgLength;
        if (pOriginal->msg != NULL) {
            pData->msg = (char*)calloc(1, pData->msgLength + 1);
            if (pData->msg == NULL) {
                mtkLogD(RFX_LOG_TAG, "OOM");
                return;
            }
            memcpy(pData->msg, pOriginal->msg, pData->msgLength);
            pData->msg[pData->msgLength] = '\0';
        }

        m_data = (void*)pData;
        m_length = length;
    }
}

RfxCertMsgData::~RfxCertMsgData() {
    // free memory
    if (m_data != NULL) {
        RIL_CertMsg* pData = (RIL_CertMsg*)m_data;
        if (pData->cert != NULL) {
            free(pData->cert);
        }
        if (pData->msg != NULL) {
            free(pData->msg);
        }
        free(pData);
    }
}
