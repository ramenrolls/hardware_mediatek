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

#include "RfxSmsWriteData.h"
#include <telephony/mtk_ril.h>

RFX_IMPLEMENT_DATA_CLASS(RfxSmsWriteData);

RfxSmsWriteData::RfxSmsWriteData(void* _data, int _length) : RfxBaseData(_data, _length) {
    if (_data != NULL) {
        RIL_SMS_WriteArgs* pSmsWrite = (RIL_SMS_WriteArgs*)_data;
        RIL_SMS_WriteArgs* pData = (RIL_SMS_WriteArgs*)calloc(1, sizeof(RIL_SMS_WriteArgs));

        if (pData != NULL) {
            pData->status = pSmsWrite->status;

            if (pSmsWrite->smsc != NULL) {
                asprintf(&pData->smsc, "%s", pSmsWrite->smsc);
            }
            if (pSmsWrite->pdu != NULL) {
                asprintf(&pData->pdu, "%s", pSmsWrite->pdu);
            }
        }

        m_data = (void*)pData;
        m_length = _length;
    }
}

RfxSmsWriteData::~RfxSmsWriteData() {
    // free memory
    if (m_data != NULL) {
        RIL_SMS_WriteArgs* pData = (RIL_SMS_WriteArgs*)m_data;
        if (pData->pdu != NULL) {
            free(pData->pdu);
            pData->pdu = NULL;
        }
        if (pData->smsc != NULL) {
            free(pData->smsc);
            pData->smsc = NULL;
        }
        free(m_data);
        m_data = NULL;
    }
}
