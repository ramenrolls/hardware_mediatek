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

#include "RfxNeighboringCellData.h"

RFX_IMPLEMENT_DATA_CLASS(RfxNeighboringCellData);

RfxNeighboringCellData::RfxNeighboringCellData(void* data, int length) : RfxBaseData(data, length) {
    if (data != NULL) {
        m_length = length;
        int countStrings = length / sizeof(RIL_NeighboringCell*);
        copyCells((RIL_NeighboringCell**)data, countStrings);
    }
}

RfxNeighboringCellData::RfxNeighboringCellData(RIL_NeighboringCell** data, int countStrings)
    : RfxBaseData(data, countStrings * sizeof(RIL_NeighboringCell*)) {
    if (data != NULL) {
        m_length = countStrings * sizeof(RIL_NeighboringCell*);
        copyCells(data, countStrings);
    }
}

void RfxNeighboringCellData::copyCells(RIL_NeighboringCell** data, int countCell) {
    RIL_NeighboringCell** pCell =
            (RIL_NeighboringCell**)calloc(countCell, sizeof(RIL_NeighboringCell*));
    if (pCell == NULL) goto error;
    for (int i = 0; i < countCell; i++) {
        pCell[i] = (RIL_NeighboringCell*)calloc(1, sizeof(RIL_NeighboringCell));
        if (pCell[i] == NULL) goto error;
        asprintf(&pCell[i]->cid, "%s", data[i]->cid);
        pCell[i]->rssi = data[i]->rssi;
    }
    m_data = pCell;
    return;
error:
    if (pCell) free(pCell);
    m_data = NULL;
}

RfxNeighboringCellData::~RfxNeighboringCellData() {
    if (m_data) {
        RIL_NeighboringCell** pTmp = (RIL_NeighboringCell**)m_data;
        int countCell = m_length / sizeof(RIL_NeighboringCell*);
        for (int i = 0; i < countCell; i++) {
            if (pTmp[i]) {
                if (pTmp[i]->cid) {
                    free(pTmp[i]->cid);
                }
                free(pTmp[i]);
            }
        }
        free(m_data);
    }
}
