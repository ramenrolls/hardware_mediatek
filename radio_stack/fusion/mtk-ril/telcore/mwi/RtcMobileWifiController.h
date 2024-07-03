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

#ifndef __RFX_MOBILE_WIFI_CONTROLLER_H__
#define __RFX_MOBILE_WIFI_CONTROLLER_H__

/*****************************************************************************
 * Include
 *****************************************************************************/

#include "RfxController.h"

/*****************************************************************************
 * Class RfxController
 *****************************************************************************/

class RfxStringsData;

class RtcMobileWifiController : public RfxController {
    // Required: declare this class
    RFX_DECLARE_CLASS(RtcMobileWifiController);

  public:
    RtcMobileWifiController();
    virtual ~RtcMobileWifiController();

    // Override
  protected:
    virtual void onInit();
    virtual void onDeinit();
    virtual bool onHandleRequest(const sp<RfxMessage>& message);
    virtual bool onHandleUrc(const sp<RfxMessage>& message);
    virtual bool onHandleResponse(const sp<RfxMessage>& response);
    virtual bool onCheckIfRejectMessage(const sp<RfxMessage>& message, bool isModemPowerOff,
                                        int radioState);
    void onWFCPDNStatusChanged(RfxStatusKeyEnum key, RfxVariant old_value, RfxVariant new_value);

  private:
};

#endif /* __RFX_MOBILE_WIFI_CONTROLLER_H__ */
