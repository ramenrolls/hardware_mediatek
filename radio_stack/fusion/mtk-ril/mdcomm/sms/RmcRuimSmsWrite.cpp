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
#include "RmcRuimSmsWrite.h"
#include "RmcCdmaSmsConverter.h"

/*****************************************************************************
 * Register Data Class
 *****************************************************************************/

RFX_REGISTER_DATA_TO_REQUEST_ID(RmcCdmaWriteRuimSmsReq, RmcCdmaWriteRuimSmsRsp,
                                RFX_MSG_REQUEST_CDMA_WRITE_SMS_TO_RUIM);

/*****************************************************************************
 * Class RmcCdmaWriteRuimSmsReq
 *****************************************************************************/
RFX_IMPLEMENT_DATA_CLASS(RmcCdmaWriteRuimSmsReq);
RmcCdmaWriteRuimSmsReq::RmcCdmaWriteRuimSmsReq(void* data, int length)
    : RmcSingleAtReq(data, length), m_index(-1) {
    RFX_ASSERT(length == sizeof(m_writeArgs));
    m_writeArgs = *((RIL_CDMA_SMS_WriteArgs*)data);
    m_data = &m_writeArgs;
    m_length = length;
}

RmcCdmaWriteRuimSmsReq::~RmcCdmaWriteRuimSmsReq() {}

RmcAtSendInfo* RmcCdmaWriteRuimSmsReq::onGetAtInfo(RfxBaseHandler* h) {
    RFX_UNUSED(h);
    String8 hexPdu;
    String8 number;
    if (!RmcCdmaSmsConverter::toHexPdu(m_writeArgs.message, hexPdu, number)) {
        this->setError(RIL_E_SYSTEM_ERR);
        return NULL;
    }
    String8 cmd =
            String8::format("AT+EC2KCMGW=\"0\",\"%s\",%d", hexPdu.string(), m_writeArgs.status);
    String8 responsePrefix("+EC2KCMGW:");
    return new RmcSingleLineAtSendInfo(cmd, responsePrefix);
}

bool RmcCdmaWriteRuimSmsReq::onHandleIntermediates(RfxAtLine* line, RfxBaseHandler* h) {
    RFX_UNUSED(h);
    int err;
    char* storage = line->atTokNextstr(&err);
    if (err < 0) {
        setError(RIL_E_SYSTEM_ERR);
        return false;
    }
    m_index = line->atTokNextint(&err);
    if (err < 0) {
        setError(RIL_E_SYSTEM_ERR);
        return false;
    }
    return false;
}

/*****************************************************************************
 * Class RmcCdmaWriteRuimSmsRsp
 *****************************************************************************/
RFX_IMPLEMENT_DATA_CLASS(RmcCdmaWriteRuimSmsRsp);
RmcCdmaWriteRuimSmsRsp::RmcCdmaWriteRuimSmsRsp(void* data, int length)
    : RmcVoidRsp(data, length), m_index(-1) {
    if (data != NULL) {
        m_index = *((int*)data);
        m_data = (void*)&m_index;
        m_length = length;
    }
}

RmcCdmaWriteRuimSmsRsp::RmcCdmaWriteRuimSmsRsp(int index, RIL_Errno e)
    : RmcVoidRsp(e), m_index(index) {
    if (e == RIL_E_SUCCESS) {
        m_data = (void*)&m_index;
        m_length = sizeof(m_index);
    }
}

RmcCdmaWriteRuimSmsRsp::~RmcCdmaWriteRuimSmsRsp() {}

/*****************************************************************************
 * Class RmcCdmaWriteRuimSmsHdlr
 *****************************************************************************/
RmcBaseRspData* RmcCdmaWriteRuimSmsHdlr::onGetRspData(RmcBaseReqData* req) {
    RmcCdmaWriteRuimSmsReq* writeReq = (RmcCdmaWriteRuimSmsReq*)req;
    return new RmcCdmaWriteRuimSmsRsp(writeReq->getIndex(), req->getError());
}
