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

#include "RfxDataCallResponseData.h"

#define RFX_LOG_TAG "RfxDataCallResponseData"

RFX_IMPLEMENT_DATA_CLASS(RfxDataCallResponseData);

RfxDataCallResponseData::RfxDataCallResponseData(void* data, int length)
    : RfxBaseData(data, length) {
    m_length = length;
    copyDataCallResponseData((MTK_RIL_Data_Call_Response_v11*)data);
}

RfxDataCallResponseData::RfxDataCallResponseData(MTK_RIL_Data_Call_Response_v11* data, int num)
    : RfxBaseData(data, num * sizeof(MTK_RIL_Data_Call_Response_v11)) {
    m_length = num * sizeof(MTK_RIL_Data_Call_Response_v11);
    copyDataCallResponseData(data);
}

void RfxDataCallResponseData::copyDataCallResponseData(MTK_RIL_Data_Call_Response_v11* data) {
    int num = m_length / sizeof(MTK_RIL_Data_Call_Response_v11);
    if (num == 0 || data == NULL) {
        RFX_LOG_E(RFX_LOG_TAG,
                  "[%s] The number of responses is %d (if num != 0, means data is null)",
                  __FUNCTION__, num);
        return;
    }

    MTK_RIL_Data_Call_Response_v11* tmpPtr = (MTK_RIL_Data_Call_Response_v11*)calloc(1, m_length);
    RFX_ASSERT(tmpPtr != NULL);
    MTK_RIL_Data_Call_Response_v11* curPtr = data;

    for (int i = 0; i < num; i++) {
        tmpPtr[i].status = curPtr[i].status;
        tmpPtr[i].suggestedRetryTime = curPtr[i].suggestedRetryTime;
        tmpPtr[i].cid = curPtr[i].cid;
        tmpPtr[i].active = curPtr[i].active;
        tmpPtr[i].mtu = curPtr[i].mtu;
        tmpPtr[i].rat = curPtr[i].rat;

        if ((&curPtr[i])->type != NULL) {
            (&tmpPtr[i])->type = (char*)calloc(1, (strlen((&curPtr[i])->type) * sizeof(char)) + 1);
            RFX_ASSERT((&tmpPtr[i])->type != NULL);
            strncpy((&tmpPtr[i])->type, (&curPtr[i])->type, strlen((&curPtr[i])->type));
        }

        if ((&curPtr[i])->ifname != NULL) {
            (&tmpPtr[i])->ifname =
                    (char*)calloc(1, (strlen((&curPtr[i])->ifname) * sizeof(char)) + 1);
            RFX_ASSERT((&tmpPtr[i])->ifname != NULL);
            strncpy((&tmpPtr[i])->ifname, (&curPtr[i])->ifname, strlen((&curPtr[i])->ifname));
        }

        if ((&curPtr[i])->addresses != NULL) {
            (&tmpPtr[i])->addresses =
                    (char*)calloc(1, (strlen((&curPtr[i])->addresses) * sizeof(char)) + 1);
            RFX_ASSERT((&tmpPtr[i])->addresses != NULL);
            strncpy((&tmpPtr[i])->addresses, (&curPtr[i])->addresses,
                    strlen((&curPtr[i])->addresses));
        }

        if ((&curPtr[i])->dnses != NULL) {
            (&tmpPtr[i])->dnses =
                    (char*)calloc(1, (strlen((&curPtr[i])->dnses) * sizeof(char)) + 1);
            RFX_ASSERT((&tmpPtr[i])->dnses != NULL);
            strncpy((&tmpPtr[i])->dnses, (&curPtr[i])->dnses, strlen((&curPtr[i])->dnses));
        }

        if ((&curPtr[i])->gateways != NULL) {
            (&tmpPtr[i])->gateways =
                    (char*)calloc(1, (strlen((&curPtr[i])->gateways) * sizeof(char)) + 1);
            RFX_ASSERT((&tmpPtr[i])->gateways != NULL);
            strncpy((&tmpPtr[i])->gateways, (&curPtr[i])->gateways, strlen((&curPtr[i])->gateways));
        }

        if ((&curPtr[i])->pcscf != NULL) {
            (&tmpPtr[i])->pcscf =
                    (char*)calloc(1, (strlen((&curPtr[i])->pcscf) * sizeof(char)) + 1);
            RFX_ASSERT((&tmpPtr[i])->pcscf != NULL);
            strncpy((&tmpPtr[i])->pcscf, (&curPtr[i])->pcscf, strlen((&curPtr[i])->pcscf));
        }
    }
    m_data = tmpPtr;
}

RfxDataCallResponseData::~RfxDataCallResponseData() {
    MTK_RIL_Data_Call_Response_v11* tmpPtr = (MTK_RIL_Data_Call_Response_v11*)m_data;
    int num = m_length / sizeof(MTK_RIL_Data_Call_Response_v11);

    if (tmpPtr != NULL) {
        for (int i = 0; i < num; i++) {
            FREEIF((&tmpPtr[i])->type);
            FREEIF((&tmpPtr[i])->ifname);
            FREEIF((&tmpPtr[i])->addresses);
            FREEIF((&tmpPtr[i])->gateways);
            FREEIF((&tmpPtr[i])->dnses);
            FREEIF((&tmpPtr[i])->pcscf);
        }
        free(tmpPtr);
        tmpPtr = NULL;
    }
}
