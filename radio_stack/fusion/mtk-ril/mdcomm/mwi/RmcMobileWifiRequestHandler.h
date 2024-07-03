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

#ifndef __RMC_MOBILE_WIFI_REQUEST_HANDLER_H__
#define __RMC_MOBILE_WIFI_REQUEST_HANDLER_H__

#include "RfxBaseHandler.h"
#include <linux/wireless.h>

#define EWIFIEN_NEED_SEND_AP_MODE 1 << 4
#define EWIFIEN_AP_MODE_STATE 1 << 3
#define EWIFIEN_NEED_SEND_WIFI_ENABLED 1 << 2
#define EWIFIEN_WIFI_ENABLED_STATE 1 << 1
#define EWIFIEN_CAUSE 1 << 0

#define PRIV_CMD_SIZE 512
#define BEFORE_GEN97 0

/* wifi type: 11g, 11n, ... */
#define PRIV_CMD_GET_WIFI_TYPE 41
#define IOCTL_SET_STRUCT_FOR_EM (SIOCIWFIRSTPRIV + 11)

struct priv_driver_cmd_t {
    char buf[PRIV_CMD_SIZE];
    int used_len;
    int total_len;
};

class RmcMobileWifiRequestHandler : public RfxBaseHandler {
    RFX_DECLARE_HANDLER_CLASS(RmcMobileWifiRequestHandler);

  public:
    RmcMobileWifiRequestHandler(int slot_id, int channel_id);
    virtual ~RmcMobileWifiRequestHandler();

  protected:
    virtual void onHandleRequest(const sp<RfxMclMessage>& msg);
    virtual void onHandleTimer();

  private:
    void setWifiEnabled(const sp<RfxMclMessage>& msg);
    void setWifiAssociated(const sp<RfxMclMessage>& msg);
    void setWfcConfig(const sp<RfxMclMessage>& msg);
    void setWfcConfig_WifiUeMac(const sp<RfxMclMessage>& msg);
    void setWfcConfig_LocationSetting(const sp<RfxMclMessage>& msg);
    void setWifiSignal(const sp<RfxMclMessage>& msg);
    void setWifiIpAddress(const sp<RfxMclMessage>& msg);
    void setGeoLocation(const sp<RfxMclMessage>& msg);
    void setEmergencyAddressId(const sp<RfxMclMessage>& msg);
    void setNattKeepAliveStatus(const sp<RfxMclMessage>& msg);
    void setWifiPingResult(const sp<RfxMclMessage>& msg);
    /// M: Notify ePDG screen state
    void notifyEPDGScreenState(const sp<RfxMclMessage>& msg);
    void querySsacStatus(const sp<RfxMclMessage>& msg);
};

#endif
