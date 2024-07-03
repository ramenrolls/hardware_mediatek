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

#include "RfxSimStatusData.h"
#include <telephony/mtk_ril.h>

RFX_IMPLEMENT_DATA_CLASS(RfxSimStatusData);

RfxSimStatusData::RfxSimStatusData(void* _data, int _length) : RfxBaseData(_data, _length) {
    if (_data != NULL) {
        RIL_CardStatus_v8* pCard = (RIL_CardStatus_v8*)_data;
        RIL_CardStatus_v8* pData = (RIL_CardStatus_v8*)calloc(1, sizeof(RIL_CardStatus_v8));
        RFX_ASSERT(pData != NULL);
        pData->card_state = pCard->card_state;
        pData->universal_pin_state = pCard->universal_pin_state;
        pData->gsm_umts_subscription_app_index = pCard->gsm_umts_subscription_app_index;
        pData->ims_subscription_app_index = pCard->ims_subscription_app_index;
        pData->cdma_subscription_app_index = pCard->cdma_subscription_app_index;
        pData->num_applications = pCard->num_applications;

        for (int i = 0; i < pData->num_applications; i++) {
            pData->applications[i].app_type = pCard->applications[i].app_type;
            pData->applications[i].app_state = pCard->applications[i].app_state;
            pData->applications[i].perso_substate = pCard->applications[i].perso_substate;
            if (pCard->applications[i].aid_ptr != NULL) {
                asprintf(&pData->applications[i].aid_ptr, "%s", pCard->applications[i].aid_ptr);
            }
            if (pCard->applications[i].app_label_ptr != NULL) {
                asprintf(&pData->applications[i].app_label_ptr, "%s",
                         pCard->applications[i].app_label_ptr);
            }
            pData->applications[i].pin1 = pCard->applications[i].pin1;
            pData->applications[i].pin1_replaced = pCard->applications[i].pin1_replaced;
            pData->applications[i].pin2 = pCard->applications[i].pin2;
        }

        // Parameter add from android radio hidl v1.2
        pData->physicalSlotId = pCard->physicalSlotId;
        if (pCard->atr != NULL) {
            asprintf(&pData->atr, "%s", pCard->atr);
        }
        if (pCard->iccId != NULL) {
            asprintf(&pData->iccId, "%s", pCard->iccId);
        }

        // Parameter add from android radio hidl v1.4
        if (pCard->eid != NULL) {
            asprintf(&pData->eid, "%s", pCard->eid);
        }

        m_data = (void*)pData;
        m_length = _length;
    }
}

RfxSimStatusData::~RfxSimStatusData() {
    if (m_data != NULL) {
        RIL_CardStatus_v8* pData = (RIL_CardStatus_v8*)m_data;

        for (int i = 0; i < pData->num_applications; i++) {
            if (pData->applications[i].aid_ptr != NULL) {
                free(pData->applications[i].aid_ptr);
                pData->applications[i].aid_ptr = NULL;
            }
            if (pData->applications[i].app_label_ptr != NULL) {
                free(pData->applications[i].app_label_ptr);
                pData->applications[i].app_label_ptr = NULL;
            }
        }

        // Parameter add from android radio hidl v1.2
        if (pData->atr != NULL) {
            free(pData->atr);
            pData->atr = NULL;
        }
        if (pData->iccId != NULL) {
            free(pData->iccId);
            pData->iccId = NULL;
        }

        // Parameter add from android radio hidl v1.4
        if (pData->eid != NULL) {
            free(pData->eid);
            pData->eid = NULL;
        }

        // free memory
        free(m_data);
        m_data = NULL;
    }
}
