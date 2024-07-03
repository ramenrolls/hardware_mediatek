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

#include "RfxKeepaliveStatusData.h"
#include <string.h>

RFX_IMPLEMENT_DATA_CLASS(RfxKeepaliveStatusData);

RfxKeepaliveStatusData::RfxKeepaliveStatusData(void* data, int length) : RfxBaseData(data, length) {
    if (data != NULL) {
        RIL_KeepaliveStatus* pInput = (RIL_KeepaliveStatus*)data;
        RIL_KeepaliveStatus* pData = (RIL_KeepaliveStatus*)calloc(1, sizeof(RIL_KeepaliveStatus));
        RFX_ASSERT(pData != NULL);

        pData->sessionHandle = pInput->sessionHandle;
        pData->code = pInput->code;

        m_data = (void*)pData;
        m_length = length;
    }
}

RfxKeepaliveStatusData::~RfxKeepaliveStatusData() {
    // free
    if (m_data != NULL) {
        free(m_data);
        m_data = NULL;
    }
}
