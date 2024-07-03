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

#ifndef __RMC_CALL_CONTROL_COMMON_REQUEST_HANDLER_H__
#define __RMC_CALL_CONTROL_COMMON_REQUEST_HANDLER_H__

#include "RmcCallControlBaseHandler.h"

class RmcCallControlImsRequestHandler;

class RmcCallControlCommonRequestHandler : public RmcCallControlBaseHandler {
    RFX_DECLARE_HANDLER_CLASS(RmcCallControlCommonRequestHandler);

  public:
    RmcCallControlCommonRequestHandler(int slot_id, int channel_id);
    virtual ~RmcCallControlCommonRequestHandler();

  protected:
    virtual void onHandleRequest(const sp<RfxMclMessage>& msg);
    virtual void onHandleEvent(const sp<RfxMclMessage>& msg);

    virtual void onHandleTimer();

    void requestGetCurrentCalls(const sp<RfxMclMessage>& msg);
    void requestDial(const sp<RfxMclMessage>& msg, bool isEcc, bool isVt, bool isImsDial);

    void copyString(char* destStr, char* srcStr, int bufSize);

    int clccsStateToRILState(int state, RIL_CallState* p_state);
    int clccStateToRILState(int state, RIL_CallState* p_state);
    void clearCnap();

    char cachedCnap[MAX_CNAP_LENGTH] = {0};
    char cachedClccName[MAX_GSMCALL_CONNECTIONS][MAX_CNAP_LENGTH] = {{0}};

  private:
    void requestAnswer(const sp<RfxMclMessage>& msg);
    void requestUdub(const sp<RfxMclMessage>& msg);
    void requestDtmf(const sp<RfxMclMessage>& msg);
    void requestDtmfStart(const sp<RfxMclMessage>& msg);
    void requestDtmfStop(const sp<RfxMclMessage>& msg);
    void requestSetMute(const sp<RfxMclMessage>& msg);
    void requestGetMute(const sp<RfxMclMessage>& msg);
    void requestSetTtyMode(const sp<RfxMclMessage>& msg);
    void requestQueryTtyMode(const sp<RfxMclMessage>& msg);
    void requestLastCallFailCause(const sp<RfxMclMessage>& msg);
    void requestSetCallIndication(const sp<RfxMclMessage>& msg);
    void requestLocalSetEccServiceCategory(const sp<RfxMclMessage>& msg);
    int callFromCLCCSLine(RfxAtLine* line, RIL_Call* p_call);
    int callFromCLCCLine(RfxAtLine* line, RIL_Call* p_call);
    void requestExitEmergencyCallbackMode(const sp<RfxMclMessage>& msg);
    void handleCnapUpdate(const sp<RfxMclMessage>& msg);
    void handleClearClccName(const sp<RfxMclMessage>& msg);

    /// C2K specific start
    void requestSetPreferredVoicePrivacyMode(const sp<RfxMclMessage>& msg);
    void requestQueryPreferredVoicePrivacyMode(const sp<RfxMclMessage>& msg);
    void requestFlash(const sp<RfxMclMessage>& msg);
    void requestBurstDtmf(const sp<RfxMclMessage>& msg);
    /// C2K specific end

    /// redial @{
    void requestEmergencyRedial(const sp<RfxMclMessage>& msg);
    void requestNotifyEmergencySession(const sp<RfxMclMessage>& msg, bool isStarted);
    /// @}

    void requestEccPreferredRat(const sp<RfxMclMessage>& msg);
    void requestLocalCurrentStatus(const sp<RfxMclMessage>& msg);

    bool isNumberIncludePause(char* number);
    bool isValidDialString(const char* dialString);

    char atLog[MAX_AT_RESPONSE] = {0};

    int bUseLocalCallFailCause = 0;
    int dialLastError = 0;
    RmcCallControlImsRequestHandler* mImsCCReqHdlr = NULL;

    void queryEmciSupport();
};

#endif
