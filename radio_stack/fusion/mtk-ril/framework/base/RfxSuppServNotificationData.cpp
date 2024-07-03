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

#include "RfxSuppServNotificationData.h"

RFX_IMPLEMENT_DATA_CLASS(RfxSuppServNotificationData);

RfxSuppServNotificationData::RfxSuppServNotificationData(void* data, int length)
    : RfxBaseData(data, length) {
    if (data == NULL) {
        m_data = NULL;
        return;
    }

    m_length = length;
    RIL_SuppSvcNotification* dupData;
    RIL_SuppSvcNotification* srcData = (RIL_SuppSvcNotification*)data;

    dupData = (RIL_SuppSvcNotification*)calloc(1, sizeof(RIL_SuppSvcNotification));
    RFX_ASSERT(dupData != NULL);
    memset(dupData, 0x00, sizeof(RIL_SuppSvcNotification));
    m_data = dupData;

    dupData->notificationType = srcData->notificationType;
    dupData->code = srcData->code;
    dupData->index = srcData->index;
    dupData->type = srcData->type;

    if (srcData->number != NULL) {
        int len = strlen(srcData->number);
        dupData->number = (char*)calloc(len + 1, sizeof(char));
        RFX_ASSERT(dupData->number != NULL);
        strncpy(dupData->number, srcData->number, len);
    }
}

RfxSuppServNotificationData::~RfxSuppServNotificationData() {
    // free
    RIL_SuppSvcNotification* tmp = (RIL_SuppSvcNotification*)m_data;
    if (tmp != NULL) {
        if (tmp->number != NULL) {
            free(tmp->number);
        }
        free(tmp);
    }
    m_data = NULL;
}
