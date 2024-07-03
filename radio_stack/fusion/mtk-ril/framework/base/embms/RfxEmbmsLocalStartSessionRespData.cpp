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

#include "RfxEmbmsLocalStartSessionRespData.h"
#include <string.h>
#include <mtk_log.h>

#define LOG_TAG "EMBMS"

RFX_IMPLEMENT_DATA_CLASS(RfxEmbmsLocalStartSessionRespData);

RfxEmbmsLocalStartSessionRespData::RfxEmbmsLocalStartSessionRespData(void* data, int length)
    : RfxBaseData(data, length) {
    copyData((RIL_EMBMS_LocalStartSessionResp*)data, length);
}

RfxEmbmsLocalStartSessionRespData::RfxEmbmsLocalStartSessionRespData(
        RIL_EMBMS_LocalStartSessionResp* data, int length)
    : RfxBaseData(data, length) {
    copyData(data, length);
}

void RfxEmbmsLocalStartSessionRespData::copyData(RIL_EMBMS_LocalStartSessionResp* data,
                                                 int length) {
    m_length = length;
    m_data = (RIL_EMBMS_LocalStartSessionResp*)calloc(1, sizeof(RIL_EMBMS_LocalStartSessionResp));
    if (m_data != NULL) {
        memcpy(m_data, data, length);
    } else {
        mtkLogE(LOG_TAG, "calloc fail in %s", __FILE__);
    }
}

RfxEmbmsLocalStartSessionRespData::~RfxEmbmsLocalStartSessionRespData() {
    if (m_data) {
        free(m_data);
        m_data = NULL;
    }
}
