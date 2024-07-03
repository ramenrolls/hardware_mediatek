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

#include "RfxRawData.h"
#include "RfxMessageId.h"
#include "RfxDataCloneManager.h"

#define RFX_LOG_TAG "RfxRawData"

RFX_IMPLEMENT_DATA_CLASS(RfxRawData);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxRawData, RFX_MSG_EVENT_IMS_DATA);

RfxRawData::RfxRawData() : RfxBaseData(NULL, 0) {}

RfxRawData::RfxRawData(void* data, int length) : RfxBaseData(data, length) {
    copyRaw((char*)data, length);
}

RfxRawData::RfxRawData(char data[], int length) : RfxBaseData(data, length) {
    copyRaw(data, length);
}

void RfxRawData::copyRaw(char* data, int length) {
    if (data != NULL) {
        m_length = length;
        char* pTmp = (char*)calloc(1, length + 1);
        if (pTmp == NULL) {
            RFX_LOG_E(RFX_LOG_TAG, "OOM");
            return;
        }
        memcpy(pTmp, data, m_length);
        pTmp[length] = '\0';
        m_data = pTmp;
    }
}

RfxRawData::~RfxRawData() {
    // free memory
    if (m_data) {
        free(m_data);
    }
}
