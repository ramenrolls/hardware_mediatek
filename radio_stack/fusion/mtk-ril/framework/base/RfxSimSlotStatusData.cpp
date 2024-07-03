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

#include "RfxSimSlotStatusData.h"
#include <telephony/mtk_ril.h>

RFX_IMPLEMENT_DATA_CLASS(RfxSimSlotStatusData);

RfxSimSlotStatusData::RfxSimSlotStatusData(void* _data, int _length) : RfxBaseData(_data, _length) {
    if (_data == NULL) {
        m_data = NULL;
        return;
    }

    m_data = NULL;
    m_length = _length;
    int countOfSlot = m_length / sizeof(RIL_SimSlotStatus*);
    RIL_SimSlotStatus** pp_slot_status =
            (RIL_SimSlotStatus**)calloc(countOfSlot, sizeof(RIL_SimSlotStatus*));
    RFX_ASSERT(pp_slot_status != NULL);
    if (countOfSlot > 0) {
        RIL_SimSlotStatus** tmp = (RIL_SimSlotStatus**)_data;
        for (int i = 0; i < countOfSlot; i++) {
            pp_slot_status[i] = (RIL_SimSlotStatus*)calloc(1, sizeof(RIL_SimSlotStatus));
            RFX_ASSERT(pp_slot_status[i] != NULL);
            memset(pp_slot_status[i], 0, sizeof(RIL_SimSlotStatus));
            memcpy(pp_slot_status[i], tmp[i], sizeof(RIL_SimSlotStatus));

            pp_slot_status[i]->card_state = tmp[i]->card_state;
            pp_slot_status[i]->slotState = tmp[i]->slotState;
            pp_slot_status[i]->logicalSlotId = tmp[i]->logicalSlotId;
            if (tmp[i]->atr != NULL) {
                int len = strlen(tmp[i]->atr);
                pp_slot_status[i]->atr = (char*)calloc(len + 1, sizeof(char));
                RFX_ASSERT(pp_slot_status[i]->atr != NULL);
                strncpy(pp_slot_status[i]->atr, tmp[i]->atr, len);
            }
            if (tmp[i]->iccId != NULL) {
                int len = strlen(tmp[i]->iccId);
                pp_slot_status[i]->iccId = (char*)calloc(len + 1, sizeof(char));
                RFX_ASSERT(pp_slot_status[i]->iccId != NULL);
                strncpy(pp_slot_status[i]->iccId, tmp[i]->iccId, len);
            }
            if (tmp[i]->eid != NULL) {
                int len = strlen(tmp[i]->eid);
                pp_slot_status[i]->eid = (char*)calloc(len + 1, sizeof(char));
                RFX_ASSERT(pp_slot_status[i]->eid != NULL);
                strncpy(pp_slot_status[i]->eid, tmp[i]->eid, len);
            }
        }
    }
    m_data = (void*)pp_slot_status;
    m_length = _length;
}

RfxSimSlotStatusData::~RfxSimSlotStatusData() {
    int countOfSlot = m_length / sizeof(RIL_SimSlotStatus*);
    RIL_SimSlotStatus** pp_slot_status = (RIL_SimSlotStatus**)m_data;
    if (pp_slot_status != NULL) {
        for (int i = 0; i < countOfSlot; i++) {
            if (pp_slot_status[i] != NULL) {
                if (pp_slot_status[i]->atr != NULL) {
                    free(pp_slot_status[i]->atr);
                }
                if (pp_slot_status[i]->iccId != NULL) {
                    free(pp_slot_status[i]->iccId);
                }
                if (pp_slot_status[i]->eid != NULL) {
                    free(pp_slot_status[i]->eid);
                }
                free(pp_slot_status[i]);
            }
        }
        free(pp_slot_status);
    }
    m_data = NULL;
}
