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
#include "RfxPcoIaData.h"

/*****************************************************************************
 * Class RfxPcoIaData
 *****************************************************************************/
RFX_IMPLEMENT_DATA_CLASS(RfxPcoIaData);

RfxPcoIaData::RfxPcoIaData(void* data, int length) : RfxBaseData(data, length) {
    if (data != NULL) {
        RIL_PCO_Data_attached* pdata = (RIL_PCO_Data_attached*)data;
        RIL_PCO_Data_attached* response =
                (RIL_PCO_Data_attached*)calloc(1, sizeof(RIL_PCO_Data_attached));
        RFX_ASSERT(response != NULL);

        response->cid = pdata->cid;
        copyString(&(response->apn_name), pdata->apn_name);
        copyString(&(response->bearer_proto), pdata->bearer_proto);
        response->pco_id = pdata->pco_id;
        response->contents_length = pdata->contents_length;
        copyString(&(response->contents), pdata->contents);

        m_data = response;
        m_length = length;
    }
}

RfxPcoIaData::~RfxPcoIaData() {
    RIL_PCO_Data_attached* pdata = (RIL_PCO_Data_attached*)m_data;
    free(pdata->apn_name);
    free(pdata->bearer_proto);
    free(pdata->contents);
    free(pdata);
}

void RfxPcoIaData::copyString(char** dst, char* src) {
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
