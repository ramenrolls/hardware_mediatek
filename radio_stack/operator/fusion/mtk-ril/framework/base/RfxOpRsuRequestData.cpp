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

#include "RfxOpRsuRequestData.h"

RFX_IMPLEMENT_DATA_CLASS(RfxOpRsuRequestData);

RfxOpRsuRequestData::RfxOpRsuRequestData(void* data, int length) : RfxBaseData(data, length) {
    m_length = length;
    RIL_RsuRequestInfo* dupData;
    RIL_RsuRequestInfo* srcData = (RIL_RsuRequestInfo*)data;
    int strLength = -1;

    dupData = (RIL_RsuRequestInfo*)calloc(1, sizeof(RIL_RsuRequestInfo));
    RFX_ASSERT(dupData != NULL);
    memcpy(dupData, srcData, sizeof(RIL_RsuRequestInfo));
    if (srcData->data != NULL) {
        strLength = strlen(srcData->data) + 1;
        dupData->data = (char*)calloc(strLength, sizeof(char));
        RFX_ASSERT(dupData->data != NULL);
        memset(dupData->data, 0, strLength);
        memcpy(dupData->data, srcData->data, strLength);
    }
    if (srcData->reserveString1 != NULL) {
        strLength = strlen(srcData->reserveString1) + 1;
        dupData->reserveString1 = (char*)calloc(strLength, sizeof(char));
        RFX_ASSERT(dupData->reserveString1 != NULL);
        memset(dupData->reserveString1, 0, strLength);
        memcpy(dupData->reserveString1, srcData->reserveString1, strLength);
    }

    m_data = dupData;
}

RfxOpRsuRequestData::~RfxOpRsuRequestData() {
    RIL_RsuRequestInfo* data = (RIL_RsuRequestInfo*)m_data;
    if (data != NULL) {
        if (data->data != NULL) {
            free(data->data);
        }
        if (data->reserveString1 != NULL) {
            free(data->reserveString1);
        }
        free(m_data);
    }
}
