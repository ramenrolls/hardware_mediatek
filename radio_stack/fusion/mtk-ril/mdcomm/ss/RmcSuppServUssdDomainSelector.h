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

#ifndef __RMC_SUPP_SERV_USSD_DOMAIN_SELECTOR_H__
#define __RMC_SUPP_SERV_USSD_DOMAIN_SELECTOR_H__

#include "RfxBaseHandler.h"
#include "RmcSuppServUssdBaseHandler.h"

#ifdef TAG
#undef TAG
#endif
#define TAG "RmcSSUssdDomainSelector"

enum UssiAction {
    USSI_REQUEST = 1,  // UE initial USSI request
    USSI_RESPONSE = 2  // Response network USSI
};

enum UssdReportCase {
    SEND_NOTHING_BACK = 0,   // No need to send anything back to framework
    SEND_RESPONSE_BACK = 1,  // Send USSD Response back to framework
    SEND_URC_BACK = 2        // Send USSD URC back to framework
};

class RmcSuppServUssdDomainSelector : public RmcSuppServUssdBaseHandler {
    RFX_DECLARE_HANDLER_CLASS(RmcSuppServUssdDomainSelector);

  public:
    RmcSuppServUssdDomainSelector(int slot_id, int channel_id);
    virtual ~RmcSuppServUssdDomainSelector();

    virtual void requestSendUssdDomainSelect(const sp<RfxMclMessage>& msg);
    virtual void requestCancelUssdDomainSelect(const sp<RfxMclMessage>& msg);
    virtual void handleOnUssd(const sp<RfxMclMessage>& msg);
    virtual void handleOnUssi(const sp<RfxMclMessage>& msg);

  private:
    void handleUssiCSFB(const sp<RfxMclMessage>& msg);
    void requestSendUSSD(const sp<RfxMclMessage>& msg, UssdReportCase reportCase);
    void requestCancelUssd(const sp<RfxMclMessage>& msg);
    void requestSendUSSI(const sp<RfxMclMessage>& msg);
    void requestCancelUssi(const sp<RfxMclMessage>& msg);
    sp<RfxMclMessage> convertUssiToUssdUrc(const sp<RfxMclMessage>& msg);

    // Operation of USSI action
    UssiAction getUssiAction();
    void setUssiAction(UssiAction action);
    const char* ussiActionToString(UssiAction action);

  private:
    // Java framework has the ability to know which action it should take, then
    // we can pass correct EIUSD's argument to IMS Stack
    // However, if USSI comes from GSM way, there is no dirtect information for RIL SS to
    // determine that, so we need a state variable to remember it.
    // If we get "further user action required" (<n>=1, <m>=1) from USSI URC,
    // we set this state variable to USSI_RESPONSE. It means current USSI session is
    // interactive, AP has to tell IMS stack it's not a request, it's a response.
    // Otherwirse, we treat the action as USSI_REQUEST by default.
    UssiAction mUssiAction;

    // To cancel correct USSD session (CS or IMS), need a flag to memorize
    // which domain it is for current ongoing USSD session
    // 0: CS,  1: IMS
    int mOngoingSessionDomain;

    // Snapshot the USSI string. If we receive error from USSI URC, need to do USSD CSFB
    // using the same string
    char* mUssiSnapshot;
};

#endif
