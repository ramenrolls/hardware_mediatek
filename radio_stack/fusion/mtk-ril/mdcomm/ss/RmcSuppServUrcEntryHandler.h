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

#ifndef __RMC_SUPP_SERV_URC_ENTRY_HANDLER_H__
#define __RMC_SUPP_SERV_URC_ENTRY_HANDLER_H__

#include "RfxBaseHandler.h"

#ifdef TAG
#undef TAG
#endif
#define TAG "RmcSSUrc"

class RmcSuppServUrcEntryHandler : public RfxBaseHandler {
    RFX_DECLARE_HANDLER_CLASS(RmcSuppServUrcEntryHandler);

  public:
    RmcSuppServUrcEntryHandler(int slot_id, int channel_id);
    virtual ~RmcSuppServUrcEntryHandler();

    // Override
  protected:
    // Process URC here
    virtual void onHandleUrc(const sp<RfxMclMessage>& msg);
    virtual void onHandleEvent(const sp<RfxMclMessage>& msg);

  private:
    bool isECMCCSS257Received = false;
    bool isCRINGReceived = false;
    RIL_SuppSvcNotification tmpSvcNotify;

    void handleOnUssd(const sp<RfxMclMessage>& msg);
    void handleOnFusionUssd(const sp<RfxMclMessage>& msg);
    void handleOnCfuNotify(const sp<RfxMclMessage>& msg);
    void handleOnLteSuppSvcNotification(const sp<RfxMclMessage>& msg);
    void retrieveCallForwardedToNumber(char* rawString, char* number);
    void handleOnUssi(const sp<RfxMclMessage>& msg);
    void handleOnXui(const sp<RfxMclMessage>& msg);
    void handleXcapResponseCodeNotify(const sp<RfxMclMessage>& msg);
    void handleCRINGReveiced();
    void handleECPI133Received(const sp<RfxMclMessage>& msg);
    void handleECMCCSS257Received();
    void resetFlagAndSvcNotify();
};
#endif /* __RMC_SUPP_SERV_URC_ENTRY_HANDLER_H__ */
