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

#include "RfxIntsData.h"

RFX_IMPLEMENT_DATA_CLASS(RfxIntsData);

#define RFX_LOG_TAG "RfxIntsData"

RfxIntsData::RfxIntsData() : RfxBaseData(NULL, 0) {}

RfxIntsData::RfxIntsData(void* data, int length) : RfxBaseData(data, length) {
    if (data != NULL) {
        int num = length / sizeof(int);
        int* pTmp = (int*)calloc(num, sizeof(int));
        if (pTmp == NULL) {
            RFX_LOG_E(RFX_LOG_TAG, "OOM");
            return;
        }
        int* pCur = (int*)data;
        m_length = length;

        for (int i = 0; i < num; i++) {
            pTmp[i] = pCur[i];
        }
        m_data = pTmp;
    }
}

RfxIntsData::RfxIntsData(int data[], int count) : RfxBaseData(data, count * sizeof(int)) {
    if (data != NULL) {
        int* pTmp = (int*)calloc(count, sizeof(int));
        if (pTmp == NULL) {
            RFX_LOG_E(RFX_LOG_TAG, "OOM");
            return;
        }
        for (int i = 0; i < count; i++) {
            pTmp[i] = data[i];
        }
        m_data = pTmp;
        m_length = count * sizeof(int);
    }
}

RfxIntsData::~RfxIntsData() {
    // free memory
    if (m_data) {
        free(m_data);
    }
}
