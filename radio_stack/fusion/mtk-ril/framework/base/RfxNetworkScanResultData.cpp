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

#include "RfxNetworkScanResultData.h"

RFX_IMPLEMENT_DATA_CLASS(RfxNetworkScanResultData);

RfxNetworkScanResultData::RfxNetworkScanResultData(void* data, int length)
    : RfxBaseData(data, length) {
    if (data != NULL) {
        RIL_NetworkScanResult* pResult = (RIL_NetworkScanResult*)data;
        RIL_NetworkScanResult* pData =
                (RIL_NetworkScanResult*)calloc(1, sizeof(RIL_NetworkScanResult));
        if (pData == NULL) goto error;
        pData->status = pResult->status;
        pData->network_infos_length = pResult->network_infos_length;
        if (pData->network_infos_length > 0) {
            int countCell = pResult->network_infos_length;
            pData->network_infos = (RIL_CellInfo_v12*)calloc(countCell, sizeof(RIL_CellInfo_v12));
            if (pData->network_infos == NULL) goto error;
            memcpy(pData->network_infos, pResult->network_infos,
                   pData->network_infos_length * sizeof(RIL_CellInfo_v12));
            for (int i = 0; i < countCell; i++) {
                switch (pData->network_infos[i].cellInfoType) {
                    case RIL_CELL_INFO_TYPE_GSM:
                        asprintf(&(pData->network_infos[i]
                                           .CellInfo.gsm.cellIdentityGsm.operName.long_name),
                                 "%s",
                                 pResult->network_infos[i]
                                         .CellInfo.gsm.cellIdentityGsm.operName.long_name);
                        asprintf(&(pData->network_infos[i]
                                           .CellInfo.gsm.cellIdentityGsm.operName.short_name),
                                 "%s",
                                 pResult->network_infos[i]
                                         .CellInfo.gsm.cellIdentityGsm.operName.short_name);
                        break;
                    case RIL_CELL_INFO_TYPE_WCDMA:
                        asprintf(&(pData->network_infos[i]
                                           .CellInfo.wcdma.cellIdentityWcdma.operName.long_name),
                                 "%s",
                                 pResult->network_infos[i]
                                         .CellInfo.wcdma.cellIdentityWcdma.operName.long_name);
                        asprintf(&(pData->network_infos[i]
                                           .CellInfo.wcdma.cellIdentityWcdma.operName.short_name),
                                 "%s",
                                 pResult->network_infos[i]
                                         .CellInfo.wcdma.cellIdentityWcdma.operName.short_name);
                        break;
                    case RIL_CELL_INFO_TYPE_LTE:
                        asprintf(&(pData->network_infos[i]
                                           .CellInfo.lte.cellIdentityLte.operName.long_name),
                                 "%s",
                                 pResult->network_infos[i]
                                         .CellInfo.lte.cellIdentityLte.operName.long_name);
                        asprintf(&(pData->network_infos[i]
                                           .CellInfo.lte.cellIdentityLte.operName.short_name),
                                 "%s",
                                 pResult->network_infos[i]
                                         .CellInfo.lte.cellIdentityLte.operName.short_name);
                        break;
                    case RIL_CELL_INFO_TYPE_NR:
                        // TODO
                    case RIL_CELL_INFO_TYPE_TD_SCDMA:
                    case RIL_CELL_INFO_TYPE_CDMA:
                    case RIL_CELL_INFO_TYPE_NONE:
                        break;
                }
            }
        }
        m_data = pData;
        m_length = length;
        return;
    error:
        if (pData) free(pData);
        m_data = NULL;
        m_length = 0;
    }
}

RfxNetworkScanResultData::~RfxNetworkScanResultData() {
    RIL_NetworkScanResult* pData = (RIL_NetworkScanResult*)m_data;
    if (pData) {
        if (pData->network_infos_length > 0 && pData->network_infos != NULL) {
            int countCell = pData->network_infos_length;
            for (int i = 0; i < countCell; i++) {
                switch (pData->network_infos[i].cellInfoType) {
                    case RIL_CELL_INFO_TYPE_GSM:
                        if (pData->network_infos[i].CellInfo.gsm.cellIdentityGsm.operName.long_name)
                            free(pData->network_infos[i]
                                         .CellInfo.gsm.cellIdentityGsm.operName.long_name);
                        if (pData->network_infos[i]
                                    .CellInfo.gsm.cellIdentityGsm.operName.short_name)
                            free(pData->network_infos[i]
                                         .CellInfo.gsm.cellIdentityGsm.operName.short_name);
                        break;
                    case RIL_CELL_INFO_TYPE_WCDMA:
                        if (pData->network_infos[i]
                                    .CellInfo.wcdma.cellIdentityWcdma.operName.long_name)
                            free(pData->network_infos[i]
                                         .CellInfo.wcdma.cellIdentityWcdma.operName.long_name);
                        if (pData->network_infos[i]
                                    .CellInfo.wcdma.cellIdentityWcdma.operName.short_name)
                            free(pData->network_infos[i]
                                         .CellInfo.wcdma.cellIdentityWcdma.operName.short_name);
                        break;
                    case RIL_CELL_INFO_TYPE_LTE:
                        if (pData->network_infos[i].CellInfo.lte.cellIdentityLte.operName.long_name)
                            free(pData->network_infos[i]
                                         .CellInfo.lte.cellIdentityLte.operName.long_name);
                        if (pData->network_infos[i]
                                    .CellInfo.lte.cellIdentityLte.operName.short_name)
                            free(pData->network_infos[i]
                                         .CellInfo.lte.cellIdentityLte.operName.short_name);
                        break;
                    case RIL_CELL_INFO_TYPE_NR:
                        // TODO
                    case RIL_CELL_INFO_TYPE_TD_SCDMA:
                    case RIL_CELL_INFO_TYPE_CDMA:
                    case RIL_CELL_INFO_TYPE_NONE:
                        break;
                }
            }
            free(pData->network_infos);
        }
        free(pData);
    }
    m_data = NULL;
    m_length = 0;
}
