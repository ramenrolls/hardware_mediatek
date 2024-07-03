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
#include "RfxCallForwardInfosData.h"

RFX_IMPLEMENT_DATA_CLASS(RfxCallForwardInfosData);

RfxCallForwardInfosData::RfxCallForwardInfosData(void* data, int length)
    : RfxBaseData(data, length) {
    m_length = length;
    m_data = NULL;
    if (data != NULL) {
        int num = length / sizeof(RIL_CallForwardInfo*);

        if (num > 0) {
            RIL_CallForwardInfo** pTmp =
                    (RIL_CallForwardInfo**)calloc(num, sizeof(RIL_CallForwardInfo*));
            RIL_CallForwardInfo** pCur = (RIL_CallForwardInfo**)data;

            if (pTmp != NULL) {
                m_length = length;
                for (int i = 0; i < num; i++) {
                    pTmp[i] = (RIL_CallForwardInfo*)calloc(1, sizeof(RIL_CallForwardInfo));

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
                    }
                }
                m_data = pTmp;
            }
        }
    }
}

RfxCallForwardInfosData::~RfxCallForwardInfosData() {
    int num = m_length / sizeof(RIL_CallForwardInfo*);
    RIL_CallForwardInfo** pCur = (RIL_CallForwardInfo**)m_data;

    if (m_data != NULL) {
        for (int i = 0; i < num; i++) {
            if (pCur[i]->number != NULL) {
                free(pCur[i]->number);
            }
            free(pCur[i]);
        }
        free(pCur);
        m_data = NULL;
    }
}
