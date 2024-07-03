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

#include "RfxSmsRspData.h"
#include <telephony/mtk_ril.h>

RFX_IMPLEMENT_DATA_CLASS(RfxSmsRspData);

RfxSmsRspData::RfxSmsRspData(void* _data, int _length) : RfxBaseData(_data, _length) {
    if (_data != NULL) {
        RIL_SMS_Response* pSmsRsp = (RIL_SMS_Response*)_data;
        RIL_SMS_Response* pData = (RIL_SMS_Response*)calloc(1, sizeof(RIL_SMS_Response));

        if (pData != NULL) {
            pData->messageRef = pSmsRsp->messageRef;
            pData->errorCode = pSmsRsp->errorCode;

            if (pSmsRsp->ackPDU != NULL) {
                asprintf(&pData->ackPDU, "%s", pSmsRsp->ackPDU);
            }
        }

        m_data = (void*)pData;
        m_length = _length;
    }
}

RfxSmsRspData::~RfxSmsRspData() {
    // free memory
    if (m_data != NULL) {
        RIL_SMS_Response* pData = (RIL_SMS_Response*)m_data;
        if (pData->ackPDU != NULL) {
            free(pData->ackPDU);
            pData->ackPDU = NULL;
        }
        free(m_data);
        m_data = NULL;
    }
}
