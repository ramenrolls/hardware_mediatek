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

#ifndef __RMC_CDMA_MO_SMS_H__
#define __RMC_CDMA_MO_SMS_H__
/*****************************************************************************
 * Include
 *****************************************************************************/
#include "common/RmcData.h"
#include "common/RmcMessageHandler.h"

/*****************************************************************************
 * Class RmcCdmaMoSmsMessage
 *****************************************************************************/
class RmcCdmaMoSmsMessage : public RmcSingleAtReq {
    RFX_DECLARE_DATA_CLASS(RmcCdmaMoSmsMessage);

  public:
    int getMsgRef() const { return m_msgRef; }

    int getMsgErrCode() const { return m_errCode; }

    const RIL_CDMA_SMS_Message& getMessage() const { return m_msg; }

  public:
    RmcCdmaMoSmsMessage(RIL_CDMA_SMS_Message* msg);

  protected:
    virtual RmcAtSendInfo* onGetAtInfo(RfxBaseHandler* h);
    virtual bool onHandleIntermediates(RfxAtLine* line, RfxBaseHandler* h);
    virtual void onHandleFinalResponseForError(RfxAtLine* line, RfxBaseHandler* h);

    // Overridable
  protected:
    virtual bool onGetPdu(String8& hexPdu, String8& address);

  private:
    RIL_CDMA_SMS_Message m_msg;
    int m_msgRef;
    int m_errCode;

    // Implement
  private:
    const int TELESERVICE_WMT = 0x1002;
    const int TELESERVICE_WEMT = 0x1005;
    const int ERROR_RUIM_ABSENT = 107;
    const int ERROR_FDN_CHECK = 14;
    const int ERROR_CLASS_TEMP = 2;
    const int ERROR_CLASS_PERMANENT = 3;

    bool isCtSimCard(RfxBaseHandler* h);
    void preProcessMessage(RfxBaseHandler* h);
};

/*****************************************************************************
 * Class RmcCdmaMoSmsOverImsMessage
 *****************************************************************************/
class RmcCdmaMoSmsOverImsMessage : public RmcCdmaMoSmsMessage {
    RFX_DECLARE_DATA_CLASS(RmcCdmaMoSmsOverImsMessage);

  protected:
    virtual bool onGetPdu(String8& hexPdu, String8& address);
};

/*****************************************************************************
 * Class RmcCdmaMoSmsRsp
 *****************************************************************************/
class RmcCdmaMoSmsRsp : public RmcVoidRsp {
    RFX_DECLARE_DATA_CLASS(RmcCdmaMoSmsRsp);

  public:
    RmcCdmaMoSmsRsp(int msgRef, int errCode, RIL_Errno e);

  private:
    RIL_SMS_Response m_response;
};

/*****************************************************************************
 * Class RmcCdmaMoSmsHdlr
 *****************************************************************************/
class RmcCdmaMoSmsHdlr : public RmcBaseRequestHandler {
  public:
    RmcCdmaMoSmsHdlr(RfxBaseHandler* h) : RmcBaseRequestHandler(h) {}
    virtual ~RmcCdmaMoSmsHdlr() {}

  public:
    virtual RmcBaseRspData* onGetRspData(RmcBaseReqData* req);
};

#endif /* __RMC_CDMA_MO_SMS_H__ */
