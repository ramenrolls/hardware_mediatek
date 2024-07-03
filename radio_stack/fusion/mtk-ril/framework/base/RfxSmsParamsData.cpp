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

#include "RfxSmsParamsData.h"
#include <telephony/mtk_ril.h>

RFX_IMPLEMENT_DATA_CLASS(RfxSmsParamsData);

RfxSmsParamsData::RfxSmsParamsData(void* _data, int _length) : RfxBaseData(_data, _length) {
    if (_data != NULL) {
        RIL_SmsParams* pSmsParams = (RIL_SmsParams*)_data;
        RIL_SmsParams* pData = (RIL_SmsParams*)calloc(1, sizeof(RIL_SmsParams));

        if (pData != NULL) {
            pData->format = pSmsParams->format;
            pData->dcs = pSmsParams->dcs;
            pData->pid = pSmsParams->pid;
            pData->vp = pSmsParams->vp;
        }

        m_data = (void*)pData;
        m_length = _length;
    }
}

RfxSmsParamsData::~RfxSmsParamsData() {
    // free memory
    if (m_data != NULL) {
        free(m_data);
        m_data = NULL;
    }
}
