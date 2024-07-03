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

#include "RfxDialData.h"

RFX_IMPLEMENT_DATA_CLASS(RfxDialData);

RfxDialData::RfxDialData(void* data, int length) : RfxBaseData(data, length) {
    m_length = length;
    RIL_Dial* dupData;
    RIL_Dial* srcData = (RIL_Dial*)data;
    int strLength;

    dupData = (RIL_Dial*)calloc(1, sizeof(RIL_Dial));
    RFX_ASSERT(dupData != NULL);
    memset(dupData, 0x00, sizeof(RIL_Dial));
    m_data = dupData;

    /* Copy address(dial string) */
    if (srcData->address == NULL) {
        return;
    }
    strLength = strlen(srcData->address) + 1;
    dupData->address = (char*)calloc(strLength, sizeof(char));
    RFX_ASSERT(dupData->address != NULL);
    memset(dupData->address, 0, strLength);
    memcpy(dupData->address, srcData->address, strLength);

    /* CLIR setting */
    dupData->clir = srcData->clir;

    /* uusPresent */
    dupData->uusInfo = NULL;
    if (srcData->uusInfo != NULL) {
        dupData->uusInfo = (RIL_UUS_Info*)calloc(1, sizeof(RIL_UUS_Info));
        RFX_ASSERT(dupData->uusInfo != NULL);
        dupData->uusInfo->uusType = (RIL_UUS_Type)srcData->uusInfo->uusType;
        dupData->uusInfo->uusDcs = (RIL_UUS_DCS)srcData->uusInfo->uusDcs;
        dupData->uusInfo->uusLength = srcData->uusInfo->uusLength;
        if (dupData->uusInfo->uusLength == -1) {
            dupData->uusInfo->uusData = NULL;
        } else {
            dupData->uusInfo->uusData =
                    (char*)calloc(dupData->uusInfo->uusLength + 1, sizeof(char));
            RFX_ASSERT(dupData->uusInfo->uusData != NULL);
            memset(dupData->uusInfo->uusData, 0, dupData->uusInfo->uusLength + 1);
            memcpy(dupData->uusInfo->uusData, srcData->uusInfo->uusData,
                   dupData->uusInfo->uusLength + 1);
        }
    }
}

RfxDialData::~RfxDialData() {
    // free
    RIL_Dial* data = (RIL_Dial*)m_data;
    if (data != NULL) {
        if (data->address != NULL) {
            free(data->address);
        }
        if (data->uusInfo != NULL) {
            if (data->uusInfo->uusData != NULL) {
                free(data->uusInfo->uusData);
            }
            free(data->uusInfo);
        }
        free(m_data);
    }
}
