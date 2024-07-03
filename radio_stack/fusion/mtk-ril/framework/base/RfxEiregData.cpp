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

#include "RfxEiregData.h"
#include <telephony/mtk_ril.h>

#define LOG_TAG "EiregData"

RFX_IMPLEMENT_DATA_CLASS(RfxEiregData);

RfxEiregData::RfxEiregData(void* _data, int _length) : RfxBaseData(_data, _length) {
    if (_data != NULL) {
        RIL_EiregData* pIn = (RIL_EiregData*)_data;
        RIL_EiregData* pOut = (RIL_EiregData*)calloc(1, sizeof(RIL_EiregData));

        pOut->reg_type = pIn->reg_type;
        pOut->reg_state = pIn->reg_state;
        pOut->ext_info = pIn->ext_info;
        pOut->dereg_cause = pIn->dereg_cause;
        pOut->ims_retry = pIn->ims_retry;
        pOut->rat = pIn->rat;
        pOut->sip_uri_type = pIn->sip_uri_type;

        m_data = (void*)pOut;
        m_length = _length;
    }
}

RfxEiregData::~RfxEiregData() {
    // free memory
    if (m_data != NULL) {
        free(m_data);
        m_data = NULL;
    }
}
