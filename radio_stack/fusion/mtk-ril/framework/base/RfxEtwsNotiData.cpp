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

#include "RfxEtwsNotiData.h"
#include <telephony/mtk_ril.h>

#define LOG_TAG "ETWS"

RFX_IMPLEMENT_DATA_CLASS(RfxEtwsNotiData);

RfxEtwsNotiData::RfxEtwsNotiData(void* _data, int _length) : RfxBaseData(_data, _length) {
    if (_data != NULL) {
        RIL_CBEtwsNotification* pEtwsNoti = (RIL_CBEtwsNotification*)_data;
        RIL_CBEtwsNotification* pData =
                (RIL_CBEtwsNotification*)calloc(1, sizeof(RIL_CBEtwsNotification));

        if (pData != NULL) {
            pData->warningType = pEtwsNoti->warningType;
            pData->serialNumber = pEtwsNoti->serialNumber;
            pData->messageId = pEtwsNoti->messageId;

            if (pEtwsNoti->plmnId != NULL) {
                asprintf(&pData->plmnId, "%s", pEtwsNoti->plmnId);
            }
            if (pEtwsNoti->securityInfo != NULL) {
                asprintf(&pData->securityInfo, "%s", pEtwsNoti->securityInfo);
            }
        }

        m_data = (void*)pData;
        m_length = _length;
    }
}

RfxEtwsNotiData::~RfxEtwsNotiData() {
    // free memory
    if (m_data != NULL) {
        RIL_CBEtwsNotification* pData = (RIL_CBEtwsNotification*)m_data;
        if (pData->plmnId != NULL) {
            free(pData->plmnId);
            pData->plmnId = NULL;
        }
        if (pData->securityInfo != NULL) {
            free(pData->securityInfo);
            pData->securityInfo = NULL;
        }
        free(m_data);
        m_data = NULL;
    }
}
