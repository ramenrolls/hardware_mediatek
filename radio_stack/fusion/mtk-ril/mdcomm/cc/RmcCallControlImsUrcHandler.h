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

#ifndef __RMC_CALL_CONTROL_IMS_URC_HANDLER_H__
#define __RMC_CALL_CONTROL_IMS_URC_HANDLER_H__

#include "RmcCallControlBaseHandler.h"

class RmcCallControlImsUrcHandler : public RmcCallControlBaseHandler {
    RFX_DECLARE_HANDLER_CLASS(RmcCallControlImsUrcHandler);

  public:
    RmcCallControlImsUrcHandler(int slot_id, int channel_id);
    virtual ~RmcCallControlImsUrcHandler();

  protected:
    virtual void onHandleUrc(const sp<RfxMclMessage>& msg);

    virtual void onHandleTimer();

  private:
    void handleConfSrvcc(const sp<RfxMclMessage>& msg);
    void handleSipMsgIndication(const sp<RfxMclMessage>& msg);
    void handleConfModifiedResult(const sp<RfxMclMessage>& msg);
    // void handleCallStateControlResult(const sp<RfxMclMessage>& msg);
    void handleVoiceDomainSelectResult(const sp<RfxMclMessage>& msg);
    void handleCallModeChanged(const sp<RfxMclMessage>& msg);
    void handleECT(const sp<RfxMclMessage>& msg);
    void handleVideoCapabilityChanged(const sp<RfxMclMessage>& msg);
    void handleEventPackage(const sp<RfxMclMessage>& msg);
    void handleSrvccStateChange(const sp<RfxMclMessage>& msg);
    void handleImsCallControlResult(const sp<RfxMclMessage>& msg);
    void handleEmergencyBearerSupportInfo(const sp<RfxMclMessage>& msg);
    void handleRedialEmergencyIndication(const sp<RfxMclMessage>& msg);
    void handleImsHeaderInfo(const sp<RfxMclMessage>& msg);
    void handleCallAdditionalInfo(const sp<RfxMclMessage>& msg);

    void notifySrvccState(int state);
    void notifyUnsolWithInt(int unsol, int data);
    void notifyUnsolWithInts(int unsol, int dataLen, int data[]);
    void notifyUnsolWithStrings(int unsol, char* str);
};
#endif
