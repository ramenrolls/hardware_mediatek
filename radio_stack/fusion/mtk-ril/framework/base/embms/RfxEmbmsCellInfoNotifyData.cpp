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

#include "RfxEmbmsCellInfoNotifyData.h"
#include <string.h>
#include <mtk_log.h>

#define LOG_TAG "EMBMS"

RFX_IMPLEMENT_DATA_CLASS(RfxEmbmsCellInfoNotifyData);

RfxEmbmsCellInfoNotifyData::RfxEmbmsCellInfoNotifyData(void* data, int length)
    : RfxBaseData(data, length) {
    copyData((RIL_EMBMS_CellInfoNotify*)data, length);
}

RfxEmbmsCellInfoNotifyData::RfxEmbmsCellInfoNotifyData(RIL_EMBMS_CellInfoNotify* data, int length)
    : RfxBaseData(data, length) {
    copyData(data, length);
}

void RfxEmbmsCellInfoNotifyData::copyData(RIL_EMBMS_CellInfoNotify* data, int length) {
    m_length = length;
    m_data = (RIL_EMBMS_CellInfoNotify*)calloc(1, sizeof(RIL_EMBMS_CellInfoNotify));
    if (m_data != NULL) {
        memcpy(m_data, data, length);
    } else {
        mtkLogE(LOG_TAG, "calloc fail in %s", __FILE__);
    }
}

RfxEmbmsCellInfoNotifyData::~RfxEmbmsCellInfoNotifyData() {
    if (m_data) {
        free(m_data);
        m_data = NULL;
    }
}
