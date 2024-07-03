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

#include "RfxCrssNotificationData.h"

RFX_IMPLEMENT_DATA_CLASS(RfxCrssNotificationData);

RfxCrssNotificationData::RfxCrssNotificationData(void* data, int length)
    : RfxBaseData(data, length) {
    if (data == NULL) {
        m_data = NULL;
        return;
    }

    m_length = length;
    RIL_CrssNotification* dupData;
    RIL_CrssNotification* srcData = (RIL_CrssNotification*)data;
    int len;

    dupData = (RIL_CrssNotification*)calloc(1, sizeof(RIL_CrssNotification));
    RFX_ASSERT(dupData != NULL);
    memset(dupData, 0x00, sizeof(RIL_CrssNotification));
    m_data = dupData;

    dupData->code = srcData->code;
    dupData->type = srcData->type;
    dupData->cli_validity = srcData->cli_validity;

    if (srcData->number != NULL) {
        len = strlen(srcData->number);
        dupData->number = (char*)calloc(len + 1, sizeof(char));
        RFX_ASSERT(dupData->number != NULL);
        strncpy(dupData->number, srcData->number, len);
    }
    if (srcData->alphaid != NULL) {
        len = strlen(srcData->alphaid);
        dupData->alphaid = (char*)calloc(len + 1, sizeof(char));
        RFX_ASSERT(dupData->alphaid != NULL);
        strncpy(dupData->alphaid, srcData->alphaid, len);
    }
}

RfxCrssNotificationData::~RfxCrssNotificationData() {
    // free
    RIL_CrssNotification* tmp = (RIL_CrssNotification*)m_data;
    if (tmp != NULL) {
        if (tmp->number != NULL) {
            free(tmp->number);
        }
        if (tmp->alphaid != NULL) {
            free(tmp->alphaid);
        }
        free(tmp);
    }
    m_data = NULL;
}
