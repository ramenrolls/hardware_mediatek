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

// External SIM - START
#include "RfxVsimEventData.h"
#include <telephony/mtk_ril.h>
#include "RfxRilUtils.h"

RFX_IMPLEMENT_DATA_CLASS(RfxVsimEventData);

RfxVsimEventData::RfxVsimEventData(void* data, int length) : RfxBaseData(data, length) {
    if (data != NULL) {
        RIL_VsimEvent* pVsim = (RIL_VsimEvent*)data;
        RIL_VsimEvent* pData = (RIL_VsimEvent*)calloc(1, sizeof(RIL_VsimEvent));
        if (pData == NULL) {
            RfxRilUtils::printLog(ERROR, String8::format("[RfxVsimEventData]"),
                                  String8::format("fail to allocate pData!"), 0);
            return;
        }
        pData->eventId = pVsim->eventId;
        pData->sim_type = pVsim->sim_type;
        pData->transaction_id = pVsim->transaction_id;

        m_data = pData;
        m_length = length;
    }
}

RfxVsimEventData::~RfxVsimEventData() {
    if (m_data != NULL) {
        RIL_VsimEvent* pData = (RIL_VsimEvent*)m_data;
        free(m_data);
    }
}
// External SIM - END
