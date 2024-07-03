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

#ifndef __RMC_OEM_REQUEST_HANDLER_H__
#define __RMC_OEM_REQUEST_HANDLER_H__

#include "RfxBaseHandler.h"
#include <string>

using namespace std;

typedef struct {
    char* imei;
    char* imeisv;
    char* esnHex;
    char* meidHex;
    char* uimid;
    char* esnDec;
    char* meidDec;
} RIL_IDENTITY;

/*typedef struct {
    int custId;
    char* key;
} AuthContext;*/

class RmcOemRequestHandler : public RfxBaseHandler {
    RFX_DECLARE_HANDLER_CLASS(RmcOemRequestHandler);

  public:
    RmcOemRequestHandler(int slot_id, int channel_id);
    virtual ~RmcOemRequestHandler();

  protected:
    virtual void onHandleRequest(const sp<RfxMclMessage>& msg);

    virtual void onHandleTimer();

    virtual void onHandleEvent(const sp<RfxMclMessage>& msg);

  private:
    void requestDeviceIdentity(const sp<RfxMclMessage>& msg);

    void requestOemHookRaw(const sp<RfxMclMessage>& msg);

    void requestOemHookStrings(const sp<RfxMclMessage>& msg);

    void requestGetImei();

    void requestGetImei(const sp<RfxMclMessage>& msg);

    void requestGetImeisv();

    void requestGetImeisv(const sp<RfxMclMessage>& msg);

    void requestGetActivityInfo(const sp<RfxMclMessage>& msg);

    void requestBasebandVersion(const sp<RfxMclMessage>& msg);

    void requestQueryThermal(const sp<RfxMclMessage>& msg);

    void requestSetTrm(const sp<RfxMclMessage>& msg);

    void requestGetGcfMode();

    void bootupGetBasebandProject();

    void requestMdVersion();

    void requestSN();

    void requestSetTxPowerStatus(const sp<RfxMclMessage>& msg);

    void requestGetPhoneCapability(const sp<RfxMclMessage>& msg);

    void requestEnableDsdaIndication(const sp<RfxMclMessage>& msg);

    void requestGetDsdaStatus(const sp<RfxMclMessage>& msg);

    void routeCert(const sp<RfxMclMessage>& msg);

    void routeAuth(const sp<RfxMclMessage>& msg);

    void enableCapability(const sp<RfxMclMessage>& msg);

    void abortCert(const sp<RfxMclMessage>& msg);

    void queryCapability(const sp<RfxMclMessage>& msg);

    char* getCertPhase1Command(const char* cert);
    char* getCertPhase2Command(int custId, const char* msg);
    char* getAuthPhaseCommand(const char* msg);
    char* getCapabilityCommand(int uid, char* featureName, int timer);
    char* getQueryCapabilityCommand(int uid, char* featureName);
    char* getAbortCommand(int uid);
    char* getAuthPrefixCommand();
    char* getQueryPrefixCommand();
    void updateContext(int id, int custId, char* key);
    void removeContext(int id);
    void byteToString(char* byte, int size, char** output);
    void stringToByte(char* string, int size, char** output);
    void updateRnd(char* rnd);
    void requestSendSarIndicator(const sp<RfxMclMessage>& msg);

  private:
    String8 mImei;
    String8 mImeiSv;
    static void* sDlHandler;
};

#endif
