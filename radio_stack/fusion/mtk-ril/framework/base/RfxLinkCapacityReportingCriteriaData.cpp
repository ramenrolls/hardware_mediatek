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
#include "RfxLinkCapacityReportingCriteriaData.h"

/*****************************************************************************
 * Define
 *****************************************************************************/
#define RFX_LOG_TAG "RfxLceRcData"

/*****************************************************************************
 * Class RfxLinkCapacityReportingCriteriaData
 *****************************************************************************/
RFX_IMPLEMENT_DATA_CLASS(RfxLinkCapacityReportingCriteriaData);

RfxLinkCapacityReportingCriteriaData::RfxLinkCapacityReportingCriteriaData(void* data, int length)
    : RfxBaseData(data, length) {
    if (data != NULL) {
        RIL_LinkCapacityReportingCriteria* tmpPtr = (RIL_LinkCapacityReportingCriteria*)data;
        RIL_LinkCapacityReportingCriteria* request = (RIL_LinkCapacityReportingCriteria*)calloc(
                1, sizeof(RIL_LinkCapacityReportingCriteria));
        if (request == NULL) {
            RFX_LOG_E(RFX_LOG_TAG, "Memory allocation failed for request");
            return;
        }

        request->hysteresisMs = tmpPtr->hysteresisMs;
        request->hysteresisDlKbps = tmpPtr->hysteresisDlKbps;
        request->hysteresisUlKbps = tmpPtr->hysteresisUlKbps;
        request->thresholdDlKbpsNumber = tmpPtr->thresholdDlKbpsNumber > MAX_LCE_THRESHOLD_NUMBER
                                                 ? MAX_LCE_THRESHOLD_NUMBER
                                                 : tmpPtr->thresholdDlKbpsNumber;
        for (int i = 0; i < request->thresholdDlKbpsNumber; i++) {
            request->thresholdDlKbpsList[i] = tmpPtr->thresholdDlKbpsList[i];
        }
        request->thresholdUlKbpsNumber = tmpPtr->thresholdUlKbpsNumber > MAX_LCE_THRESHOLD_NUMBER
                                                 ? MAX_LCE_THRESHOLD_NUMBER
                                                 : tmpPtr->thresholdUlKbpsNumber;
        for (int i = 0; i < request->thresholdUlKbpsNumber; i++) {
            request->thresholdUlKbpsList[i] = tmpPtr->thresholdUlKbpsList[i];
        }
        request->accessNetwork = tmpPtr->accessNetwork;

        m_data = request;
        m_length = length;
    }
}

RfxLinkCapacityReportingCriteriaData::~RfxLinkCapacityReportingCriteriaData() {
    if (m_data != NULL) {
        free(m_data);
    }
}
