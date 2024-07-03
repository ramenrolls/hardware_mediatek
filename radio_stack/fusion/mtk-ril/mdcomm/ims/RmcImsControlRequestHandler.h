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

#ifndef __RMC_IMS_CONTROL_REQUEST_HANDLER_H__
#define __RMC_IMS_CONTROL_REQUEST_HANDLER_H__

#include "RfxBaseHandler.h"
#include "RfxIntsData.h"
#include "RfxVoidData.h"
#include "RfxStringData.h"
#include "RfxStringsData.h"
#include "RfxMessageId.h"

#define RFX_LOG_TAG "RmcImsCtlReqHdl"

#define MAX_MD_CFGTYPE_NUM 3
typedef enum { ECFGSET = 0, EWOCFGSET = 1, EIWLCFGSET = 2 } MD_CFGTYPE;
static const char* cfgAtCmd[] = {"AT+ECFGSET", "AT+EWOCFGSET", "AT+EIWLCFGSET"};

class RmcImsControlRequestHandler : public RfxBaseHandler {
    RFX_DECLARE_HANDLER_CLASS(RmcImsControlRequestHandler);

  private:
    String8 userAgentformat;

  public:
    RmcImsControlRequestHandler(int slot_id, int channel_id);
    virtual ~RmcImsControlRequestHandler();

  protected:
    virtual void onHandleRequest(const sp<RfxMclMessage>& msg);

  private:
    void requestSetVolteEnabled(const sp<RfxMclMessage>& msg);
    void requestSetWfcEnabled(const sp<RfxMclMessage>& msg);
    void requestSetVilteEnabled(const sp<RfxMclMessage>& msg);
    void requestSetViwifiEnabled(const sp<RfxMclMessage>& msg);
    void requestSetImsCfg(const sp<RfxMclMessage>& msg);
    void requestSetModemImsCfg(const sp<RfxMclMessage>& msg);
    void requestSendWfcProfile(const sp<RfxMclMessage>& msg);
    void enableImsRegistrationReport(const sp<RfxMclMessage>& msg);
    void requestImsRegistrationState(const sp<RfxMclMessage>& msg);
    void requestSetImsEnabled(const sp<RfxMclMessage>& msg);
    void requestImsDereg(const sp<RfxMclMessage>& msg);
    void enableImsEccSupportReport();
    void requestSetImsRtpReport(const sp<RfxMclMessage>& msg);
    void requestMultiImsSupportCount();
    void enableImsRcsStateFeature();
    void enableVoPSUrc();
    void enableSipRegInfoInd();
    void requestSetSipUserAgent();
    bool isCurrentSlotSupportIms();
    void queryVopsStatus(const sp<RfxMclMessage>& msg);
    void enableVopsStatusReport();
};

#endif
