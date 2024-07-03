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

#include "RfxRsuResponseData.h"

RFX_IMPLEMENT_DATA_CLASS(RfxRsuResponseData);

RfxRsuResponseData::RfxRsuResponseData(void* data, int length) : RfxBaseData(data, length) {
    m_length = length;
    if ((m_length / sizeof(RIL_RsuResponse)) == 0 || data == NULL) {
        return;
    }

    RIL_RsuResponse* dupData;
    RIL_RsuResponse* srcData = (RIL_RsuResponse*)data;
    int strLength = -1;

    dupData = (RIL_RsuResponse*)calloc(1, sizeof(RIL_RsuResponse));
    RFX_ASSERT(dupData != NULL);
    memcpy(dupData, srcData, sizeof(RIL_RsuResponse));

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

RfxRsuResponseData::~RfxRsuResponseData() {
    RIL_RsuResponse* data = (RIL_RsuResponse*)m_data;
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
