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

#include "RfxEmergencyDialData.h"

RFX_IMPLEMENT_DATA_CLASS(RfxEmergencyDialData);

RfxEmergencyDialData::RfxEmergencyDialData(void* data, int length) : RfxBaseData(data, length) {
    m_length = length;
    RIL_Emergency_Dial* dupData;
    RIL_Emergency_Dial* srcData = (RIL_Emergency_Dial*)data;
    int strLength;

    dupData = (RIL_Emergency_Dial*)calloc(1, sizeof(RIL_Emergency_Dial));
    RFX_ASSERT(dupData != NULL);
    memset(dupData, 0x00, sizeof(RIL_Emergency_Dial));
    m_data = dupData;

    /* Copy address(dial string) */
    if (srcData->dialData->address == NULL) {
        return;
    }

    dupData->dialData = (RIL_Dial*)calloc(1, sizeof(RIL_Dial));
    RFX_ASSERT(dupData->dialData != NULL);

    strLength = strlen(srcData->dialData->address) + 1;
    dupData->dialData->address = (char*)calloc(strLength, sizeof(char));
    RFX_ASSERT(dupData->dialData->address != NULL);
    memset(dupData->dialData->address, 0, strLength);
    memcpy(dupData->dialData->address, srcData->dialData->address, strLength);

    /* CLIR setting */
    dupData->dialData->clir = srcData->dialData->clir;

    /* uusPresent */
    dupData->dialData->uusInfo = NULL;
    if (srcData->dialData->uusInfo != NULL) {
        dupData->dialData->uusInfo = (RIL_UUS_Info*)calloc(1, sizeof(RIL_UUS_Info));
        RFX_ASSERT(dupData->dialData->uusInfo != NULL);
        dupData->dialData->uusInfo->uusType = (RIL_UUS_Type)srcData->dialData->uusInfo->uusType;
        dupData->dialData->uusInfo->uusDcs = (RIL_UUS_DCS)srcData->dialData->uusInfo->uusDcs;
        dupData->dialData->uusInfo->uusLength = srcData->dialData->uusInfo->uusLength;
        if (dupData->dialData->uusInfo->uusLength == -1) {
            dupData->dialData->uusInfo->uusData = NULL;
        } else {
            dupData->dialData->uusInfo->uusData =
                    (char*)calloc(dupData->dialData->uusInfo->uusLength + 1, sizeof(char));
            RFX_ASSERT(dupData->dialData->uusInfo->uusData != NULL);
            memset(dupData->dialData->uusInfo->uusData, 0,
                   dupData->dialData->uusInfo->uusLength + 1);
            memcpy(dupData->dialData->uusInfo->uusData, srcData->dialData->uusInfo->uusData,
                   dupData->dialData->uusInfo->uusLength + 1);
        }
    }

    /* EmergencyServiceCategory */
    dupData->serviceCategory = srcData->serviceCategory;

    /* EmergencyCallRouting */
    dupData->routing = srcData->routing;

    /* isTesting */
    dupData->isTesting = srcData->isTesting;
}

RfxEmergencyDialData::~RfxEmergencyDialData() {
    // free
    RIL_Emergency_Dial* data = (RIL_Emergency_Dial*)m_data;
    if (data != NULL) {
        if (data->dialData->address != NULL) {
            free(data->dialData->address);
        }
        if (data->dialData->uusInfo != NULL) {
            if (data->dialData->uusInfo->uusData != NULL) {
                free(data->dialData->uusInfo->uusData);
            }
            free(data->dialData->uusInfo);
        }
        free(m_data);
    }
}
