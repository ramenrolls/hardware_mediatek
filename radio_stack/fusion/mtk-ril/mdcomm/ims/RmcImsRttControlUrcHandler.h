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
#ifndef __RMC_IMS_RTT_CONTROL_URC_HANDLER_H__
#define __RMC_IMS_RTT_CONTROL_URC_HANDLER_H__

#include "RfxBaseHandler.h"
#include "RfxIntsData.h"
#include "RfxStringsData.h"
#include "RfxVoidData.h"

#define RFX_LOG_TAG "RmcImsRttCtlUrcHdl"

class RmcImsRttControlUrcHandler : public RfxBaseHandler {
    RFX_DECLARE_HANDLER_CLASS(RmcImsRttControlUrcHandler);

  public:
    RmcImsRttControlUrcHandler(int slot_id, int channel_id);
    virtual ~RmcImsRttControlUrcHandler();

  protected:
    virtual void onHandleUrc(const sp<RfxMclMessage>& msg);

  private:
    void onRttModifyResponse(const sp<RfxMclMessage>& msg);
    void onRttTextReceive(const sp<RfxMclMessage>& msg);
    void onRttCapabilityIndication(const sp<RfxMclMessage>& msg);
    void onRttModifyRequestReceive(const sp<RfxMclMessage>& msg);
    void onAudioIndication(const sp<RfxMclMessage>& msg);
};
#endif
