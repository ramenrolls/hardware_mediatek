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
#include "RfxLinkCapacityEstimateData.h"

/*****************************************************************************
 * Define
 *****************************************************************************/
#define RFX_LOG_TAG "RfxLceData"

/*****************************************************************************
 * Class RfxLinkCapacityEstimateData
 *****************************************************************************/
RFX_IMPLEMENT_DATA_CLASS(RfxLinkCapacityEstimateData);

RfxLinkCapacityEstimateData::RfxLinkCapacityEstimateData(void* data, int length)
    : RfxBaseData(data, length) {
    if (data != NULL) {
        RIL_LinkCapacityEstimate* tmpPtr = (RIL_LinkCapacityEstimate*)data;
        RIL_LinkCapacityEstimate* response =
                (RIL_LinkCapacityEstimate*)calloc(1, sizeof(RIL_LinkCapacityEstimate));
        if (response == NULL) {
            RFX_LOG_E(RFX_LOG_TAG, "Memory allocation failed for response");
            return;
        }

        response->downlinkCapacityKbps = tmpPtr->downlinkCapacityKbps;
        response->uplinkCapacityKbps = tmpPtr->uplinkCapacityKbps;

        m_data = response;
        m_length = length;
    }
}

RfxLinkCapacityEstimateData::~RfxLinkCapacityEstimateData() {
    if (m_data != NULL) {
        free(m_data);
    }
}
