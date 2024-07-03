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

#include "RfxImsBearerNotifyData.h"
#include "telephony/mtk_ril.h"
RFX_IMPLEMENT_DATA_CLASS(RfxImsBearerNotifyData);

RfxImsBearerNotifyData::RfxImsBearerNotifyData(void* data, int length) : RfxBaseData(data, length) {
    if (NULL != data) {
        RIL_IMS_BearerNotification* pIn = (RIL_IMS_BearerNotification*)data;
        RIL_IMS_BearerNotification* pOut =
                (RIL_IMS_BearerNotification*)calloc(1, sizeof(RIL_IMS_BearerNotification));
        RFX_ASSERT(pOut != NULL);

        pOut->phone = pIn->phone;
        pOut->aid = pIn->aid;
        pOut->type = strdup(pIn->type);
        pOut->action = pIn->action;
        m_data = (void*)pOut;
        m_length = length;
    }
}

RfxImsBearerNotifyData::~RfxImsBearerNotifyData() {
    if (m_data) {
        RIL_IMS_BearerNotification* p = (RIL_IMS_BearerNotification*)m_data;
        free(p->type);
        free(m_data);
        m_data = NULL;
    }
}
