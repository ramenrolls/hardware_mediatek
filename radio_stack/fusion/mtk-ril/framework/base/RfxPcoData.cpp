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

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "RfxPcoData.h"

/*****************************************************************************
 * Class RfxPcoData
 *****************************************************************************/
RFX_IMPLEMENT_DATA_CLASS(RfxPcoData);

RfxPcoData::RfxPcoData(void* data, int length) : RfxBaseData(data, length) {
    if (data != NULL) {
        RIL_PCO_Data* pdata = (RIL_PCO_Data*)data;
        RIL_PCO_Data* response = (RIL_PCO_Data*)calloc(1, sizeof(RIL_PCO_Data));
        RFX_ASSERT(response != NULL);

        response->cid = pdata->cid;
        copyString(&(response->bearer_proto), pdata->bearer_proto);
        response->pco_id = pdata->pco_id;
        response->contents_length = pdata->contents_length;
        copyString(&(response->contents), pdata->contents);

        m_data = response;
        m_length = length;
    }
}

RfxPcoData::~RfxPcoData() {
    RIL_PCO_Data* pdata = (RIL_PCO_Data*)m_data;
    free(pdata->bearer_proto);
    free(pdata->contents);
    free(pdata);
}

void RfxPcoData::copyString(char** dst, char* src) {
    RFX_ASSERT(dst != NULL);
    if (src != NULL) {
        *dst = (char*)calloc(strlen(src) + 1, sizeof(char));
        RFX_ASSERT((*dst) != NULL);
        strncpy(*dst, src, strlen(src));
    } else {
        *dst = (char*)calloc(1, sizeof(char));
        RFX_ASSERT((*dst) != NULL);
        (*dst)[0] = 0;
    }
}
