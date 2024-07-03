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
#include "RfxVsimOpIndicationData.h"
#include <telephony/mtk_ril.h>
#include "RfxRilUtils.h"

RFX_IMPLEMENT_DATA_CLASS(RfxVsimOpIndicationData);

RfxVsimOpIndicationData::RfxVsimOpIndicationData(void* data, int length)
    : RfxBaseData(data, length) {
    if (data != NULL) {
        RIL_VsimOperationEvent* pVsim = (RIL_VsimOperationEvent*)data;
        RIL_VsimOperationEvent* pData =
                (RIL_VsimOperationEvent*)calloc(1, sizeof(RIL_VsimOperationEvent));
        if (pData == NULL) {
            RfxRilUtils::printLog(ERROR, String8::format("[RfxVsimOpIndicationData]"),
                                  String8::format("fail to allocate pData!"), 0);
            return;
        }
        pData->data_length = pVsim->data_length;
        if (pVsim->data != NULL) {
            pData->data = (char*)calloc(1, pVsim->data_length + 1);

            if (pData->data != NULL) {
                memset(pData->data, 0, pVsim->data_length + 1);
                memcpy(pData->data, pVsim->data, pVsim->data_length);
            } else {
                RfxRilUtils::printLog(
                        ERROR, String8::format("[RfxVsimOpIndicationData]"),
                        String8::format("fail to allocate memory, size = %d", pData->data_length),
                        0);
                pData->data_length = 0;
            }
        }
        pData->eventId = pVsim->eventId;
        pData->result = pVsim->result;
        pData->transaction_id = pVsim->transaction_id;

        m_data = pData;
        m_length = length;
    }
}

RfxVsimOpIndicationData::~RfxVsimOpIndicationData() {
    if (m_data != NULL) {
        RIL_VsimOperationEvent* pData = (RIL_VsimOperationEvent*)m_data;
        if (pData->data != NULL) {
            free(pData->data);
        }
        free(m_data);
    }
}
// External SIM - END
