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

#ifndef __RMC_COMM_SIM_OP_REQUEST_HANDLER_H__
#define __RMC_COMM_SIM_OP_REQUEST_HANDLER_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "RfxOpRsuRequestData.h"
#include "RfxOpRsuResponseData.h"
#include "RfxIntsData.h"
#include "RfxStringsData.h"
#include "RfxVoidData.h"
#include "RmcCommSimRequestHandler.h"
#include "RmcCommSimOpRequestHandler.h"
#include "RmcCommSimDefs.h"
#include "RmcSimBaseHandler.h"
#ifdef __cplusplus
extern "C" {
#endif
#ifdef __cplusplus
}
#endif

/*****************************************************************************
 * Class RpSimController
 *****************************************************************************/

class RmcCommSimOpRequestHandler : public RmcCommSimRequestHandler {
    RFX_DECLARE_HANDLER_CLASS(RmcCommSimOpRequestHandler);

  public:
    RmcCommSimOpRequestHandler(int slot_id, int channel_id);
    virtual ~RmcCommSimOpRequestHandler();

    // Check if the handler have to process the Request or not
    virtual RmcSimBaseHandler::SIM_HANDLE_RESULT needHandle(const sp<RfxMclMessage>& msg);
    // Process Request here
    virtual void handleRequest(const sp<RfxMclMessage>& msg);
    // Return the list which you want to reqister
    virtual const int* queryTable(int channel_id, int* record_num);

  private:
    // Request handler
    void handleAttGetSharedKey(const sp<RfxMclMessage>& msg);
    void handleAttUpdateSimLockSettings(const sp<RfxMclMessage>& msg);
    void handleAttGetSimLockVersion(const sp<RfxMclMessage>& msg);
    void handleAttResetSimLockSettings(const sp<RfxMclMessage>& msg);
    void handleAttGetModemStatus(const sp<RfxMclMessage>& msg);
    void handleAttGetModemConfig(const sp<RfxMclMessage>& msg);
    void handleAttGetApiVersion(const sp<RfxMclMessage>& msg);
    void handleAttGetSimSlots(const sp<RfxMclMessage>& msg);
    void handleRsuRequest(const sp<RfxMclMessage>& msg);
    void handleVzwRsuRequest(const sp<RfxMclMessage>& msg);
    void handleVzwRsuInitiate(const sp<RfxMclMessage>& msg);
    void handleVzwRsuGetSharedKey(const sp<RfxMclMessage>& msg);
    void handleVzwRsuUpdateLockData(const sp<RfxMclMessage>& msg);
    void handleVzwRsuGetVersion(const sp<RfxMclMessage>& msg);
    void handleVzwRsuGetStatus(const sp<RfxMclMessage>& msg);
    void handleVzwRsuUnlockTimer(const sp<RfxMclMessage>& msg);
    void handleKDDIRsuRequest(const sp<RfxMclMessage>& msg);
    void handleKDDIRsuSetSimLockData(const sp<RfxMclMessage>& msg);
    void handleKDDIRsuGetLockData(const sp<RfxMclMessage>& msg, int requestId);
};
#endif /* __RMC_COMM_SIM_OP_REQUEST_HANDLER_H__ */
