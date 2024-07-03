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
#include "RmcCdmaBcActivate.h"

/*****************************************************************************
 * Register Data Class
 *****************************************************************************/
RFX_REGISTER_DATA_TO_REQUEST_ID(RmcCdmaBcActivationReq, RmcVoidRsp,
                                RFX_MSG_REQUEST_CDMA_SMS_BROADCAST_ACTIVATION);

/*****************************************************************************
 * Class RmcCdmaBcActivationReq
 *****************************************************************************/
RFX_IMPLEMENT_DATA_CLASS(RmcCdmaBcActivationReq);
RmcCdmaBcActivationReq::RmcCdmaBcActivationReq(void* data, int length)
    : RmcSingleAtReq(data, length) {
    RFX_ASSERT(length == sizeof(int));
    m_activate = *((int*)data);
    m_data = &m_activate;
    m_length = length;
}

RmcCdmaBcActivationReq::~RmcCdmaBcActivationReq() {}

RmcAtSendInfo* RmcCdmaBcActivationReq::onGetAtInfo(RfxBaseHandler* h) {
    RFX_UNUSED(h);
    String8 cmd = String8::format("AT+ECSCB=%d", (m_activate == 0) ? 1 : 0);
    return new RmcNoLineAtSendInfo(cmd);
}
