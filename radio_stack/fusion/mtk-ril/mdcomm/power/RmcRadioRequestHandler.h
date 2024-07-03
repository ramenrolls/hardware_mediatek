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

#ifndef __RMC_RADIO_REQUEST_HANDLER_H__
#define __RMC_RADIO_REQUEST_HANDLER_H__

#include "RfxBaseHandler.h"

class RmcRadioRequestHandler : public RfxBaseHandler {
    RFX_DECLARE_HANDLER_CLASS(RmcRadioRequestHandler);

  public:
    RmcRadioRequestHandler(int slotId, int channelId);
    virtual ~RmcRadioRequestHandler();

  protected:
    virtual void onHandleRequest(const sp<RfxMclMessage>& msg);

    virtual void onHandleTimer();

    virtual void onHandleEvent(const sp<RfxMclMessage>& msg);

  private:
    void requestRadioPower(const sp<RfxMclMessage>& msg);

    int isModemRadioOn();

    void enableMdProtocol();

    void enableSilentReboot();

    void resetIpoProperty();

    void updateSupportDSBP();

    void updateDataCallPrefer();

    // External SIM [Start]
    int queryModemVsimCapability();
    void requestSwitchExternalSim();
    void initVsimConfiguration();
    void requestSetAkaSim();
    // External SIM [End]

    // Update RAT Before Radio on [Start]
    void sendERAT();
    // Update RAT Before Radio on[End]

    // SIM switch [Start]
    int queryMainProtocol();
    void resetRadio();
    // SIM switch [End]

    void bootupSetRadioPower(const sp<RfxMclMessage>& msg);
    int isNormalBootUp();

    bool isUnderCryptKeeper();

    void initMdStatusReader();
    static void getResetLock();
    static void releaseResetLock();
    static void* mdStatusReaderLoop(void* params);
    static void sendUrcToTelCor(char* buffer);

  private:
    pthread_t mMdStatsuReaderThread;
};
#endif
