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

#include "RfxImsRegInfoData.h"
#include <telephony/mtk_ril.h>

#define LOG_TAG "ImsRegInfo"

RFX_IMPLEMENT_DATA_CLASS(RfxImsRegInfoData);

RfxImsRegInfoData::RfxImsRegInfoData(void* _data, int _length) : RfxBaseData(_data, _length) {
    if (_data != NULL) {
        RIL_ImsRegInfo* pImsRegInfo = (RIL_ImsRegInfo*)_data;
        RIL_ImsRegInfo* pData = (RIL_ImsRegInfo*)calloc(1, sizeof(RIL_ImsRegInfo));

        if (pData != NULL) {
            pData->report_type = pImsRegInfo->report_type;
            pData->account_id = pImsRegInfo->account_id;
            pData->expire_time = pImsRegInfo->expire_time;
            pData->error_code = pImsRegInfo->error_code;

            if (pImsRegInfo->uri != NULL) {
                asprintf(&pData->uri, "%s", pImsRegInfo->uri);
            }
            if (pImsRegInfo->error_msg != NULL) {
                asprintf(&pData->error_msg, "%s", pImsRegInfo->error_msg);
            }
        }

        m_data = (void*)pData;
        m_length = _length;
    }
}

RfxImsRegInfoData::~RfxImsRegInfoData() {
    // free memory
    if (m_data != NULL) {
        RIL_ImsRegInfo* pData = (RIL_ImsRegInfo*)m_data;
        if (pData->uri != NULL) {
            free(pData->uri);
            pData->uri = NULL;
        }
        if (pData->error_msg != NULL) {
            free(pData->error_msg);
            pData->error_msg = NULL;
        }
        free(m_data);
        m_data = NULL;
    }
}
