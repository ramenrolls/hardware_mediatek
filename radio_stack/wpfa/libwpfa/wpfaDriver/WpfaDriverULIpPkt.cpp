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

#include "WpfaDriverULIpPkt.h"
#include "WpfaDriverMessage.h"
#include "FilterStructure.h"

#include <mtk_log.h>

#define WPFA_D_LOG_TAG "WpfaDriverULIpPkt"

WPFA_IMPLEMENT_DATA_CLASS(WpfaDriverULIpPkt);

WpfaDriverULIpPkt::WpfaDriverULIpPkt(void* _data, int _length)
    : WpfaDriverBaseData(_data, _length) {
    int ret = 0;
    if (_data != NULL) {
        wifiproxy_m2a_ul_ip_pkt_t* pSendMsg = (wifiproxy_m2a_ul_ip_pkt_t*)_data;
        wifiproxy_m2a_ul_ip_pkt_t* pData =
                (wifiproxy_m2a_ul_ip_pkt_t*)calloc(1, sizeof(wifiproxy_m2a_ul_ip_pkt_t));
        if (pData == NULL) {
            mtkLogD(WPFA_D_LOG_TAG, "pData is null, return.");
            return;
        }

        for (int i = 0; i < MAX_UL_IP_PKT_SIZE; i++) {
            pData->pkt[i] = pSendMsg->pkt[i];
            // mtkLogD("WpfaDriverULIpPkt", "pData->pkt[%d]=%c",i, pData->pkt[i]);
        }
        mData = (void*)pData;
        mLength = _length;
    }
}

WpfaDriverULIpPkt::~WpfaDriverULIpPkt() {
    // free memory
    if (mData != NULL) {
        free(mData);
        mData = NULL;
    }
}
