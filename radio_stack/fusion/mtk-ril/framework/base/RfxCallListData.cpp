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

#include "RfxCallListData.h"

RFX_IMPLEMENT_DATA_CLASS(RfxCallListData);

RfxCallListData::RfxCallListData(void* data, int length) : RfxBaseData(data, length) {
    if (data == NULL) {
        m_data = NULL;
        return;
    }

    m_length = length;
    m_data = NULL;
    int countOfCall = m_length / sizeof(RIL_Call*);
    RIL_Call** pp_calls = (RIL_Call**)calloc(countOfCall, sizeof(RIL_Call*));
    RFX_ASSERT(pp_calls != NULL);

    if (countOfCall > 0) {
        RIL_Call** tmp = (RIL_Call**)data;
        for (int i = 0; i < countOfCall; i++) {
            pp_calls[i] = (RIL_Call*)calloc(1, sizeof(RIL_Call));
            RFX_ASSERT(pp_calls[i] != NULL);
            memset(pp_calls[i], 0, sizeof(RIL_Call));
            memcpy(pp_calls[i], tmp[i], sizeof(RIL_Call));
            if (tmp[i]->number != NULL) {
                int len = strlen(tmp[i]->number);
                // Allocate and zero-initialize array
                pp_calls[i]->number = (char*)calloc(len + 1, sizeof(char));
                RFX_ASSERT(pp_calls[i]->number != NULL);
                strncpy(pp_calls[i]->number, tmp[i]->number, len);
            }
            if (tmp[i]->name != NULL) {
                int len = strlen(tmp[i]->name);
                // Allocate and zero-initialize array
                pp_calls[i]->name = (char*)calloc(len + 1, sizeof(char));
                RFX_ASSERT(pp_calls[i]->name != NULL);
                strncpy(pp_calls[i]->name, tmp[i]->name, len);
            }
        }
    }

    m_data = pp_calls;
}

RfxCallListData::~RfxCallListData() {
    // free
    int countOfCall = m_length / sizeof(RIL_Call*);
    RIL_Call** pp_calls = (RIL_Call**)m_data;
    if (pp_calls != NULL) {
        for (int i = 0; i < countOfCall; i++) {
            if (pp_calls[i] != NULL) {
                if (pp_calls[i]->number != NULL) {
                    free(pp_calls[i]->number);
                }
                if (pp_calls[i]->name != NULL) {
                    free(pp_calls[i]->name);
                }
                free(pp_calls[i]);
            }
        }
        free(pp_calls);
    }
    m_data = NULL;
}
