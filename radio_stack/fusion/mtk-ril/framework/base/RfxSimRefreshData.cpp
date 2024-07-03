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

#include "RfxSimRefreshData.h"

RFX_IMPLEMENT_DATA_CLASS(RfxSimRefreshData);

RfxSimRefreshData::RfxSimRefreshData(void* data, int length) : RfxBaseData(data, length) {
    m_length = length;
    RIL_SimRefreshResponse_v7* dupData;
    RIL_SimRefreshResponse_v7* srcData = (RIL_SimRefreshResponse_v7*)data;
    int strLength;

    dupData = (RIL_SimRefreshResponse_v7*)calloc(1, sizeof(RIL_SimRefreshResponse_v7));
    RFX_ASSERT(dupData != NULL);
    memset(dupData, 0x00, sizeof(RIL_SimRefreshResponse_v7));
    m_data = dupData;

    /* result setting */
    dupData->result = srcData->result;

    /* ef_id setting */
    dupData->ef_id = srcData->ef_id;

    /* Copy aid */
    if (srcData->aid == NULL) {
        return;
    }

    strLength = strlen(srcData->aid) + 1;
    dupData->aid = (char*)calloc(strLength, sizeof(char));
    RFX_ASSERT(dupData->aid != NULL);
    memcpy(dupData->aid, srcData->aid, strLength);
}

RfxSimRefreshData::~RfxSimRefreshData() {
    // free
    RIL_SimRefreshResponse_v7* data = (RIL_SimRefreshResponse_v7*)m_data;
    free(data->aid);
    free(m_data);
}
