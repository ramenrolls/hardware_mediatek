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

#ifndef __RMC_MOBILE_WIFI_URC_HANDLER_H__
#define __RMC_MOBILE_WIFI_URC_HANDLER_H__

#include "RfxBaseHandler.h"

class RmcMobileWifiUrcHandler : public RfxBaseHandler {
    RFX_DECLARE_HANDLER_CLASS(RmcMobileWifiUrcHandler);

  public:
    RmcMobileWifiUrcHandler(int slot_id, int channel_id);
    virtual ~RmcMobileWifiUrcHandler();

  protected:
    virtual void onHandleUrc(const sp<RfxMclMessage>& msg);
    virtual void onHandleTimer();

  private:
    void onNotifyWfcWifiRoveout(const sp<RfxMclMessage>& msg);
    void onNotifyPdnHandover(const sp<RfxMclMessage>& msg);
    void onNotifyActiveWifiPdnCount(const sp<RfxMclMessage>& msg);
    void onNotifyWifiRssiMonitoringConfig(const sp<RfxMclMessage>& msg);
    void onNotifyWifiPdnError(const sp<RfxMclMessage>& msg);
    void onNotifyGeoRequest(const sp<RfxMclMessage>& msg);
    void onNotifyNattKeepAliveChanged(const sp<RfxMclMessage>& msg);
    void onNotifyWifiPingRequest(const sp<RfxMclMessage>& msg);
    void onNotifyWifiPdnOOS(const sp<RfxMclMessage>& msg);
    void onNotifyWifiLock(const sp<RfxMclMessage>& msg);
    void onNotifySsacStatus(const sp<RfxMclMessage>& msg);
};

#endif
