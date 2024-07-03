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

#ifndef __RMC_SUPP_SERV_USSD_HANDLER_H__
#define __RMC_SUPP_SERV_USSD_HANDLER_H__

#include "RfxBaseHandler.h"
#include "RmcSuppServUssdBaseHandler.h"

#ifdef TAG
#undef TAG
#endif
#define TAG "RmcSSUssdHandler"

class RmcSuppServUssdHandler : public RmcSuppServUssdBaseHandler {
    RFX_DECLARE_HANDLER_CLASS(RmcSuppServUssdHandler);

  public:
    RmcSuppServUssdHandler(int slot_id, int channel_id);
    virtual ~RmcSuppServUssdHandler();

    virtual void requestSendUssdDomainSelect(const sp<RfxMclMessage>& msg);
    virtual void requestCancelUssdDomainSelect(const sp<RfxMclMessage>& msg);
    virtual void handleOnUssd(const sp<RfxMclMessage>& msg);
    virtual void handleOnUssi(const sp<RfxMclMessage>& msg);

  private:
    void requestSendUSSD(const sp<RfxMclMessage>& msg);
    void requestCancelUssd(const sp<RfxMclMessage>& msg);
};

#endif
