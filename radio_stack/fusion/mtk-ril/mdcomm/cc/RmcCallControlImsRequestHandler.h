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

#ifndef __RMC_CALL_CONTROL_IMS_REQUEST_HANDLER_H__
#define __RMC_CALL_CONTROL_IMS_REQUEST_HANDLER_H__

#include "RmcCallControlBaseHandler.h"

class RmcCallControlImsRequestHandler : public RmcCallControlBaseHandler {
    RFX_DECLARE_HANDLER_CLASS(RmcCallControlCommonRequestHandler);

  public:
    RmcCallControlImsRequestHandler(int slot_id, int channel_id);
    virtual ~RmcCallControlImsRequestHandler();

  protected:
    virtual void onHandleRequest(const sp<RfxMclMessage>& msg);

    virtual void onHandleTimer();

  private:
    void modifyImsConf(const sp<RfxMclMessage>& msg, int request);
    void controlCall(const sp<RfxMclMessage>& msg, int request);
    void dialWithSipUri(const sp<RfxMclMessage>& msg, bool isVt = false);
    void confDial(const sp<RfxMclMessage>& msg);
    void acceptVideoCall(const sp<RfxMclMessage>& msg);
    void approveEccRedial(const sp<RfxMclMessage>& msg);
    void pullCall(const sp<RfxMclMessage>& msg);
    void explicitCallTransfer(const sp<RfxMclMessage>& msg);
    void controlCallAsync(const sp<RfxMclMessage>& msg, int request);
    void setRttMode(const sp<RfxMclMessage>& msg);
    void setSipHeader(const sp<RfxMclMessage>& msg);
    void enableSipHeaderReport(const sp<RfxMclMessage>& msg);
    void setImsCallMode(const sp<RfxMclMessage>& msg);
    void setImsCallAdditionalInfo(const sp<RfxMclMessage>& msg);
};

#endif
