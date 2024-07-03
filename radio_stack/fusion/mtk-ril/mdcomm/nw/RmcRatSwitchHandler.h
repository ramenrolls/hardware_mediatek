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

#ifndef __RMC_RAT_SWITCH_HANDLER_H__
#define __RMC_RAT_SWITCH_HANDLER_H__

#include "RmcNetworkHandler.h"
#include "libmtkrilutils.h"

#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "RmcRatSwHdlr"

class RmcRatSwitchHandler : public RmcNetworkHandler {
    RFX_DECLARE_HANDLER_CLASS(RmcRatSwitchHandler);

  public:
    RmcRatSwitchHandler(int slot_id, int channel_id);
    virtual ~RmcRatSwitchHandler();

  protected:
    virtual void onHandleRequest(const sp<RfxMclMessage>& msg);

    virtual void onHandleUrc(const sp<RfxMclMessage>& msg) { RFX_UNUSED(msg); }

    virtual void onHandleEvent(const sp<RfxMclMessage>& msg);

    virtual void onHandleTimer() {}

  private:
    void requestAbortQueryAvailableNetworks(const sp<RfxMclMessage>& msg);
    void requestStopNetworkScan(const sp<RfxMclMessage>& msg);
    void requestSetPreferredNetworkType(const sp<RfxMclMessage>& msg);
    void requestGetPreferredNetworkType(const sp<RfxMclMessage>& msg);
    void requestVoiceRadioTech(const sp<RfxMclMessage>& msg);

    bool isRatPreferred();
    bool isInCall();

    void requestGetGmssRatMode(const sp<RfxMclMessage>& msg);

    void requestSetDisable2G(const sp<RfxMclMessage>& msg);
    void requestGetDisable2G(const sp<RfxMclMessage>& msg);

  protected:
    int m_slot_id;
    int m_channel_id;

  private:
    int mLastReqRatType;
    int mRetryCount;
    bool tc1_support;
};

#endif
