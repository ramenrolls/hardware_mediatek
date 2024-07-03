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

#include "RfxKeepaliveRequestData.h"
#include <string.h>

#define RFX_LOG_TAG "RfxKeepaliveRequestData"

RFX_IMPLEMENT_DATA_CLASS(RfxKeepaliveRequestData);

RfxKeepaliveRequestData::RfxKeepaliveRequestData(void* data, int length)
    : RfxBaseData(data, length) {
    if (data != NULL) {
        RIL_KeepaliveRequest* pInput = (RIL_KeepaliveRequest*)data;
        RIL_KeepaliveRequest* pData =
                (RIL_KeepaliveRequest*)calloc(1, sizeof(RIL_KeepaliveRequest));
        RFX_ASSERT(pData != NULL);

        pData->type = pInput->type;
        memcpy(pData->sourceAddress, pInput->sourceAddress, MAX_INADDR_LEN);
        pData->sourcePort = pInput->sourcePort;
        memcpy(pData->destinationAddress, pInput->destinationAddress, MAX_INADDR_LEN);
        pData->destinationPort = pInput->destinationPort;
        pData->maxKeepaliveIntervalMillis = pInput->maxKeepaliveIntervalMillis;
        pData->cid = pInput->cid;

        m_data = (void*)pData;
        m_length = length;
    }
}

RfxKeepaliveRequestData::~RfxKeepaliveRequestData() {
    // free
    if (m_data != NULL) {
        free(m_data);
        m_data = NULL;
    }
}
