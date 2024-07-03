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

#include "RfxVtCallStatusData.h"
#include <telephony/mtk_ril.h>
#include <stdio.h>

RFX_IMPLEMENT_DATA_CLASS(RfxVtCallStatusData);

RfxVtCallStatusData::RfxVtCallStatusData(void* _data, int _length) : RfxBaseData(_data, _length) {
    if (_data != NULL) {
        RIL_VT_CALL_STATUS_UPDATE* pCallStatus = (RIL_VT_CALL_STATUS_UPDATE*)_data;
        RIL_VT_CALL_STATUS_UPDATE* pData =
                (RIL_VT_CALL_STATUS_UPDATE*)calloc(1, sizeof(RIL_VT_CALL_STATUS_UPDATE));

        pData->phone = pCallStatus->phone;
        pData->call_id = pCallStatus->call_id;
        pData->call_state = pCallStatus->call_state;

        if (pCallStatus->data != NULL) {
            asprintf(&pData->data, "%s", pCallStatus->data);
        }

        m_data = (void*)pData;
        m_length = _length;
    }
}

RfxVtCallStatusData::~RfxVtCallStatusData() {
    // free memory
    if (m_data != NULL) {
        free(m_data);
        m_data = NULL;
    }
}
