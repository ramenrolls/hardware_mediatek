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

#ifndef __RMC_OP_NETWORK_REQUEST_HANDLER_H__
#define __RMC_OP_NETWORK_REQUEST_HANDLER_H__

#include "RfxBaseHandler.h"
#include "RfxIntsData.h"
#include "nw/RmcNetworkHandler.h"

#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "RmcOpNetworkRequestHandler"

class RmcOpNetworkRequestHandler : public RmcNetworkHandler {
    RFX_DECLARE_HANDLER_CLASS(RmcOpNetworkRequestHandler);

  public:
    RmcOpNetworkRequestHandler(int slot_id, int channel_id);
    virtual ~RmcOpNetworkRequestHandler();

  protected:
    virtual void onHandleRequest(const sp<RfxMclMessage>& msg);

  private:
    void requestAntennaConf(const sp<RfxMclMessage>& msg);
    void requestAntennaInfo(const sp<RfxMclMessage>& msg);

  protected:
    // MTK_TC1_FEATURE for Antenna Testing start
    int antennaTestingType = 0;
    // MTK_TC1_FEATURE for Antenna Testing end
};

#endif
