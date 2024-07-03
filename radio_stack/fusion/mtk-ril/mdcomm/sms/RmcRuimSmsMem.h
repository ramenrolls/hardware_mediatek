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

#ifndef __RMC_RUIM_SMS_MEM_H__
#define __RMC_RUIM_SMS_MEM_H__
/*****************************************************************************
 * Include
 *****************************************************************************/
#include "common/RmcData.h"
#include "common/RmcMessageHandler.h"

/*****************************************************************************
 * Class RmcCdmaGetRuimSmsMemReq
 *****************************************************************************/
class RmcCdmaGetRuimSmsMemReq : RmcSingleAtReq {
    RFX_DECLARE_DATA_CLASS(RmcCdmaGetRuimSmsMemReq);

  public:
    int getUsed() const { return m_used; }

    int getTotal() const { return m_total; }

  protected:
    virtual RmcAtSendInfo* onGetAtInfo(RfxBaseHandler* h);
    virtual bool onHandleIntermediates(RfxAtLine* line, RfxBaseHandler* h);

  private:
    int m_used;
    int m_total;
};

/*****************************************************************************
 * Class RmcCdmaGetRuimSmsMemRsp
 *****************************************************************************/
class RmcCdmaGetRuimSmsMemRsp : public RmcVoidRsp {
    RFX_DECLARE_DATA_CLASS(RmcCdmaGetRuimSmsMemRsp);

  public:
    RmcCdmaGetRuimSmsMemRsp(int total, int used, RIL_Errno e);

  private:
    struct MemInfo {
        MemInfo() : m_used(0), m_total(0) {}
        MemInfo(int total, int used) : m_used(used), m_total(total) {}
        int m_used;
        int m_total;
    };

    MemInfo m_info;
};

/*****************************************************************************
 * Class RmcCdmaGetRuimSmsMemHdlr
 *****************************************************************************/
class RmcCdmaGetRuimSmsMemHdlr : public RmcBaseRequestHandler {
  public:
    RmcCdmaGetRuimSmsMemHdlr(RfxBaseHandler* h) : RmcBaseRequestHandler(h) {}
    virtual ~RmcCdmaGetRuimSmsMemHdlr() {}

  public:
    virtual RmcBaseRspData* onGetRspData(RmcBaseReqData* req);
};

#endif /* __RMC_RUIM_SMS_MEM_H__ */
