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

#ifndef __RMC_SUPP_SERV_REQUEST_HANDLER_H__
#define __RMC_SUPP_SERV_REQUEST_HANDLER_H__

#include "RmcSuppServRequestBaseHandler.h"
#include "RmcSuppServUssdBaseHandler.h"
#include "SuppServDef.h"
#include <string>

#ifdef TAG
#undef TAG
#endif
#define TAG "RmcSSHandler"

using std::string;

class RmcSuppServRequestHandler : public RmcSuppServRequestBaseHandler {
    RFX_DECLARE_HANDLER_CLASS(RmcSuppServRequestHandler);

  public:
    RmcSuppServRequestHandler(int slot_id, int channel_id);
    virtual ~RmcSuppServRequestHandler();

  protected:
    virtual void onHandleRequest(const sp<RfxMclMessage>& msg);
    virtual void onHandleEvent(const sp<RfxMclMessage>& msg);
    virtual void onHandleTimer();

  private:
    void requestSetClir(const sp<RfxMclMessage>& msg);
    void requestGetClir(const sp<RfxMclMessage>& msg);
    void requestSetCallForward(const sp<RfxMclMessage>& msg);
    void requestSetCallForwardInTimeSlot(const sp<RfxMclMessage>& msg);
    void requestSetCallWaiting(const sp<RfxMclMessage>& msg);
    void requestChangeBarringPassword(const sp<RfxMclMessage>& msg);
    void requestSendUSSD(const sp<RfxMclMessage>& msg);
    void requestCancelUssd(const sp<RfxMclMessage>& msg);
    void requestQueryClip(const sp<RfxMclMessage>& msg);
    void requestSetClip(const sp<RfxMclMessage>& msg);

    // Should be in protect.
    void requestGetColp(const sp<RfxMclMessage>& msg);
    void requestSetColp(const sp<RfxMclMessage>& msg);
    void requestGetColr(const sp<RfxMclMessage>& msg);
    void requestSetColr(const sp<RfxMclMessage>& msg);
    void requestSendCNAP(const sp<RfxMclMessage>& msg);
    void requestSendUSSI(const sp<RfxMclMessage>& msg);
    void requestCancelUssi(const sp<RfxMclMessage>& msg);
    void requestSetSuppSvcNotification(const sp<RfxMclMessage>& msg);
    void requestSetupXcapUserAgentString(const sp<RfxMclMessage>& msg);
    void requestSetSuppServProperty(const sp<RfxMclMessage>& msg);
    void requestGetSuppServProperty(const sp<RfxMclMessage>& msg);
    void requestGetModemVersion();
    void requestSetXcapConfig(const sp<RfxMclMessage>& msg);
    void syncCLIRToStatusManager();

    void* startUtInterface(const char* libutinterfacePath);

  private:
    RmcSuppServUssdBaseHandler* m_UssdHandler;

    /**
     * When EM wants to reset XCAP config, it should be trigger by oemhook using
     * reset and resetdone command. And except the reset cmd, all properties only need
     * to set the property for UI to get status.
     */
    bool isResetSession;
    void* hDll = NULL;
};

#endif
