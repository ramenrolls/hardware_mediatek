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

#include "RfxSipRegInfoData.h"
#include "telephony/mtk_ril.h"

RFX_IMPLEMENT_DATA_CLASS(RfxSipRegInfoData);

RfxSipRegInfoData::RfxSipRegInfoData(void* data, int length) : RfxBaseData(data, length) {
    if (NULL != data) {
        RIL_SipRegInfo* pIn = (RIL_SipRegInfo*)data;
        RIL_SipRegInfo* pOut = (RIL_SipRegInfo*)calloc(1, sizeof(RIL_SipRegInfo));

        pOut->account_id = pIn->account_id;
        pOut->response_code = pIn->response_code;
        if (pIn->direction != NULL) {
            pOut->direction = strdup(pIn->direction);
        }
        if (pIn->sip_msg_type != NULL) {
            pOut->sip_msg_type = strdup(pIn->sip_msg_type);
        }
        if (pIn->method != NULL) {
            pOut->method = strdup(pIn->method);
        }
        if (pIn->reason_phrase != NULL) {
            pOut->reason_phrase = strdup(pIn->reason_phrase);
        }
        if (pIn->warn_text != NULL) {
            pOut->warn_text = strdup(pIn->warn_text);
        }
        m_data = (void*)pOut;
        m_length = length;
    }
}

RfxSipRegInfoData::~RfxSipRegInfoData() {
    if (m_data) {
        RIL_SipRegInfo* p = (RIL_SipRegInfo*)m_data;
        if (p->direction != NULL) {
            free(p->direction);
        }
        if (p->sip_msg_type != NULL) {
            free(p->sip_msg_type);
        }
        if (p->method != NULL) {
            free(p->method);
        }
        if (p->reason_phrase != NULL) {
            free(p->reason_phrase);
        }
        if (p->warn_text != NULL) {
            free(p->warn_text);
        }
        free(m_data);
        m_data = NULL;
    }
}
