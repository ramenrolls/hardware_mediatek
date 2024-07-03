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

#ifndef __RFX_SUPP_SERV_CONTROLLER_H__
#define __RFX_SUPP_SERV_CONTROLLER_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "RfxController.h"
#include "RfxTimer.h"
#include "RfxVoidData.h"
#include "RfxRootController.h"
#include "RfxStringData.h"
#include "RfxStringsData.h"
#include "RfxIntsData.h"
#include "RtcSuppServQueue.h"

enum UssdDestination {
    USSD_URC_TO_GSM = 0,  // default value
    USSD_URC_TO_IMS = 1
};

/*****************************************************************************
 * Class RfxController
 *****************************************************************************/
class RtcSuppServController : public RfxController {
    // Required: declare this class
    RFX_DECLARE_CLASS(RtcSuppServController);

  public:
    RtcSuppServController();
    virtual ~RtcSuppServController();

    // Override
  protected:
    virtual bool onHandleRequest(const sp<RfxMessage>& message);
    virtual bool onHandleUrc(const sp<RfxMessage>& message);
    virtual bool onHandleResponse(const sp<RfxMessage>& message);
    virtual void onInit();
    virtual void onDeinit();
    virtual bool onCheckIfRejectMessage(const sp<RfxMessage>& message, bool isModemPowerOff,
                                        int radioState);
    virtual bool onHandleAtciRequest(const sp<RfxMessage>& message);

  private:
    void handleSSRequest(const sp<RfxMessage>& message);
    void handleSSResponse(const sp<RfxMessage>& message);
    void handleUssdDomainInfoReqResponse(const sp<RfxMessage>& message);
    bool handleAtciCallWaitingRequest(const sp<RfxMessage>& message);
    void responseToAtci(const sp<RfxMessage>& message);
    void onSimIccidChanged(RfxStatusKeyEnum key, RfxVariant old_value, RfxVariant value);
    RtcSuppServQueue* mQueue;

    // An USSD may be sent from GSM or IMS. We need a private variable to remember where it
    // comes from. Then RILD can return the following USSD URC to correct place.
    UssdDestination mUssdDestination;

    // Operation of UssdDestination
    UssdDestination getUssdDestination();
    void setUssdDestination(UssdDestination destination);
    const char* ussdDestinationToString(UssdDestination destination);
};

#endif /* __RFX_SUPP_SERV_CONTROLLER_H__ */
