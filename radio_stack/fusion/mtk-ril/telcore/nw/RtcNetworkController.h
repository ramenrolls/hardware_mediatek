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

#ifndef __RTC_NETWORK_CONTROLLER_H__
#define __RTC_NETWORK_CONTROLLER_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "RfxController.h"
#include <telephony/mtk_ril.h>
#include "rfx_properties.h"
#include "RfxVoidData.h"
#include "RfxNetworkScanResultData.h"
#include "nw/RtcRatSwitchController.h"

/*****************************************************************************
 * Define
 *****************************************************************************/

/*****************************************************************************
 * Class RfxController
 *****************************************************************************/
class RtcNetworkController : public RfxController {
    RFX_DECLARE_CLASS(RtcNetworkController);

  public:
    RtcNetworkController();
    virtual ~RtcNetworkController();

  protected:
    virtual bool onHandleRequest(const sp<RfxMessage>& message);
    virtual bool onHandleUrc(const sp<RfxMessage>& message);
    virtual bool onHandleResponse(const sp<RfxMessage>& message);
    virtual void onInit();

    virtual bool onCheckIfRejectMessage(const sp<RfxMessage>& message, bool isModemPowerOff,
                                        int radioState);
    virtual bool onPreviewMessage(const sp<RfxMessage>& message);
    virtual bool onCheckIfResumeMessage(const sp<RfxMessage>& message);

    void onHandleModemReset(RfxStatusKeyEnum key, RfxVariant old_value, RfxVariant value);
    void onLocalAbortAvailableNetworkDone();
    bool isAPInCall();

  private:
    bool mNetworkScanOngoing;
    bool mForceStopNetworkScan;
    sp<RfxMessage> mNwScanMessage;
};
#endif /* __RTC_NETWORK_CONTROLLER_H__ */
