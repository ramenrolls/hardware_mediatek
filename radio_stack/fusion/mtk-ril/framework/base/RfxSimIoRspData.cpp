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

#include "RfxSimIoRspData.h"
#include <telephony/mtk_ril.h>

RFX_IMPLEMENT_DATA_CLASS(RfxSimIoRspData);

RfxSimIoRspData::RfxSimIoRspData(void* _data, int _length) : RfxBaseData(_data, _length) {
    if (_data != NULL) {
        RIL_SIM_IO_Response* pSimIoRsp = (RIL_SIM_IO_Response*)_data;
        RIL_SIM_IO_Response* pData = (RIL_SIM_IO_Response*)calloc(1, sizeof(RIL_SIM_IO_Response));
        RFX_ASSERT(pData != NULL);

        pData->sw1 = pSimIoRsp->sw1;
        pData->sw2 = pSimIoRsp->sw2;

        if (pSimIoRsp->simResponse != NULL) {
            asprintf(&pData->simResponse, "%s", pSimIoRsp->simResponse);
        }

        m_data = (RIL_SIM_IO_Response*)pData;
        m_length = _length;
    }
}

RfxSimIoRspData::~RfxSimIoRspData() {
    // free memory
    if (m_data != NULL) {
        RIL_SIM_IO_Response* pData = (RIL_SIM_IO_Response*)m_data;
        if (pData->simResponse != NULL) {
            free(pData->simResponse);
            pData->simResponse = NULL;
        }
        free(m_data);
        m_data = NULL;
    }
}
