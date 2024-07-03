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
#include <telephony/ril.h>
#include "RfxCallForwardInfoExData.h"

RFX_IMPLEMENT_DATA_CLASS(RfxCallForwardInfoExData);

RfxCallForwardInfoExData::RfxCallForwardInfoExData(void* data, int length)
    : RfxBaseData(data, length) {
    RIL_CallForwardInfoEx* pTmp = (RIL_CallForwardInfoEx*)data;
    RIL_CallForwardInfoEx* pData = (RIL_CallForwardInfoEx*)calloc(1, sizeof(RIL_CallForwardInfoEx));

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

        if (pTmp->timeSlotBegin != NULL) {
            int len = strlen(pTmp->timeSlotBegin);
            pData->timeSlotBegin = (char*)calloc(len + 1, sizeof(char));
            if (pData->timeSlotBegin != NULL) {
                strncpy(pData->timeSlotBegin, pTmp->timeSlotBegin, len);
            }
        }
        if (pTmp->timeSlotEnd != NULL) {
            int len = strlen(pTmp->timeSlotEnd);
            pData->timeSlotEnd = (char*)calloc(len + 1, sizeof(char));
            if (pData->timeSlotEnd != NULL) {
                strncpy(pData->timeSlotEnd, pTmp->timeSlotEnd, len);
            }
        }

        m_data = pData;
        m_length = length;
    }
}

RfxCallForwardInfoExData::~RfxCallForwardInfoExData() {
    if (m_data != NULL && ((RIL_CallForwardInfoEx*)m_data)->number != NULL) {
        free(((RIL_CallForwardInfoEx*)m_data)->number);
    }
    if (m_data != NULL && ((RIL_CallForwardInfoEx*)m_data)->timeSlotBegin != NULL) {
        free(((RIL_CallForwardInfoEx*)m_data)->timeSlotBegin);
    }
    if (m_data != NULL && ((RIL_CallForwardInfoEx*)m_data)->timeSlotEnd != NULL) {
        free(((RIL_CallForwardInfoEx*)m_data)->timeSlotEnd);
    }
    free(m_data);
    m_data = NULL;
}
