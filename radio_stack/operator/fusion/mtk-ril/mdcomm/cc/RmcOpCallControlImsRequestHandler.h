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

#ifndef __RMC_OP_CALL_CONTROL_IMS_REQUEST_HANDLER_H__
#define __RMC_OP_CALL_CONTROL_IMS_REQUEST_HANDLER_H__

#define RFX_LOG_TAG "RmcOpImsCCReqHandler"

#include "cc/RmcCallControlBaseHandler.h"

class RmcOpCallControlImsRequestHandler : public RmcCallControlBaseHandler {
    RFX_DECLARE_HANDLER_CLASS(RmcOpCallControlCommonRequestHandler);

  public:
    RmcOpCallControlImsRequestHandler(int slot_id, int channel_id);
    virtual ~RmcOpCallControlImsRequestHandler();

  protected:
    virtual void onHandleRequest(const sp<RfxMclMessage>& msg);

    virtual void onHandleTimer();

  private:
    void requestDialFrom(const sp<RfxMclMessage>& msg);
    void setEmergencyCallConfig(const sp<RfxMclMessage>& msg);
    void deviceSwitch(const sp<RfxMclMessage>& msg);
    void cancelDeviceSwitch(const sp<RfxMclMessage>& msg);
};

#endif
