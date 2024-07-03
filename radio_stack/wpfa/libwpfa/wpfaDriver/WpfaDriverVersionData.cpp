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

#include "WpfaDriverVersionData.h"
#include "WpfaDriverMessage.h"
#include <mtk_log.h>

#define WPFA_D_LOG_TAG "WpfaDriverVersion"

WPFA_IMPLEMENT_DATA_CLASS(WpfaDriverVersionData);

WpfaDriverVersionData::WpfaDriverVersionData(void* _data, int _length)
    : WpfaDriverBaseData(_data, _length) {
    if (_data != NULL) {
        wifiproxy_ap_md_filter_ver_t* pSendMsg = (wifiproxy_ap_md_filter_ver_t*)_data;
        wifiproxy_ap_md_filter_ver_t* pData =
                (wifiproxy_ap_md_filter_ver_t*)calloc(1, sizeof(wifiproxy_ap_md_filter_ver_t));

        if (pData == NULL) {
            mtkLogD(WPFA_D_LOG_TAG, "pData is null, return.");
            return;
        }

        pData->ap_filter_ver = pSendMsg->ap_filter_ver;
        pData->md_filter_ver = pSendMsg->md_filter_ver;
        pData->dl_buffer_size = pSendMsg->dl_buffer_size;
        pData->ul_buffer_size = pSendMsg->ul_buffer_size;
        mData = (void*)pData;
        mLength = _length;
    }
}

WpfaDriverVersionData::~WpfaDriverVersionData() {
    // free memory
    if (mData != NULL) {
        free(mData);
        mData = NULL;
    }
}
