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

#include "RfxSimGenAuthData.h"
#include <telephony/mtk_ril.h>

RFX_IMPLEMENT_DATA_CLASS(RfxSimGenAuthData);

RfxSimGenAuthData::RfxSimGenAuthData(void* _data, int _length) : RfxBaseData(_data, _length) {
    if (_data != NULL) {
        RIL_SimAuthStructure* pSimAuth = (RIL_SimAuthStructure*)_data;
        RIL_SimAuthStructure* pData =
                (RIL_SimAuthStructure*)calloc(1, sizeof(RIL_SimAuthStructure));
        RFX_ASSERT(pData != NULL);
        pData->sessionId = pSimAuth->sessionId;
        pData->mode = pSimAuth->mode;
        pData->tag = pSimAuth->tag;

        if (pSimAuth->param1 != NULL) {
            asprintf(&pData->param1, "%s", pSimAuth->param1);
        }
        if (pSimAuth->param2 != NULL) {
            asprintf(&pData->param2, "%s", pSimAuth->param2);
        }

        m_data = (void*)pData;
        m_length = _length;
    }
}

RfxSimGenAuthData::~RfxSimGenAuthData() {
    // free memory
    if (m_data != NULL) {
        RIL_SimAuthStructure* pData = (RIL_SimAuthStructure*)m_data;
        if (pData->param1 != NULL) {
            free(pData->param1);
            pData->param1 = NULL;
        }
        if (pData->param2 != NULL) {
            free(pData->param2);
            pData->param2 = NULL;
        }
        free(m_data);
        m_data = NULL;
    }
}
