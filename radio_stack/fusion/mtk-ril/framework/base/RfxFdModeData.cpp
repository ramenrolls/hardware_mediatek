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

#include "RfxFdModeData.h"

RFX_IMPLEMENT_DATA_CLASS(RfxFdModeData);

RfxFdModeData::RfxFdModeData(void* data, int length) : RfxBaseData(data, length) {
    m_length = length;
    RIL_FdModeStructure* pTmp = (RIL_FdModeStructure*)data;
    RIL_FdModeStructure* pData = (RIL_FdModeStructure*)calloc(1, sizeof(RIL_FdModeStructure));
    if (pData != NULL) {
        pData->paramNumber = pTmp->paramNumber;
        pData->mode = pTmp->mode;
        pData->parameter1 = pTmp->parameter1;
        pData->parameter2 = pTmp->parameter2;
        m_data = pData;
    }
}

RfxFdModeData::~RfxFdModeData() {
    free(m_data);
    m_data = NULL;
}
