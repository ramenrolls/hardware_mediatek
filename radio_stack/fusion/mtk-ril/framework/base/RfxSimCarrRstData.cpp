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

#include "RfxSimCarrRstData.h"
#include <telephony/mtk_ril.h>
#include "RfxLog.h"

#define RFX_LOG_TAG "RfxSimCarrRstData"

RFX_IMPLEMENT_DATA_CLASS(RfxSimCarrRstData);

RfxSimCarrRstData::RfxSimCarrRstData(void* _data, int _length) : RfxBaseData(_data, _length) {
    if (_data != NULL) {
        RIL_CarrierRestrictionsWithPriority* pCarrRst = (RIL_CarrierRestrictionsWithPriority*)_data;
        RIL_CarrierRestrictionsWithPriority* pData = (RIL_CarrierRestrictionsWithPriority*)calloc(
                1, sizeof(RIL_CarrierRestrictionsWithPriority));
        RFX_ASSERT(pData != NULL);

        RIL_Carrier* allowedCarriers = NULL;
        RIL_Carrier* excludedCarriers = NULL;

        pData->len_allowed_carriers = pCarrRst->len_allowed_carriers;
        allowedCarriers = (RIL_Carrier*)calloc(pData->len_allowed_carriers, sizeof(RIL_Carrier));
        RFX_ASSERT(allowedCarriers != NULL);
        pData->allowed_carriers = allowedCarriers;

        pData->len_excluded_carriers = pCarrRst->len_excluded_carriers;
        excludedCarriers = (RIL_Carrier*)calloc(pData->len_excluded_carriers, sizeof(RIL_Carrier));
        RFX_ASSERT(excludedCarriers != NULL);
        pData->excluded_carriers = excludedCarriers;

        for (int i = 0; i < pData->len_allowed_carriers; i++) {
            RIL_Carrier* item = pCarrRst->allowed_carriers + i;
            if (item->mcc != NULL) {
                asprintf(&allowedCarriers[i].mcc, "%s", item->mcc);
            }
            if (item->mnc != NULL) {
                asprintf(&allowedCarriers[i].mnc, "%s", item->mnc);
            }
            allowedCarriers[i].match_type = item->match_type;
            if (item->match_data != NULL) {
                asprintf(&allowedCarriers[i].match_data, "%s", item->match_data);
            }
            RFX_LOG_I(RFX_LOG_TAG, "allowedCarriers %s, %s, %d, %s", allowedCarriers[i].mcc,
                      allowedCarriers[i].mnc, allowedCarriers[i].match_type,
                      allowedCarriers[i].match_data);
        }

        for (int i = 0; i < pData->len_excluded_carriers; i++) {
            RIL_Carrier* item = pCarrRst->excluded_carriers + i;
            if (item->mcc != NULL) {
                asprintf(&excludedCarriers[i].mcc, "%s", item->mcc);
            }
            if (item->mnc != NULL) {
                asprintf(&excludedCarriers[i].mnc, "%s", item->mnc);
            }
            excludedCarriers[i].match_type = item->match_type;
            if (item->match_data != NULL) {
                asprintf(&excludedCarriers[i].match_data, "%s", item->match_data);
            }
            RFX_LOG_I(RFX_LOG_TAG, "excludedCarriers %s, %s, %d, %s", excludedCarriers[i].mcc,
                      excludedCarriers[i].mnc, excludedCarriers[i].match_type,
                      excludedCarriers[i].match_data);
        }

        pData->allowedCarriersPrioritized = pCarrRst->allowedCarriersPrioritized;
        pData->simLockMultiSimPolicy = pCarrRst->simLockMultiSimPolicy;
        RFX_LOG_E(RFX_LOG_TAG, "pData Allowed %d, Excluded %d, Prioritized %d, MultiSIMPolicy %d",
                  pData->len_allowed_carriers, pData->len_excluded_carriers,
                  pData->allowedCarriersPrioritized, pData->simLockMultiSimPolicy);
        m_data = (void*)pData;
        m_length = _length;
    }
}

RfxSimCarrRstData::~RfxSimCarrRstData() {
    // free memory
    if (m_data != NULL) {
        RIL_CarrierRestrictionsWithPriority* pData = (RIL_CarrierRestrictionsWithPriority*)m_data;
        if (pData->allowed_carriers != NULL) {
            for (int i = 0; i < pData->len_allowed_carriers; i++) {
                RIL_Carrier* item = pData->allowed_carriers + i;
                if (item->mcc != NULL) {
                    free((void*)item->mcc);
                    item->mcc = NULL;
                }
                if (item->mnc != NULL) {
                    free((void*)item->mnc);
                    item->mnc = NULL;
                }
                if (item->match_data != NULL) {
                    free((void*)item->match_data);
                    item->match_data = NULL;
                }
            }
            free(pData->allowed_carriers);
            pData->allowed_carriers = NULL;
        }
        if (pData->excluded_carriers != NULL) {
            for (int i = 0; i < pData->len_excluded_carriers; i++) {
                RIL_Carrier* item = pData->excluded_carriers + i;
                if (item->mcc != NULL) {
                    free((void*)item->mcc);
                    item->mcc = NULL;
                }
                if (item->mnc != NULL) {
                    free((void*)item->mnc);
                    item->mnc = NULL;
                }
                if (item->match_data != NULL) {
                    free((void*)item->match_data);
                    item->match_data = NULL;
                }
            }
            free(pData->excluded_carriers);
            pData->excluded_carriers = NULL;
        }
        free(m_data);
        m_data = NULL;
    }
}
