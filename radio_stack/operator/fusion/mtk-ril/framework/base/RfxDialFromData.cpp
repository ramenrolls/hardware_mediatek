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

#include "RfxDialFromData.h"

RFX_IMPLEMENT_DATA_CLASS(RfxDialFromData);

RfxDialFromData::RfxDialFromData(void* data, int length) : RfxBaseData(data, length) {
    m_length = length;
    RIL_DialFrom* dupData;
    RIL_DialFrom* srcData = (RIL_DialFrom*)data;
    int strLength;

    dupData = (RIL_DialFrom*)calloc(1, sizeof(RIL_DialFrom));
    RFX_ASSERT(dupData != NULL);
    memset(dupData, 0x00, sizeof(RIL_DialFrom));
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

    /* Copy fromAddress(dial from string) */
    if (srcData->fromAddress == NULL) {
        return;
    }
    strLength = strlen(srcData->fromAddress) + 1;
    dupData->fromAddress = (char*)calloc(strLength, sizeof(char));
    RFX_ASSERT(dupData->fromAddress != NULL);
    memset(dupData->fromAddress, 0, strLength);
    memcpy(dupData->fromAddress, srcData->fromAddress, strLength);

    /* CLIR setting */
    dupData->clir = srcData->clir;

    /* is video call */
    dupData->isVideoCall = srcData->isVideoCall;
}

RfxDialFromData::~RfxDialFromData() {
    // free
    RIL_DialFrom* data = (RIL_DialFrom*)m_data;
    if (data != NULL) {
        if (data->address != NULL) {
            free(data->address);
        }
        if (data->fromAddress != NULL) {
            free(data->fromAddress);
        }
        free(m_data);
    }
}
