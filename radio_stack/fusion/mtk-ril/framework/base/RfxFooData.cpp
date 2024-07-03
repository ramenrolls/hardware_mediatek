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

#include "RfxFooData.h"

RFX_IMPLEMENT_DATA_CLASS(RfxFooData);

RfxFooData::RfxFooData(void* data, int length) : RfxBaseData(data, length) {
    RIL_Foo* pTmp = (RIL_Foo*)data;
    m_length = length;
    RIL_Foo* res = (RIL_Foo*)calloc(1, sizeof(RIL_Foo));
    if (res == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "OOM");
        return;
    }
    res->timeout = pTmp->timeout;
    res->power = pTmp->power;
    m_data = res;
}

RfxFooData::RfxFooData(int timeout, int power) : RfxBaseData(NULL, 0) {
    m_length = sizeof(RIL_Foo);
    RIL_Foo* res = (RIL_Foo*)calloc(1, sizeof(RIL_Foo));
    if (res == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "OOM");
        return;
    }
    res->timeout = timeout;
    res->power = power;
    m_data = res;
}

RfxFooData::~RfxFooData() { free(m_data); }
