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

#ifndef __RMC_GSM_SMS_REQUEST_HANDLER_H__
#define __RMC_GSM_SMS_REQUEST_HANDLER_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "RmcGsmSmsBaseHandler.h"
#include "RmcGsmSmsDefs.h"

/*****************************************************************************
 * Class RpSimController
 *****************************************************************************/

class RmcGsmSmsRequestHandler : public RmcGsmSmsBaseHandler {
    RFX_DECLARE_HANDLER_CLASS(RmcGsmSmsRequestHandler);

  public:
    RmcGsmSmsRequestHandler(int slot_id, int channel_id);

    // Override
  protected:
    // Process Request here
    virtual void onHandleRequest(const sp<RfxMclMessage>& msg);
    virtual void onHandleEvent(const sp<RfxMclMessage>& msg);

  private:
    // Utility functions
    void setTag(String8 tag) { mTag = tag; }
    void sendNewSmsErrorAck(const sp<RfxMclMessage>& msg);
    RIL_GSM_BroadcastSmsConfigInfo** convertChStringToGsmCbCfgInfo(char* channels, int* count);

    // Request handlers
    void handleSendSms(const sp<RfxMclMessage>& msg);
    void handleSmsAcknowledge(const sp<RfxMclMessage>& msg);
    void handleWriteSmsToSim(const sp<RfxMclMessage>& msg);
    void handleDeleteSmsOnSim(const sp<RfxMclMessage>& msg);
    void handleReportSmsMemStatus(const sp<RfxMclMessage>& msg);
    void handleGetSmsSimMemStatus(const sp<RfxMclMessage>& msg);
    void handleGetSmscAddress(const sp<RfxMclMessage>& msg);
    void handleSetSmscAddress(const sp<RfxMclMessage>& msg);
    void handleSmsAcknowledgeWithPdu(const sp<RfxMclMessage>& msg);
    void handleGetGsmCbSmsCfg(const sp<RfxMclMessage>& msg, GSM_CB_QUERY_MODE query);
    void handleSetGsmCbSmsCfg(const sp<RfxMclMessage>& msg);
    void handleSetGsmCbSmsLanguage(const sp<RfxMclMessage>& msg);
    void handleGsmCbSmsActivation(const sp<RfxMclMessage>& msg);
    void handleGsmGetCbSmsActivation(const sp<RfxMclMessage>& msg);
    void handleGsmEtwsActivation(const sp<RfxMclMessage>& msg);
    void handleRemoveCbMsg(const sp<RfxMclMessage>& msg);
    void handleGetSmsParams(const sp<RfxMclMessage>& msg);
    void handleSetSmsParams(const sp<RfxMclMessage>& msg);

  private:
    String8 mTag;
};
#endif /* __RMC_GSM_SMS_REQUEST_HANDLER_H__ */
