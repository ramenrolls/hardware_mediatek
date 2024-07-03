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

#include "RfxSimOpenChannelData.h"
#include <telephony/mtk_ril.h>

RFX_IMPLEMENT_DATA_CLASS(RfxSimOpenChannelData);

RfxSimOpenChannelData::RfxSimOpenChannelData(void* _data, int _length)
    : RfxBaseData(_data, _length) {
    if (_data != NULL) {
        RIL_OpenChannelParams* pSimOpenChannel = (RIL_OpenChannelParams*)_data;
        RIL_OpenChannelParams* pData =
                (RIL_OpenChannelParams*)calloc(1, sizeof(RIL_OpenChannelParams));
        RFX_ASSERT(pData != NULL);

        pData->p2 = pSimOpenChannel->p2;

        if (pSimOpenChannel->aidPtr != NULL) {
            asprintf(&pData->aidPtr, "%s", pSimOpenChannel->aidPtr);
        }

        m_data = (void*)pData;
        m_length = _length;
    }
}

RfxSimOpenChannelData::~RfxSimOpenChannelData() {
    // free memory
    if (m_data != NULL) {
        RIL_OpenChannelParams* pData = (RIL_OpenChannelParams*)m_data;
        if (pData->aidPtr != NULL) {
            free(pData->aidPtr);
            pData->aidPtr = NULL;
        }
        free(m_data);
        m_data = NULL;
    }
}
