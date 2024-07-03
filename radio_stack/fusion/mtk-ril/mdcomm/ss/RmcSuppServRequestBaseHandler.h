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

#ifndef __RMC_SUPP_SERV_REQUEST_BASE_HANDLER_H__
#define __RMC_SUPP_SERV_REQUEST_BASE_HANDLER_H__

#include "RfxBaseHandler.h"
#include "RfxStringData.h"
#include "RfxVoidData.h"
#include "RfxIntsData.h"
#include "RfxStringsData.h"
#include "SuppServDef.h"
#include "RfxCallForwardInfoData.h"
#include "RfxCallForwardInfosData.h"
#include "RfxCallForwardInfoExData.h"
#include "RfxCallForwardInfosExData.h"

#ifdef TAG
#undef TAG
#endif
#define TAG "RmcSSBaseHandler"

class RmcSuppServRequestBaseHandler : public RfxBaseHandler {
  public:
    RmcSuppServRequestBaseHandler(int slot_id, int channel_id);
    virtual ~RmcSuppServRequestBaseHandler();

  protected:
    void requestClirOperation(const sp<RfxMclMessage>& msg);
    void requestCallForwardOperation(const sp<RfxMclMessage>& msg, CallForwardOperationE op);
    void requestCallForwardExOperation(const sp<RfxMclMessage>& msg, CallForwardOperationE op);
    void requestCallWaitingOperation(const sp<RfxMclMessage>& msg, CallWaitingOperationE op);
    bool checkTerminalBaseCallWaitingStatus(char* tbCWStatus, int* responses, const int* inputData,
                                            CallWaitingOperationE op);
    int checkTerminalBaseCallWaitingType(sp<RfxAtResponse> p_response);
    void handleTerminalBaseCallWaitingResponse(const char* tbCWStatus, int* responses,
                                               int sendBsCode, const int* inputData,
                                               int& responseForAll, sp<RfxAtResponse> p_response,
                                               CallWaitingOperationE op);
    void requestColpOperation(const sp<RfxMclMessage>& msg);
    void requestColrOperation(const sp<RfxMclMessage>& msg);
    void requestCallBarring(const sp<RfxMclMessage>& msg, CallBarringOperationE op);
    void sleepMsec(long long msec);
    char* parseErrorMessageFromXCAP(sp<RfxAtResponse> p_response);
    void handleSetClirResponse(int clir_n);
    void handleGetClirResponse(int (&responses)[2], bool isTerminalBasedSolution);
    void resetClirProperty();
    void requestResetSuppServ(const sp<RfxMclMessage>& msg);
    void handleErrorMessageFromXcap(sp<RfxAtResponse> p_response, AT_CME_Error errorCode);
};

#endif
