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

#ifndef __RMC_CDMA_SMS_ACK__
#define __RMC_CDMA_SMS_ACK__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "common/RmcData.h"
#include "common/RmcMessageHandler.h"

/*****************************************************************************
 * Class RmcCdmaSmsAck
 *****************************************************************************/
class RmcCdmaSmsAck : public RmcSingleAtReq {
    RFX_DECLARE_DATA_CLASS(RmcCdmaSmsAck);

  public:
    RmcCdmaSmsAck(int type, int replyReqNo, RIL_CDMA_SMS_Ack ack,
                  const RIL_CDMA_SMS_Address* pAddr);
    virtual RmcAtSendInfo* onGetAtInfo(RfxBaseHandler* h);

    bool setCmd(int type, int replySeqNo, const RIL_CDMA_SMS_Address* pAddr);

    const String8& getCmd() const { return m_cmd; }

  private:
    RIL_CDMA_SMS_Ack m_ack;
    String8 m_cmd;
};

/*****************************************************************************
 * Class RmcCdmaSmsAckHdlr
 *****************************************************************************/
class RmcCdmaSmsAckHdlr : public RmcBaseRequestHandler {
  public:
    RmcCdmaSmsAckHdlr(RfxBaseHandler* h) : RmcBaseRequestHandler(h) {}
    virtual ~RmcCdmaSmsAckHdlr() {}

  public:
    virtual void onAfterResponse();
};

#endif /* __RMC_CDMA_SMS_ACK__ */
