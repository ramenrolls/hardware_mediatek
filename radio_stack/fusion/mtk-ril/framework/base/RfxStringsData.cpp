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

#include "RfxStringsData.h"

RFX_IMPLEMENT_DATA_CLASS(RfxStringsData);

RfxStringsData::RfxStringsData() : RfxBaseData(NULL, 0) {}

RfxStringsData::RfxStringsData(void* data, int length) : RfxBaseData(data, length) {
    if (data != NULL) {
        m_length = length;
        int countStrings = length / sizeof(char*);
        copyStrings((char**)data, countStrings);
    }
}

RfxStringsData::RfxStringsData(char** data, int countStrings)
    : RfxBaseData(data, countStrings * sizeof(char*)) {
    if (data != NULL) {
        m_length = countStrings * sizeof(char*);
        copyStrings(data, countStrings);
    }
}

void RfxStringsData::copyStrings(char** data, int countStrings) {
    char** pStrings = (char**)calloc(countStrings, sizeof(char*));
    if (pStrings != NULL) {
        for (int i = 0; i < countStrings; i++) {
            if (data[i] != NULL) {
                pStrings[i] = strdup(data[i]);
            } else {
                pStrings[i] = NULL;
            }
        }
        m_data = pStrings;
    }
}

RfxStringsData::~RfxStringsData() {
    // free
    if (m_data) {
        char** pTmp = (char**)m_data;
        int countStrings = m_length / sizeof(char*);
        for (int i = 0; i < countStrings; i++) {
            if (pTmp[i]) {
                free(pTmp[i]);
            }
        }
        free(m_data);
    }
}
