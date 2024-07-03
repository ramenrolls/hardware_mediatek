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

#include "RfxSimApduData.h"
#include <telephony/mtk_ril.h>

RFX_IMPLEMENT_DATA_CLASS(RfxSimApduData);

RfxSimApduData::RfxSimApduData(void* _data, int _length) : RfxBaseData(_data, _length) {
    if (_data != NULL) {
        RIL_SIM_APDU* pSimApdu = (RIL_SIM_APDU*)_data;
        RIL_SIM_APDU* pData = (RIL_SIM_APDU*)calloc(1, sizeof(RIL_SIM_APDU));
        RFX_ASSERT(pData != NULL);

        pData->sessionid = pSimApdu->sessionid;
        pData->cla = pSimApdu->cla;
        pData->instruction = pSimApdu->instruction;
        pData->p1 = pSimApdu->p1;
        pData->p2 = pSimApdu->p2;
        pData->p3 = pSimApdu->p3;
        if (pSimApdu->data != NULL) {
            asprintf(&pData->data, "%s", pSimApdu->data);
        }

        m_data = (void*)pData;
        m_length = _length;
    }
}

RfxSimApduData::~RfxSimApduData() {
    // free memory
    if (m_data != NULL) {
        RIL_SIM_APDU* pData = (RIL_SIM_APDU*)m_data;
        if (pData->data != NULL) {
            free(pData->data);
            pData->data = NULL;
        }
        free(m_data);
        m_data = NULL;
    }
}
