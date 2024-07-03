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

#include "RfxImsConfData.h"

RFX_IMPLEMENT_DATA_CLASS(RfxImsConfData);

RfxImsConfData::RfxImsConfData(void* data, int length) : RfxBaseData(data, length) {
    if (data == NULL) {
        m_length = 0;
        m_data = NULL;
        return;
    }

    m_length = length;
    m_data = NULL;
    RIL_Conference_Participants* srcData = (RIL_Conference_Participants*)data;
    int count = m_length / sizeof(RIL_Conference_Participants);
    RIL_Conference_Participants* dupData =
            (RIL_Conference_Participants*)calloc(count, sizeof(RIL_Conference_Participants));
    RFX_ASSERT(dupData != NULL);
    for (int i = 0; i < count; i++) {
        int len = strlen(srcData[i].useraddr);
        dupData[i].useraddr = (char*)calloc(len + 1, sizeof(char));
        RFX_ASSERT(dupData[i].useraddr != NULL);
        strncpy(dupData[i].useraddr, srcData[i].useraddr, len);
        len = strlen(srcData[i].end_point);
        dupData[i].end_point = (char*)calloc(strlen(srcData[i].end_point) + 1, sizeof(char));
        RFX_ASSERT(dupData[i].end_point != NULL);
        strncpy(dupData[i].end_point, srcData[i].end_point, len);
        len = strlen(srcData[i].entity);
        dupData[i].entity = (char*)calloc(strlen(srcData[i].entity) + 1, sizeof(char));
        RFX_ASSERT(dupData[i].entity != NULL);
        strncpy(dupData[i].entity, srcData[i].entity, len);
        len = strlen(srcData[i].display_text);
        dupData[i].display_text = (char*)calloc(strlen(srcData[i].display_text) + 1, sizeof(char));
        RFX_ASSERT(dupData[i].display_text != NULL);
        strncpy(dupData[i].display_text, srcData[i].display_text, len);
        len = strlen(srcData[i].status);
        dupData[i].status = (char*)calloc(strlen(srcData[i].status) + 1, sizeof(char));
        RFX_ASSERT(dupData[i].status != NULL);
        strncpy(dupData[i].status, srcData[i].status, len);
    }
    m_data = dupData;
}

RfxImsConfData::~RfxImsConfData() {
    // free
    int count = m_length / sizeof(RIL_Conference_Participants);
    RIL_Conference_Participants* dupData = (RIL_Conference_Participants*)m_data;
    if (dupData != NULL) {
        for (int i = 0; i < count; i++) {
            free(dupData[i].useraddr);
            free(dupData[i].end_point);
            free(dupData[i].display_text);
            free(dupData[i].status);
            free(dupData[i].entity);
        }
        free(dupData);
    }
    m_length = 0;
    m_data = NULL;
}
