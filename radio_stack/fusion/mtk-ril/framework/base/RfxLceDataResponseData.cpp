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

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "RfxLceDataResponseData.h"

/*****************************************************************************
 * Class RfxLceDataResponseData
 *****************************************************************************/
RFX_IMPLEMENT_DATA_CLASS(RfxLceDataResponseData);

RfxLceDataResponseData::RfxLceDataResponseData(void* data, int length) : RfxBaseData(data, length) {
    if (data != NULL) {
        RIL_LceDataInfo* tmpPtr = (RIL_LceDataInfo*)data;
        RIL_LceDataInfo* response = (RIL_LceDataInfo*)calloc(1, sizeof(RIL_LceDataInfo));
        RFX_ASSERT(response != NULL);
        response->last_hop_capacity_kbps = tmpPtr->last_hop_capacity_kbps;
        response->confidence_level = tmpPtr->confidence_level;
        response->lce_suspended = tmpPtr->lce_suspended;
        m_data = response;
        m_length = length;
    }
}

RfxLceDataResponseData::~RfxLceDataResponseData() {
    if (m_data != NULL) {
        free(m_data);
    }
}
