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
#include "RfxCallForwardInfosExData.h"

RFX_IMPLEMENT_DATA_CLASS(RfxCallForwardInfosExData);

RfxCallForwardInfosExData::RfxCallForwardInfosExData(void* data, int length)
    : RfxBaseData(data, length) {
    m_length = length;
    m_data = NULL;
    if (data != NULL) {
        int num = length / sizeof(RIL_CallForwardInfoEx*);

        if (num > 0) {
            RIL_CallForwardInfoEx** pTmp =
                    (RIL_CallForwardInfoEx**)calloc(num, sizeof(RIL_CallForwardInfoEx*));
            RIL_CallForwardInfoEx** pCur = (RIL_CallForwardInfoEx**)data;

            if (pTmp != NULL) {
                m_length = length;
                for (int i = 0; i < num; i++) {
                    pTmp[i] = (RIL_CallForwardInfoEx*)calloc(1, sizeof(RIL_CallForwardInfoEx));

                    if (pTmp[i] != NULL) {
                        pTmp[i]->status = pCur[i]->status;
                        pTmp[i]->reason = pCur[i]->reason;
                        pTmp[i]->serviceClass = pCur[i]->serviceClass;
                        pTmp[i]->toa = pCur[i]->toa;

                        if (pCur[i]->number != NULL) {
                            int len = strlen(pCur[i]->number);
                            pTmp[i]->number = (char*)calloc(len + 1, sizeof(char));
                            if (pTmp[i]->number != NULL) {
                                strncpy(pTmp[i]->number, pCur[i]->number, len);
                            }
                        }

                        pTmp[i]->timeSeconds = pCur[i]->timeSeconds;

                        if (pCur[i]->timeSlotBegin != NULL) {
                            int len = strlen(pCur[i]->timeSlotBegin);
                            pTmp[i]->timeSlotBegin = (char*)calloc(len + 1, sizeof(char));
                            if (pTmp[i]->timeSlotBegin != NULL) {
                                strncpy(pTmp[i]->timeSlotBegin, pCur[i]->timeSlotBegin, len);
                            }
                        }
                        if (pCur[i]->timeSlotEnd != NULL) {
                            int len = strlen(pCur[i]->timeSlotEnd);
                            pTmp[i]->timeSlotEnd = (char*)calloc(len + 1, sizeof(char));
                            if (pTmp[i]->timeSlotEnd != NULL) {
                                strncpy(pTmp[i]->timeSlotEnd, pCur[i]->timeSlotEnd, len);
                            }
                        }
                    }
                }
                m_data = pTmp;
            }
        }
    }
}

RfxCallForwardInfosExData::~RfxCallForwardInfosExData() {
    int num = m_length / sizeof(RIL_CallForwardInfoEx*);
    RIL_CallForwardInfoEx** pCur = (RIL_CallForwardInfoEx**)m_data;

    if (m_data != NULL) {
        for (int i = 0; i < num; i++) {
            if (pCur[i]->number != NULL) {
                free(pCur[i]->number);
            }
            if (pCur[i]->timeSlotBegin != NULL) {
                free(pCur[i]->timeSlotBegin);
            }
            if (pCur[i]->timeSlotEnd != NULL) {
                free(pCur[i]->timeSlotEnd);
            }
            free(pCur[i]);
        }
        free(pCur);
        m_data = NULL;
    }
}
