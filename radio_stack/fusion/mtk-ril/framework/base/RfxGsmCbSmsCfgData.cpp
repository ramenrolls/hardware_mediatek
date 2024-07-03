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

#include "RfxGsmCbSmsCfgData.h"
#include "RfxLog.h"
#include <telephony/mtk_ril.h>

RFX_IMPLEMENT_DATA_CLASS(RfxGsmCbSmsCfgData);

RfxGsmCbSmsCfgData::RfxGsmCbSmsCfgData(void* _data, int _length) : RfxBaseData(_data, _length) {
    if (_data != NULL) {
        int num = _length / sizeof(RIL_GSM_BroadcastSmsConfigInfo*);
        RIL_GSM_BroadcastSmsConfigInfo** pGsmBrSmsCnf = (RIL_GSM_BroadcastSmsConfigInfo**)_data;
        RIL_GSM_BroadcastSmsConfigInfo** pData = (RIL_GSM_BroadcastSmsConfigInfo**)calloc(
                1, sizeof(RIL_GSM_BroadcastSmsConfigInfo*) * num);
        RFX_ASSERT(pData != NULL);

        for (int i = 0; i < num; i++) {
            pData[i] = (RIL_GSM_BroadcastSmsConfigInfo*)calloc(
                    1, sizeof(RIL_GSM_BroadcastSmsConfigInfo));
            RFX_ASSERT(pData[i] != NULL);
            pData[i]->fromServiceId = pGsmBrSmsCnf[i]->fromServiceId;
            pData[i]->toServiceId = pGsmBrSmsCnf[i]->toServiceId;
            pData[i]->fromCodeScheme = pGsmBrSmsCnf[i]->fromCodeScheme;
            pData[i]->toCodeScheme = pGsmBrSmsCnf[i]->toCodeScheme;
            pData[i]->selected = pGsmBrSmsCnf[i]->selected;
        }
        m_data = (void*)pData;
        m_length = _length;
    }
}

RfxGsmCbSmsCfgData::~RfxGsmCbSmsCfgData() {
    // free memory
    if (m_data != NULL) {
        RIL_GSM_BroadcastSmsConfigInfo** pData = (RIL_GSM_BroadcastSmsConfigInfo**)m_data;
        int num = m_length / sizeof(RIL_GSM_BroadcastSmsConfigInfo*);
        for (int i = 0; i < num; i++) {
            if (pData[i] != NULL) {
                free(pData[i]);
            }
        }
        free(m_data);
        m_data = NULL;
    }
}
