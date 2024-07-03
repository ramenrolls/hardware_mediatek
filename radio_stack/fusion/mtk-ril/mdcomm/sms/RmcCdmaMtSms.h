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

#ifndef __RMC_CDMA_MT_SMS_H__
#define __RMC_CDMA_MT_SMS_H__
/*****************************************************************************
 * Include
 *****************************************************************************/
#include "common/RmcData.h"
#include "common/RmcMessageHandler.h"

/*****************************************************************************
 * Class RmcCdmaMtSmsMessage
 *****************************************************************************/
class RmcCdmaMtSmsMessage : public RmcVoidUrsp {
    RFX_DECLARE_DATA_CLASS(RmcCdmaMtSmsMessage);

  public:
    int getReplySeqNo() const { return m_replySeqNo; }

    const RIL_CDMA_SMS_Address* getAddress() const { return &m_msg.sAddress; }

  public:
    RmcCdmaMtSmsMessage(int urcId, int slotId, String8& hexPdu);

  private:
    RIL_CDMA_SMS_Message m_msg;
    int m_replySeqNo;
};

/*****************************************************************************
 * Class RmcCdmaMtSmsHdlr
 *****************************************************************************/
class RmcCdmaMtSmsHdlr : public RmcBaseUrcHandler {
    // Constructor / Destructor
  public:
    RmcCdmaMtSmsHdlr(RfxBaseHandler* h, int type)
        : RmcBaseUrcHandler(h), m_type(type), m_data(-1) {}
    virtual ~RmcCdmaMtSmsHdlr() {}

  public:
    virtual bool onHandleRawUrc(RfxAtLine* line);
    virtual RmcBaseUrspData* onGetUrcData(int slotId);

  private:
    int m_type;
    int m_data;
    String8 m_hexPdu;
};

#endif /* __RMC_CDMA_MT_SMS_H__ */
