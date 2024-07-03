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

#include "RfxImsSmsData.h"
#include "RfxLog.h"
#include <telephony/mtk_ril.h>

RFX_IMPLEMENT_DATA_CLASS(RfxImsSmsData);

RfxImsSmsData::RfxImsSmsData(void* _data, int _length) : RfxBaseData(_data, _length) {
    if (_data != NULL) {
        RIL_IMS_SMS_Message* pIms = (RIL_IMS_SMS_Message*)_data;

        if (pIms->tech == RADIO_TECH_3GPP) {
            copyImsGsmSmsData(_data, _length);
        } else {
            copyImsCdmaSmsData(_data, _length);
        }
    }
}

RfxImsSmsData::~RfxImsSmsData() {
    // free memory
    if (m_data != NULL) {
        RIL_IMS_SMS_Message* pIms = (RIL_IMS_SMS_Message*)m_data;
        if (pIms->tech == RADIO_TECH_3GPP) {
            releaseImsGsmSmsData();
        } else {
            releaseImsCdmaSmsData();
        }
    }
}

void RfxImsSmsData::copyImsGsmSmsData(void* data, int length) {
    RIL_IMS_SMS_Message* pIms = (RIL_IMS_SMS_Message*)data;
    RIL_IMS_SMS_Message* pData = (RIL_IMS_SMS_Message*)calloc(1, sizeof(RIL_IMS_SMS_Message));
    char** pStrs = pIms->message.gsmMessage;
    char** pDstStrs = NULL;
    int count = GSM_SMS_MESSAGE_STRS_COUNT;

    RFX_ASSERT(pData != NULL);
    pData->tech = pIms->tech;
    pData->messageRef = pIms->messageRef;

    if (pStrs != NULL) {
        pDstStrs = (char**)calloc(count, sizeof(char*));
        RFX_ASSERT(pDstStrs != NULL);
        for (int i = 0; i < count; i++) {
            if (pStrs[i] != NULL) {
                asprintf(&pDstStrs[i], "%s", pStrs[i]);
            }
        }
        pData->message.gsmMessage = pDstStrs;
    }

    m_data = (void*)pData;
    m_length = length;
}

void RfxImsSmsData::releaseImsGsmSmsData() {
    if (m_data != NULL) {
        RIL_IMS_SMS_Message* pIms = (RIL_IMS_SMS_Message*)m_data;
        char** pStrs = pIms->message.gsmMessage;
        int count = GSM_SMS_MESSAGE_STRS_COUNT;

        if (pStrs != NULL) {
            for (int i = 0; i < count; i++) {
                if (pStrs[i] != NULL) {
                    free(pStrs[i]);
                }
            }

            free(pStrs);
        }

        free(m_data);
        m_data = NULL;
    }
}

void RfxImsSmsData::copyImsCdmaSmsData(void* data, int length) {
    RIL_IMS_SMS_Message* pIms = (RIL_IMS_SMS_Message*)data;
    RIL_IMS_SMS_Message* pData = (RIL_IMS_SMS_Message*)calloc(1, sizeof(RIL_IMS_SMS_Message));
    RFX_ASSERT(pData != NULL);
    pData->tech = pIms->tech;
    pData->messageRef = pIms->messageRef;
    if (pIms->message.cdmaMessage != NULL) {
        pData->message.cdmaMessage = (RIL_CDMA_SMS_Message*)malloc(sizeof(RIL_CDMA_SMS_Message));
        RFX_ASSERT(pData->message.cdmaMessage != NULL);
        *(pData->message.cdmaMessage) = *(pIms->message.cdmaMessage);
    }
    m_data = (void*)pData;
    m_length = length;
}

void RfxImsSmsData::releaseImsCdmaSmsData() {
    if (m_data != NULL) {
        RIL_IMS_SMS_Message* pIms = (RIL_IMS_SMS_Message*)m_data;
        free(pIms->message.cdmaMessage);
        pIms->message.cdmaMessage = NULL;
        free(m_data);
        m_data = NULL;
    }
}
