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

#include <string.h>
#include "RfxIaApnData.h"
#include "RfxLog.h"

RFX_IMPLEMENT_DATA_CLASS(RfxIaApnData);

#define RFX_LOG_TAG "RfxIaApnData"

RfxIaApnData::RfxIaApnData(void* data, int length) : RfxBaseData(data, length) {
    m_length = length;
    RFX_ASSERT(m_length == sizeof(RIL_InitialAttachApn_v15));

    int result;

    RIL_InitialAttachApn_v15* pTmp =
            (RIL_InitialAttachApn_v15*)calloc(1, sizeof(RIL_InitialAttachApn_v15));
    if (pTmp == NULL) {
        m_data = NULL;
        return;
    }
    RIL_InitialAttachApn_v15* pCur = (RIL_InitialAttachApn_v15*)data;

    copyString(&(pTmp->apn), pCur->apn);
    copyString(&(pTmp->protocol), pCur->protocol);
    copyString(&(pTmp->roamingProtocol), pCur->roamingProtocol);
    copyString(&(pTmp->username), pCur->username);
    copyString(&(pTmp->password), pCur->password);

    pTmp->authtype = pCur->authtype;
    m_data = pTmp;
}

RfxIaApnData::~RfxIaApnData() {
    RIL_InitialAttachApn_v15* pTmp = (RIL_InitialAttachApn_v15*)m_data;
    // free memory
    if (pTmp != NULL) {
        if (pTmp->apn != NULL) {
            free(pTmp->apn);
        }
        if (pTmp->protocol != NULL) {
            free(pTmp->protocol);
        }
        if (pTmp->roamingProtocol != NULL) {
            free(pTmp->roamingProtocol);
        }
        if (pTmp->username != NULL) {
            free(pTmp->username);
        }
        if (pTmp->password != NULL) {
            free(pTmp->password);
        }
        free(pTmp);
    }
}

void RfxIaApnData::copyString(char** dst, char* src) {
    if (src != NULL) {
        *dst = (char*)calloc(strlen(src) + 1, sizeof(char));
        if (*dst == NULL) {
            RFX_LOG_E(RFX_LOG_TAG, "OOM");
            return;
        }
        strncpy(*dst, src, strlen(src));
    } else {
        *dst = (char*)calloc(1, sizeof(char));
        if (*dst == NULL) {
            RFX_LOG_E(RFX_LOG_TAG, "OOM");
            return;
        }
        (*dst)[0] = 0;
    }
}
