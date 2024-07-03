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

#ifndef __RMC_OP_RAT_SWITCH_HANDLER_H__
#define __RMC_OP_RAT_SWITCH_HANDLER_H__

#include "RfxBaseHandler.h"
#include "RfxVoidData.h"
#include "nw/RmcNetworkHandler.h"
#include "libmtkrilutils.h"

#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "RmcOpRatSwitchHandler"

class RmcOpRatSwitchHandler : public RmcNetworkHandler {
    RFX_DECLARE_HANDLER_CLASS(RmcOpRatSwitchHandler);

  public:
    RmcOpRatSwitchHandler(int slot_id, int channel_id);
    virtual ~RmcOpRatSwitchHandler();

  protected:
    virtual void onHandleRequest(const sp<RfxMclMessage>& msg);

  private:
    void requestSetDisable2G(const sp<RfxMclMessage>& msg);
    void requestGetDisable2G(const sp<RfxMclMessage>& msg);
};

#endif
