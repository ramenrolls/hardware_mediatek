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

#include "nw/RmcNetworkHandler.h"
#include "RfxDataRegStateData.h"

RFX_IMPLEMENT_DATA_CLASS(RfxDataRegStateData);

RfxDataRegStateData::RfxDataRegStateData(void* data, int length) : RfxBaseData(data, length) {
    if (data != NULL) {
        RIL_DataRegistrationStateResponse* tmp = (RIL_DataRegistrationStateResponse*)data;
        RIL_DataRegistrationStateResponse* pData =
                (RIL_DataRegistrationStateResponse*)calloc(1, length);
        RFX_ASSERT(pData != NULL);
        memcpy(pData, tmp, length);

        // alloc memory for string type
        switch (pData->cellIdentity.cellInfoType) {
            case RIL_CELL_INFO_TYPE_GSM: {
                asprintf(&(pData->cellIdentity.cellIdentityGsm.operName.long_name), "%s",
                         tmp->cellIdentity.cellIdentityGsm.operName.long_name);
                asprintf(&(pData->cellIdentity.cellIdentityGsm.operName.short_name), "%s",
                         tmp->cellIdentity.cellIdentityGsm.operName.short_name);
                break;
            }

            case RIL_CELL_INFO_TYPE_WCDMA: {
                asprintf(&(pData->cellIdentity.cellIdentityWcdma.operName.long_name), "%s",
                         tmp->cellIdentity.cellIdentityWcdma.operName.long_name);
                asprintf(&(pData->cellIdentity.cellIdentityWcdma.operName.short_name), "%s",
                         tmp->cellIdentity.cellIdentityWcdma.operName.short_name);
                break;
            }

            case RIL_CELL_INFO_TYPE_CDMA: {
                asprintf(&(pData->cellIdentity.cellIdentityCdma.operName.long_name), "%s",
                         tmp->cellIdentity.cellIdentityCdma.operName.long_name);
                asprintf(&(pData->cellIdentity.cellIdentityCdma.operName.short_name), "%s",
                         tmp->cellIdentity.cellIdentityCdma.operName.short_name);
                break;
            }

            case RIL_CELL_INFO_TYPE_LTE: {
                asprintf(&(pData->cellIdentity.cellIdentityLte.operName.long_name), "%s",
                         tmp->cellIdentity.cellIdentityLte.operName.long_name);
                asprintf(&(pData->cellIdentity.cellIdentityLte.operName.short_name), "%s",
                         tmp->cellIdentity.cellIdentityLte.operName.short_name);
                break;
            }

            case RIL_CELL_INFO_TYPE_TD_SCDMA: {
                asprintf(&(pData->cellIdentity.cellIdentityTdscdma.operName.long_name), "%s",
                         tmp->cellIdentity.cellIdentityTdscdma.operName.long_name);
                asprintf(&(pData->cellIdentity.cellIdentityTdscdma.operName.short_name), "%s",
                         tmp->cellIdentity.cellIdentityTdscdma.operName.short_name);
                break;
            }
            case RIL_CELL_INFO_TYPE_NR: {
                asprintf(&(pData->cellIdentity.cellIdentityNr.operName.long_name), "%s",
                         tmp->cellIdentity.cellIdentityNr.operName.long_name);
                asprintf(&(pData->cellIdentity.cellIdentityNr.operName.short_name), "%s",
                         tmp->cellIdentity.cellIdentityNr.operName.short_name);
                break;
            }
            case RIL_CELL_INFO_TYPE_NONE: {
                pData->cellIdentity.cellIdentityTdscdma.operName.long_name = NULL;
                pData->cellIdentity.cellIdentityTdscdma.operName.short_name = NULL;
                break;
            }
        }

        m_data = pData;
        m_length = length;
    }
}

RfxDataRegStateData::~RfxDataRegStateData() {
    if (m_data) {
        RIL_DataRegistrationStateResponse* tmp = (RIL_DataRegistrationStateResponse*)m_data;
        // free string if it is not null.
        switch (tmp->cellIdentity.cellInfoType) {
            case RIL_CELL_INFO_TYPE_GSM: {
                if (tmp->cellIdentity.cellIdentityGsm.operName.long_name)
                    free(tmp->cellIdentity.cellIdentityGsm.operName.long_name);
                if (tmp->cellIdentity.cellIdentityGsm.operName.short_name)
                    free(tmp->cellIdentity.cellIdentityGsm.operName.short_name);
                break;
            }

            case RIL_CELL_INFO_TYPE_WCDMA: {
                if (tmp->cellIdentity.cellIdentityWcdma.operName.long_name)
                    free(tmp->cellIdentity.cellIdentityWcdma.operName.long_name);
                if (tmp->cellIdentity.cellIdentityWcdma.operName.short_name)
                    free(tmp->cellIdentity.cellIdentityWcdma.operName.short_name);
                break;
            }

            case RIL_CELL_INFO_TYPE_CDMA: {
                if (tmp->cellIdentity.cellIdentityCdma.operName.long_name)
                    free(tmp->cellIdentity.cellIdentityCdma.operName.long_name);
                if (tmp->cellIdentity.cellIdentityCdma.operName.short_name)
                    free(tmp->cellIdentity.cellIdentityCdma.operName.short_name);
                break;
            }

            case RIL_CELL_INFO_TYPE_LTE: {
                if (tmp->cellIdentity.cellIdentityLte.operName.long_name)
                    free(tmp->cellIdentity.cellIdentityLte.operName.long_name);
                if (tmp->cellIdentity.cellIdentityLte.operName.short_name)
                    free(tmp->cellIdentity.cellIdentityLte.operName.short_name);
                break;
            }

            case RIL_CELL_INFO_TYPE_TD_SCDMA: {
                if (tmp->cellIdentity.cellIdentityTdscdma.operName.long_name)
                    free(tmp->cellIdentity.cellIdentityTdscdma.operName.long_name);
                if (tmp->cellIdentity.cellIdentityTdscdma.operName.short_name)
                    free(tmp->cellIdentity.cellIdentityTdscdma.operName.short_name);
                break;
            }
            case RIL_CELL_INFO_TYPE_NR: {
                if (tmp->cellIdentity.cellIdentityNr.operName.long_name)
                    free(tmp->cellIdentity.cellIdentityNr.operName.long_name);
                if (tmp->cellIdentity.cellIdentityNr.operName.short_name)
                    free(tmp->cellIdentity.cellIdentityNr.operName.short_name);
                break;
            }
            case RIL_CELL_INFO_TYPE_NONE:
                break;
        }
        free(m_data);
    }
    m_data = NULL;
    m_length = 0;
}
