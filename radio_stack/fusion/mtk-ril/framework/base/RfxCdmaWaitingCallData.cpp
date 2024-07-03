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

#include "RfxCdmaWaitingCallData.h"

RFX_IMPLEMENT_DATA_CLASS(RfxCdmaWaitingCallData);

RfxCdmaWaitingCallData::RfxCdmaWaitingCallData(void* data, int length) : RfxBaseData(data, length) {
    if (data == NULL) {
        m_data = NULL;
        return;
    }

    int len = 0;
    m_length = length;
    RIL_CDMA_CallWaiting_v6* tmp = (RIL_CDMA_CallWaiting_v6*)data;
    RIL_CDMA_CallWaiting_v6* dup =
            (RIL_CDMA_CallWaiting_v6*)calloc(1, sizeof(RIL_CDMA_CallWaiting_v6));
    RFX_ASSERT(dup != NULL);
    memset(dup, 0, sizeof(RIL_CDMA_CallWaiting_v6));
    memcpy(dup, tmp, sizeof(RIL_CDMA_CallWaiting_v6));
    if (tmp->number != NULL) {
        len = strlen(tmp->number) + 1;
        dup->number = (char*)calloc(len, sizeof(char));
        RFX_ASSERT(dup->number != NULL);
        strncpy(dup->number, tmp->number, len - 1);
    }
    if (tmp->name != NULL) {
        len = strlen(tmp->name) + 1;
        dup->name = (char*)calloc(len, sizeof(char));
        RFX_ASSERT(dup->name != NULL);
        strncpy(dup->name, tmp->name, len - 1);
    }
    m_data = dup;
}

RfxCdmaWaitingCallData::~RfxCdmaWaitingCallData() {
    // free
    RIL_CDMA_CallWaiting_v6* tmp = (RIL_CDMA_CallWaiting_v6*)m_data;
    if (tmp != NULL) {
        if (tmp->number != NULL) {
            free(tmp->number);
        }
        if (tmp->name != NULL) {
            free(tmp->name);
        }
        free(tmp);
    }
    m_data = NULL;
}
