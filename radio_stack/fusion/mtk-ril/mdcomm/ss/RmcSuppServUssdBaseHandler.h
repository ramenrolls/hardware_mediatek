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

#ifndef __RMC_SUPP_SERV_USSD_BASE_HANDLER_H__
#define __RMC_SUPP_SERV_USSD_BASE_HANDLER_H__

#include "RfxBaseHandler.h"

#ifdef TAG
#undef TAG
#endif
#define TAG "RmcSSUssdBaseHandler"

static const bool DBG = true;

class RmcSuppServUssdBaseHandler : public RfxBaseHandler {
    RFX_DECLARE_HANDLER_CLASS(RmcSuppServUssdBaseHandler);

  public:
    RmcSuppServUssdBaseHandler(int slot_id, int channel_id);
    virtual ~RmcSuppServUssdBaseHandler();

    // Child must implement those USSD operations
    virtual void requestSendUssdDomainSelect(const sp<RfxMclMessage>& msg) = 0;
    virtual void requestCancelUssdDomainSelect(const sp<RfxMclMessage>& msg) = 0;
    virtual void handleOnUssd(const sp<RfxMclMessage>& msg) = 0;
    virtual void handleOnUssi(const sp<RfxMclMessage>& msg) = 0;

  protected:
    virtual void onHandleRequest(const sp<RfxMclMessage>& msg);
    virtual void onHandleEvent(const sp<RfxMclMessage>& msg);
    void sendFailureReport(const sp<RfxMclMessage>& msg, RIL_Errno ret);

    // USSI preconditions
    bool isFdnAllowed(const char* ussi);
    bool isVopsOn();
    bool isImsRegOn();
    bool isInImsCall();

  private:
    void handleUssdDomainInfoReq(const sp<RfxMclMessage>& msg);
};

#endif
