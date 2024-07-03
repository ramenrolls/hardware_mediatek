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

#ifndef __RMC_CDMA_BC_CONFIG_GET_H__
#define __RMC_CDMA_BC_CONFIG_GET_H__
/*****************************************************************************
 * Include
 *****************************************************************************/
#include "common/RmcData.h"
#include "common/RmcMessageHandler.h"
#include "RmcCdmaBcRangeParser.h"

/*****************************************************************************
 * Class RmcCdmaGetBcConfigReq
 *****************************************************************************/
class RmcCdmaGetBcConfigReq : RmcMultiAtReq {
    RFX_DECLARE_DATA_CLASS(RmcCdmaGetBcConfigReq);

    // External Method
  public:
    // Check if broadcast is activate in modem
    //
    // RETURNS: true if broadcast is activate in mode
    bool isBcActivate() { return m_mode == 1; }

    // Get the ranges of the channels
    //
    // RETURNS: ranges of channel
    const Vector<Range>& getChannels() const { return m_channels; }

    // Get the ranges of the languages
    //
    // RETURNS: ranges of the languages
    const Vector<Range>& getLanguages() const { return m_languages; }

    // Override
  protected:
    virtual RmcAtSendInfo* onGetFirstAtInfo(RfxBaseHandler* h);
    virtual RmcAtSendInfo* onGetNextAtInfo(const String8& cmd, RfxBaseHandler* h);
    virtual bool onHandleIntermediates(const String8& cmd, RfxAtLine* line, RfxBaseHandler* h);

    // Implementation
  private:
    int m_mode;
    Vector<Range> m_channels;
    Vector<Range> m_languages;
};

/*****************************************************************************
 * Class RmcCdmaGetBcConfigRsp
 *****************************************************************************/
class RmcCdmaGetBcConfigRsp : public RmcVoidRsp {
    RFX_DECLARE_DATA_CLASS(RmcCdmaGetBcConfigRsp);
    // Constructor / Destructor
  public:
    RmcCdmaGetBcConfigRsp(Vector<RIL_CDMA_BroadcastSmsConfigInfo> infos, RIL_Errno e);
    // Implementation
  private:
    Vector<RIL_CDMA_BroadcastSmsConfigInfo> m_infos;
    Vector<RIL_CDMA_BroadcastSmsConfigInfo*> m_pInfos;
};

/*****************************************************************************
 * Class RmcCdmaBcGetConfigHdlr
 *****************************************************************************/
class RmcCdmaBcGetConfigHdlr : public RmcBaseRequestHandler {
    // Constructor / Destructor
  public:
    RmcCdmaBcGetConfigHdlr(RfxBaseHandler* h) : RmcBaseRequestHandler(h) {}

    virtual ~RmcCdmaBcGetConfigHdlr() {}
    // Override
  public:
    virtual RmcBaseRspData* onGetRspData(RmcBaseReqData* req);
};
#endif /* __RMC_CDMA_BC_CONFIG_GET_H__ */
