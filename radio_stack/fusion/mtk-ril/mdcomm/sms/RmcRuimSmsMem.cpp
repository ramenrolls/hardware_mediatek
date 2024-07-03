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
#include "RmcRuimSmsMem.h"

/*****************************************************************************
 * Register Data Class
 *****************************************************************************/

RFX_REGISTER_DATA_TO_REQUEST_ID(RmcCdmaGetRuimSmsMemReq, RmcCdmaGetRuimSmsMemRsp,
                                RFX_MSG_REQUEST_GET_SMS_RUIM_MEM_STATUS);

/*****************************************************************************
 * Class RmcCdmaGetRuimSmsMemReq
 *****************************************************************************/
RFX_IMPLEMENT_DATA_CLASS(RmcCdmaGetRuimSmsMemReq);
RmcCdmaGetRuimSmsMemReq::RmcCdmaGetRuimSmsMemReq(void* data, int length)
    : RmcSingleAtReq(data, length), m_used(0), m_total(0) {}

RmcCdmaGetRuimSmsMemReq::~RmcCdmaGetRuimSmsMemReq() {}

RmcAtSendInfo* RmcCdmaGetRuimSmsMemReq::onGetAtInfo(RfxBaseHandler* h) {
    RFX_UNUSED(h);
    String8 cmd("AT+EC2KCPMS?");
    String8 responsePrefix("+EC2KCPMS:");
    return new RmcSingleLineAtSendInfo(cmd, responsePrefix);
}

bool RmcCdmaGetRuimSmsMemReq::onHandleIntermediates(RfxAtLine* line, RfxBaseHandler* h) {
    RFX_UNUSED(h);
    int err;
    line->atTokNextstr(&err);
    if (err < 0) {
        setError(RIL_E_SYSTEM_ERR);
        return false;
    }
    m_used = line->atTokNextint(&err);
    if (err < 0) {
        return false;
    }
    m_total = line->atTokNextint(&err);
    if (err < 0) {
        return false;
    }
    return true;
}

/*****************************************************************************
 * Class RmcCdmaGetRuimSmsMemRsp
 *****************************************************************************/
RFX_IMPLEMENT_DATA_CLASS(RmcCdmaGetRuimSmsMemRsp);
RmcCdmaGetRuimSmsMemRsp::RmcCdmaGetRuimSmsMemRsp(void* data, int length)
    : RmcVoidRsp(data, length) {
    if (data != NULL) {
        m_info = *((MemInfo*)data);
        m_data = (void*)&m_info;
        m_length = length;
    }
}

RmcCdmaGetRuimSmsMemRsp::RmcCdmaGetRuimSmsMemRsp(int total, int used, RIL_Errno e)
    : RmcVoidRsp(e), m_info(total, used) {
    if (e == RIL_E_SUCCESS) {
        m_data = (void*)&m_info;
        m_length = sizeof(m_info);
    }
}

RmcCdmaGetRuimSmsMemRsp::~RmcCdmaGetRuimSmsMemRsp() {}

/*****************************************************************************
 * Class RmcCdmaGetRuimSmsMemHdlr
 *****************************************************************************/
RmcBaseRspData* RmcCdmaGetRuimSmsMemHdlr::onGetRspData(RmcBaseReqData* req) {
    RmcCdmaGetRuimSmsMemReq* memReq = (RmcCdmaGetRuimSmsMemReq*)req;
    return new RmcCdmaGetRuimSmsMemRsp(memReq->getTotal(), memReq->getUsed(), req->getError());
}
