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

#include "WpfaDriverRegFilterData.h"
#include "WpfaDriverMessage.h"

#define WPFA_D_LOG_TAG "WpfaDriverRegFilter"

WPFA_IMPLEMENT_DATA_CLASS(WpfaDriverRegFilterData);

WpfaDriverRegFilterData::WpfaDriverRegFilterData(void* _data, int _length)
    : WpfaDriverBaseData(_data, _length) {
    int i = 0;
    if (_data != NULL) {
        wifiproxy_m2a_reg_dl_filter_t* pSendMsg = (wifiproxy_m2a_reg_dl_filter_t*)_data;
        wifiproxy_m2a_reg_dl_filter_t* pData =
                (wifiproxy_m2a_reg_dl_filter_t*)calloc(1, sizeof(wifiproxy_m2a_reg_dl_filter_t));

        if (pData == NULL) {
            mtkLogD(WPFA_D_LOG_TAG, "pData is null, return.");
            return;
        }

        pData->reg_hdr.filter_config = pSendMsg->reg_hdr.filter_config;
        pData->reg_hdr.operation_config = pSendMsg->reg_hdr.operation_config;
        pData->reg_hdr.priority = pSendMsg->reg_hdr.priority;
        pData->reg_hdr.ip_ver = pSendMsg->reg_hdr.ip_ver;
        pData->reg_hdr.protocol = pSendMsg->reg_hdr.protocol;
        pData->reg_hdr.icmp_type = pSendMsg->reg_hdr.icmp_type;
        pData->reg_hdr.icmp_code = pSendMsg->reg_hdr.icmp_code;
        for (i = 0; i < 16; i++) {
            pData->reg_hdr.ip_src[i] = pSendMsg->reg_hdr.ip_src[i];
        }
        pData->reg_hdr.ip_src_mask = pSendMsg->reg_hdr.ip_src_mask;
        for (i = 0; i < 16; i++) {
            pData->reg_hdr.ip_dest[i] = pSendMsg->reg_hdr.ip_dest[i];
        }
        pData->reg_hdr.ip_dest_mask = pSendMsg->reg_hdr.ip_dest_mask;
        pData->reg_hdr.src_port = pSendMsg->reg_hdr.src_port;
        pData->reg_hdr.src_port_mask = pSendMsg->reg_hdr.src_port_mask;
        pData->reg_hdr.dst_port = pSendMsg->reg_hdr.dst_port;
        pData->reg_hdr.dst_port_mask = pSendMsg->reg_hdr.dst_port_mask;
        pData->reg_hdr.tcp_flags = pSendMsg->reg_hdr.tcp_flags;
        pData->reg_hdr.tcp_flags_mask = pSendMsg->reg_hdr.tcp_flags_mask;
        pData->reg_hdr.tcp_flags_operation = pSendMsg->reg_hdr.tcp_flags_operation;
        for (i = 0; i < 16; i++) {
            pData->reg_hdr.icmp_src_ip[i] = pSendMsg->reg_hdr.icmp_src_ip[i];
        }
        pData->reg_hdr.icmp_src_port = pSendMsg->reg_hdr.icmp_src_port;
        pData->reg_hdr.icmp_src_mask = pSendMsg->reg_hdr.icmp_src_mask;
        pData->reg_hdr.esp_spi = pSendMsg->reg_hdr.esp_spi;
        pData->reg_hdr.esp_spi_mask = pSendMsg->reg_hdr.esp_spi_mask;

        pData->fid = pSendMsg->fid;

        mData = (void*)pData;
        mLength = _length;
    }
}

WpfaDriverRegFilterData::~WpfaDriverRegFilterData() {
    // free memory
    if (mData != NULL) {
        free(mData);
        mData = NULL;
    }
}
