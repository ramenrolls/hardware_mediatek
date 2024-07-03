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
#ifndef __RMC_IMS_CONTROL_URC_HANDLER_H__
#define __RMC_IMS_CONTROL_URC_HANDLER_H__

#include "RfxBaseHandler.h"
#include "RfxIntsData.h"
#include "RfxStringsData.h"
#include "RfxImsRegInfoData.h"
#include "RfxVoidData.h"

#define RFX_LOG_TAG "RmcImsCtlUrcHdl"

class RmcImsControlUrcHandler : public RfxBaseHandler {
    RFX_DECLARE_HANDLER_CLASS(RmcImsControlUrcHandler);

  public:
    RmcImsControlUrcHandler(int slot_id, int channel_id);
    virtual ~RmcImsControlUrcHandler();

  protected:
    virtual void onHandleUrc(const sp<RfxMclMessage>& msg);
    virtual void onHandleTimer();
    virtual void onHandleEvent(const sp<RfxMclMessage>& msg);

  private:
    void handleImsDisabling(const sp<RfxMclMessage>& msg);
    void handleImsEnabling(const sp<RfxMclMessage>& msg);
    void handleImsDisabled(const sp<RfxMclMessage>& msg);
    void handleImsEnabled(const sp<RfxMclMessage>& msg);
    void handleImsRegistrationInfo(const sp<RfxMclMessage>& msg);
    void handleImsDereg(const sp<RfxMclMessage>& msg);
    void handleSipMsgIndication(const sp<RfxMclMessage>& msg);
    void handleImsEccSupportInfo(const sp<RfxMclMessage>& msg);
    void handleImsRtpInfo(const sp<RfxMclMessage>& msg);
    void handleImsEiregInfo(const sp<RfxMclMessage>& msg);
    void handleSipRegInfoInd(const sp<RfxMclMessage>& msg);
    int getIntFromTokenStr(char* tokenStr, int err, int base);
    void onImsVopsIndication(const sp<RfxMclMessage>& msg);
    void handleImsRegInfoInd(const sp<RfxMclMessage>& msg);
    void handleImsRegRespInd(const sp<RfxMclMessage>& msg);

  private:
    bool mPreferCireg;
};
#endif
