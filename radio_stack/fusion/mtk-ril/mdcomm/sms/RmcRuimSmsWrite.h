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

#ifndef __RMC_RUIM_SMS_WRITE_H__
#define __RMC_RUIM_SMS_WRITE_H__
/*****************************************************************************
 * Include
 *****************************************************************************/
#include "common/RmcData.h"
#include "common/RmcMessageHandler.h"

/*****************************************************************************
 * Class RmcCdmaWriteRuimSmsReq
 *****************************************************************************/
class RmcCdmaWriteRuimSmsReq : RmcSingleAtReq {
    RFX_DECLARE_DATA_CLASS(RmcCdmaWriteRuimSmsReq);

  public:
    virtual RmcAtSendInfo* onGetAtInfo(RfxBaseHandler* h);

    virtual bool onHandleIntermediates(RfxAtLine* line, RfxBaseHandler* h);

    int getIndex() const { return m_index; }

  private:
    RIL_CDMA_SMS_WriteArgs m_writeArgs;
    int m_index;
};

/*****************************************************************************
 * Class RmcCdmaWriteRuimSmsRsp
 *****************************************************************************/
class RmcCdmaWriteRuimSmsRsp : public RmcVoidRsp {
    RFX_DECLARE_DATA_CLASS(RmcCdmaWriteRuimSmsRsp);

  public:
    RmcCdmaWriteRuimSmsRsp(int index, RIL_Errno e);

  private:
    int m_index;
};

/*****************************************************************************
 * Class RmcCdmaWriteRuimSmsHdlr
 *****************************************************************************/
class RmcCdmaWriteRuimSmsHdlr : public RmcBaseRequestHandler {
  public:
    RmcCdmaWriteRuimSmsHdlr(RfxBaseHandler* h) : RmcBaseRequestHandler(h) {}
    virtual ~RmcCdmaWriteRuimSmsHdlr() {}

  public:
    virtual RmcBaseRspData* onGetRspData(RmcBaseReqData* req);
};

#endif /* __RMC_RUIM_SMS_WRITE_H__ */
