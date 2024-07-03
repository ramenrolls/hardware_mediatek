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

#include "RfxNetworkScanData.h"

RFX_IMPLEMENT_DATA_CLASS(RfxNetworkScanData);

RfxNetworkScanData::RfxNetworkScanData(void* data, int length) : RfxBaseData(data, length) {
    RIL_NetworkScanRequest* pTmp = (RIL_NetworkScanRequest*)data;
    RIL_NetworkScanRequest* pData =
            (RIL_NetworkScanRequest*)calloc(1, sizeof(RIL_NetworkScanRequest));

    if (pData != NULL) {
        pData->type = pTmp->type;
        pData->interval = pTmp->interval;
        pData->specifiers_length = pTmp->specifiers_length;

        for (int i = 0; i < (int)pData->specifiers_length; i++) {
            pData->specifiers[i].radio_access_network = pTmp->specifiers[i].radio_access_network;
            pData->specifiers[i].bands_length = pTmp->specifiers[i].bands_length;

            if (pData->specifiers[i].radio_access_network == GERAN) {
                memcpy(&pData->specifiers[i].bands.geran_bands,
                       &pTmp->specifiers[i].bands.geran_bands, MAX_BANDS * sizeof(RIL_GeranBands));
            } else if (pData->specifiers[i].radio_access_network == UTRAN) {
                memcpy(pData->specifiers[i].bands.utran_bands,
                       pTmp->specifiers[i].bands.utran_bands, MAX_BANDS * sizeof(RIL_UtranBands));
            } else {
                memcpy(pData->specifiers[i].bands.eutran_bands,
                       pTmp->specifiers[i].bands.eutran_bands, MAX_BANDS * sizeof(RIL_EutranBands));
            }

            pData->specifiers[i].channels_length = pTmp->specifiers[i].channels_length;
            memcpy(pData->specifiers[i].channels, pTmp->specifiers[i].channels,
                   MAX_CHANNELS * sizeof(uint32_t));
        }
        pData->maxSearchTime = pTmp->maxSearchTime;
        pData->incrementalResults = pTmp->incrementalResults;
        pData->incrementalResultsPeriodicity = pTmp->incrementalResultsPeriodicity;
        pData->mccMncs_length = pTmp->mccMncs_length;
        pData->mccMncs = (char**)calloc(pData->mccMncs_length, sizeof(char*));
        RFX_ASSERT(pData->mccMncs != NULL);
        memset(pData->mccMncs, 0, pData->mccMncs_length);
        for (size_t i = 0; i < pData->mccMncs_length; i++) {
            asprintf(&(pData->mccMncs[i]), "%s", pTmp->mccMncs[i]);
        }

        m_data = pData;
        m_length = length;
    }
}

RfxNetworkScanData::~RfxNetworkScanData() {
    RIL_NetworkScanRequest* pData = (RIL_NetworkScanRequest*)m_data;
    if (pData) {
        if (pData->mccMncs_length > 0 && pData->mccMncs != NULL) {
            for (size_t i = 0; i < pData->mccMncs_length; i++) {
                if (pData->mccMncs[i] != NULL) free(pData->mccMncs[i]);
                pData->mccMncs[i] = NULL;
            }
            pData->mccMncs = NULL;
        }
    }
    free(m_data);
    m_data = NULL;
}
