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

#include "WpfaDriverDeRegFilterData.h"
#include "WpfaDriverMessage.h"
#include <mtk_log.h>

#define WPFA_D_LOG_TAG "WpfaDriverDeReg"

WPFA_IMPLEMENT_DATA_CLASS(WpfaDriverDeRegFilterData);

WpfaDriverDeRegFilterData::WpfaDriverDeRegFilterData(void* _data, int _length)
    : WpfaDriverBaseData(_data, _length) {
    if (_data != NULL) {
        wifiproxy_m2a_dereg_dl_filter_t* pSendMsg = (wifiproxy_m2a_dereg_dl_filter_t*)_data;
        wifiproxy_m2a_dereg_dl_filter_t* pData = (wifiproxy_m2a_dereg_dl_filter_t*)calloc(
                1, sizeof(wifiproxy_m2a_dereg_dl_filter_t));

        if (pData == NULL) {
            mtkLogD(WPFA_D_LOG_TAG, "pData is null, return.");
            return;
        }
        pData->fid = pSendMsg->fid;

        mData = (void*)pData;
        mLength = _length;
    }
}

WpfaDriverDeRegFilterData::~WpfaDriverDeRegFilterData() {
    // free memory
    if (mData != NULL) {
        free(mData);
        mData = NULL;
    }
}
