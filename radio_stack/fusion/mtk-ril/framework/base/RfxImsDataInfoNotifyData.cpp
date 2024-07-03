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

#include "RfxImsDataInfoNotifyData.h"
#include "telephony/mtk_ril.h"
RFX_IMPLEMENT_DATA_CLASS(RfxImsDataInfoNotifyData);

RfxImsDataInfoNotifyData::RfxImsDataInfoNotifyData(void* data, int length)
    : RfxBaseData(data, length) {
    if (NULL != data) {
        RIL_IMS_DataInfoNotify* pIn = (RIL_IMS_DataInfoNotify*)data;
        RIL_IMS_DataInfoNotify* pOut =
                (RIL_IMS_DataInfoNotify*)calloc(1, sizeof(RIL_IMS_DataInfoNotify));
        RFX_ASSERT(pOut != NULL);

        pOut->phone = pIn->phone;
        pOut->type = strdup(pIn->type);
        pOut->event = strdup(pIn->event);
        pOut->extra = strdup(pIn->extra);
        m_data = (void*)pOut;
        m_length = length;
    }
}

RfxImsDataInfoNotifyData::~RfxImsDataInfoNotifyData() {
    if (m_data) {
        RIL_IMS_DataInfoNotify* p = (RIL_IMS_DataInfoNotify*)m_data;
        free(p->type);
        free(p->event);
        free(p->extra);
        free(m_data);
        m_data = NULL;
    }
}
