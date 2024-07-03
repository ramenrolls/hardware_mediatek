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

#include <string.h>
#include "RfxBaseData.h"
#include <telephony/ril.h>
#include "RfxCallForwardInfoData.h"

RFX_IMPLEMENT_DATA_CLASS(RfxCallForwardInfoData);

RfxCallForwardInfoData::RfxCallForwardInfoData(void* data, int length) : RfxBaseData(data, length) {
    RIL_CallForwardInfo* pTmp = (RIL_CallForwardInfo*)data;
    RIL_CallForwardInfo* pData = (RIL_CallForwardInfo*)calloc(1, sizeof(RIL_CallForwardInfo));

    if (pData != NULL) {
        pData->status = pTmp->status;
        pData->reason = pTmp->reason;
        pData->serviceClass = pTmp->serviceClass;
        pData->toa = pTmp->toa;

        if (pTmp->number != NULL) {
            int len = strlen(pTmp->number);
            pData->number = (char*)calloc(len + 1, sizeof(char));
            if (pData->number != NULL) {
                strncpy(pData->number, pTmp->number, len);
            }
        }

        pData->timeSeconds = pTmp->timeSeconds;

        m_data = pData;
        m_length = length;
    }
}

RfxCallForwardInfoData::~RfxCallForwardInfoData() {
    if (m_data != NULL && ((RIL_CallForwardInfo*)m_data)->number != NULL) {
        free(((RIL_CallForwardInfo*)m_data)->number);
    }
    free(m_data);
    m_data = NULL;
}
