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

#ifndef __RMC_CALL_CONTROL_CHLD_REQUEST_HANDLER_H__
#define __RMC_CALL_CONTROL_CHLD_REQUEST_HANDLER_H__

#include "RfxBaseHandler.h"
#include "RfxStringData.h"
#include "RfxVoidData.h"
#include "RfxIntsData.h"

#include "RmcCallControlBaseHandler.h"

class RmcCallControlChldRequestHandler : public RmcCallControlBaseHandler {
    RFX_DECLARE_HANDLER_CLASS(RmcCallControlChldRequestHandler);

  public:
    RmcCallControlChldRequestHandler(int slot_id, int channel_id);
    virtual ~RmcCallControlChldRequestHandler();

  protected:
    virtual void onHandleRequest(const sp<RfxMclMessage>& msg);

    virtual void onHandleTimer();

  private:
    void requestHangupAll(const sp<RfxMclMessage>& msg);
    void requestHangup(const sp<RfxMclMessage>& msg);
    void requestHangupWaitingOrBackground(const sp<RfxMclMessage>& msg);
    void requestHangupForegroundResumeBackground(const sp<RfxMclMessage>& msg);
    void requestSwitchWaitingOrHoldingAndActive(const sp<RfxMclMessage>& msg);
    void requestConference(const sp<RfxMclMessage>& msg);
    void requestExplicitCallTransfer(const sp<RfxMclMessage>& msg);
    void requestSeparateConnection(const sp<RfxMclMessage>& msg);
    void requestHangupWithReason(const sp<RfxMclMessage>& msg);
};

#endif
