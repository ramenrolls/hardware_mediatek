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

#include "RfxCellInfoData.h"

RFX_IMPLEMENT_DATA_CLASS(RfxCellInfoData);

RfxCellInfoData::RfxCellInfoData(void* data, int length) : RfxBaseData(data, length) {
    if (data != NULL) {
        m_length = length;
        int countCell = m_length / sizeof(RIL_CellInfo_v12);

        RIL_CellInfo_v12* pCell = (RIL_CellInfo_v12*)calloc(countCell, sizeof(RIL_CellInfo_v12));
        if (pCell == NULL) goto error;
        memcpy(pCell, (RIL_CellInfo_v12*)data, m_length);
        // alloc memory for string
        RIL_CellInfo_v12* tmp = (RIL_CellInfo_v12*)data;
        for (int i = 0; i < countCell; i++) {
            switch (pCell[i].cellInfoType) {
                case RIL_CELL_INFO_TYPE_GSM: {
                    asprintf(&(pCell[i].CellInfo.gsm.cellIdentityGsm.operName.long_name), "%s",
                             tmp[i].CellInfo.gsm.cellIdentityGsm.operName.long_name);
                    asprintf(&(pCell[i].CellInfo.gsm.cellIdentityGsm.operName.short_name), "%s",
                             tmp[i].CellInfo.gsm.cellIdentityGsm.operName.short_name);
                    break;
                }

                case RIL_CELL_INFO_TYPE_WCDMA: {
                    asprintf(&(pCell[i].CellInfo.wcdma.cellIdentityWcdma.operName.long_name), "%s",
                             tmp[i].CellInfo.wcdma.cellIdentityWcdma.operName.long_name);
                    asprintf(&(pCell[i].CellInfo.wcdma.cellIdentityWcdma.operName.short_name), "%s",
                             tmp[i].CellInfo.wcdma.cellIdentityWcdma.operName.short_name);
                    break;
                }

                case RIL_CELL_INFO_TYPE_CDMA: {
                    // It's NULL now.
                    break;
                }

                case RIL_CELL_INFO_TYPE_LTE: {
                    asprintf(&(pCell[i].CellInfo.lte.cellIdentityLte.operName.long_name), "%s",
                             tmp[i].CellInfo.lte.cellIdentityLte.operName.long_name);
                    asprintf(&(pCell[i].CellInfo.lte.cellIdentityLte.operName.short_name), "%s",
                             tmp[i].CellInfo.lte.cellIdentityLte.operName.short_name);
                    break;
                }

                case RIL_CELL_INFO_TYPE_TD_SCDMA: {
                    asprintf(&(pCell[i].CellInfo.tdscdma.cellIdentityTdscdma.operName.long_name),
                             "%s", tmp[i].CellInfo.tdscdma.cellIdentityTdscdma.operName.long_name);
                    asprintf(&(pCell[i].CellInfo.tdscdma.cellIdentityTdscdma.operName.short_name),
                             "%s", tmp[i].CellInfo.tdscdma.cellIdentityTdscdma.operName.short_name);
                    break;
                }
                case RIL_CELL_INFO_TYPE_NR: {
                    asprintf(&(pCell[i].CellInfo.nr.cellidentity.operName.long_name), "%s",
                             tmp[i].CellInfo.nr.cellidentity.operName.long_name);
                    asprintf(&(pCell[i].CellInfo.nr.cellidentity.operName.short_name), "%s",
                             tmp[i].CellInfo.nr.cellidentity.operName.short_name);
                    break;
                }
                case RIL_CELL_INFO_TYPE_NONE:
                    break;
            }
        }

        m_data = pCell;
    }
    return;
error:
    m_data = NULL;
}

RfxCellInfoData::~RfxCellInfoData() {
    if (m_data) {
        int countCell = m_length / sizeof(RIL_CellInfo_v12);
        RIL_CellInfo_v12* pCell = (RIL_CellInfo_v12*)m_data;
        for (int i = 0; i < countCell; i++) {
            switch (pCell[i].cellInfoType) {
                case RIL_CELL_INFO_TYPE_GSM: {
                    if (pCell[i].CellInfo.gsm.cellIdentityGsm.operName.long_name)
                        free(pCell[i].CellInfo.gsm.cellIdentityGsm.operName.long_name);
                    if (pCell[i].CellInfo.gsm.cellIdentityGsm.operName.short_name)
                        free(pCell[i].CellInfo.gsm.cellIdentityGsm.operName.short_name);
                    break;
                }

                case RIL_CELL_INFO_TYPE_WCDMA: {
                    if (pCell[i].CellInfo.wcdma.cellIdentityWcdma.operName.long_name)
                        free(pCell[i].CellInfo.wcdma.cellIdentityWcdma.operName.long_name);
                    if (pCell[i].CellInfo.wcdma.cellIdentityWcdma.operName.short_name)
                        free(pCell[i].CellInfo.wcdma.cellIdentityWcdma.operName.short_name);
                    break;
                }

                case RIL_CELL_INFO_TYPE_CDMA: {
                    // It's NULL now.
                    break;
                }

                case RIL_CELL_INFO_TYPE_LTE: {
                    if (pCell[i].CellInfo.lte.cellIdentityLte.operName.long_name)
                        free(pCell[i].CellInfo.lte.cellIdentityLte.operName.long_name);
                    if (pCell[i].CellInfo.lte.cellIdentityLte.operName.short_name)
                        free(pCell[i].CellInfo.lte.cellIdentityLte.operName.short_name);
                    break;
                }

                case RIL_CELL_INFO_TYPE_TD_SCDMA: {
                    if (pCell[i].CellInfo.tdscdma.cellIdentityTdscdma.operName.long_name)
                        free(pCell[i].CellInfo.tdscdma.cellIdentityTdscdma.operName.long_name);
                    if (pCell[i].CellInfo.tdscdma.cellIdentityTdscdma.operName.short_name)
                        free(pCell[i].CellInfo.tdscdma.cellIdentityTdscdma.operName.short_name);
                    break;
                }
                case RIL_CELL_INFO_TYPE_NR: {
                    if (pCell[i].CellInfo.nr.cellidentity.operName.long_name)
                        free(pCell[i].CellInfo.nr.cellidentity.operName.long_name);
                    if (pCell[i].CellInfo.nr.cellidentity.operName.short_name)
                        free(pCell[i].CellInfo.nr.cellidentity.operName.short_name);
                    break;
                }
                case RIL_CELL_INFO_TYPE_NONE:
                    break;
            }
        }

        free(m_data);
    }
}
