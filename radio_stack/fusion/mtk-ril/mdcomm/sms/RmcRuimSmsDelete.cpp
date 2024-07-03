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

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "RmcRuimSmsDelete.h"

/*****************************************************************************
 * Register Data Class
 *****************************************************************************/
RFX_REGISTER_DATA_TO_REQUEST_ID(RmcCdmaDeleteRuimSmsReq, RmcVoidRsp,
                                RFX_MSG_REQUEST_CDMA_DELETE_SMS_ON_RUIM);

/*****************************************************************************
 * Class RmcCdmaDeleteRuimSmsReq
 *****************************************************************************/
RFX_IMPLEMENT_DATA_CLASS(RmcCdmaDeleteRuimSmsReq);
RmcCdmaDeleteRuimSmsReq::RmcCdmaDeleteRuimSmsReq(void* data, int length)
    : RmcSingleAtReq(data, length) {
    RFX_ASSERT(length == sizeof(int));
    m_index = *((int*)data);
    m_data = &m_index;
    m_length = length;
}

RmcCdmaDeleteRuimSmsReq::~RmcCdmaDeleteRuimSmsReq() {}

RmcAtSendInfo* RmcCdmaDeleteRuimSmsReq::onGetAtInfo(RfxBaseHandler* h) {
    RFX_UNUSED(h);
    String8 cmd;
    if (m_index == -1) {
        cmd.setTo("AT+EC2KCMGD=,4");
    } else {
        cmd = String8::format("AT+EC2KCMGD=%d", m_index - 1);
    }
    return new RmcNoLineAtSendInfo(cmd);
}
