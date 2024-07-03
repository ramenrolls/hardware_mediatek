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

#ifndef __RMC_NETWORK_NRT_REQUEST_HANDLER_H__
#define __RMC_NETWORK_NRT_REQUEST_HANDLER_H__

#include "RmcNetworkHandler.h"
#include "RfxNetworkScanData.h"
#include "RfxNetworkScanResultData.h"

#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "RmcNwReqHdlr"

class RmcNetworkNrtRequestHandler : public RmcNetworkHandler {
    RFX_DECLARE_HANDLER_CLASS(RmcNetworkNrtRequestHandler);

  public:
    RmcNetworkNrtRequestHandler(int slot_id, int channel_id);
    virtual ~RmcNetworkNrtRequestHandler();

  protected:
    virtual void onHandleRequest(const sp<RfxMclMessage>& msg);
    virtual void onHandleEvent(const sp<RfxMclMessage>& msg);

  private:
    void requestQueryAvailableNetworks(const sp<RfxMclMessage>& msg);
    void requestQueryAvailableNetworksWithAct(const sp<RfxMclMessage>& msg);
    void requestStartNetworkScan(const sp<RfxMclMessage>& msg);
    void handleSimulatedNetworkScan();

  protected:
    int m_slot_id;
    int m_channel_id;
    int mPlmn_list_format;
};
#endif
