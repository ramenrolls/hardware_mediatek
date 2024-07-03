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

#include "RfxVtSendMsgData.h"
#include <telephony/mtk_ril.h>

RFX_IMPLEMENT_DATA_CLASS(RfxVtSendMsgData);

RfxVtSendMsgData::RfxVtSendMsgData(void* _data, int _length) : RfxBaseData(_data, _length) {
    if (_data != NULL) {
        RIL_VT_SERVICE_MSG* pSendMsg = (RIL_VT_SERVICE_MSG*)_data;
        RIL_VT_SERVICE_MSG* pData = (RIL_VT_SERVICE_MSG*)calloc(1, sizeof(RIL_VT_SERVICE_MSG));

        if (NULL != pData) {
            pData->slot_id = pSendMsg->slot_id;
            pData->size = pSendMsg->size;
            pData->data = (char*)calloc(1, pSendMsg->size);

            if (NULL != pData->data) {
                memcpy(pData->data, pSendMsg->data, pSendMsg->size);
            }
        }

        m_data = (void*)pData;
        m_length = _length;
    }
}

RfxVtSendMsgData::~RfxVtSendMsgData() {
    // free memory
    if (m_data != NULL) {
        RIL_VT_SERVICE_MSG* pData = (RIL_VT_SERVICE_MSG*)m_data;
        if (pData->data != NULL) {
            free(pData->data);
            pData->data = NULL;
        }

        free(m_data);
        m_data = NULL;
    }
}
