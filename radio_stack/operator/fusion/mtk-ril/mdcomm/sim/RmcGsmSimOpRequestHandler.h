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

#ifndef __RMC_GSM_SIM_OP_REQUEST_HANDLER_H__
#define __RMC_GSM_SIM_OP_REQUEST_HANDLER_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "RmcGsmSimRequestHandler.h"
#include "RmcCommSimDefs.h"

/*****************************************************************************
 * Class RmcGsmSimOpRequestHandler
 *****************************************************************************/

class RmcGsmSimOpRequestHandler : public RmcGsmSimRequestHandler {
    RFX_DECLARE_HANDLER_CLASS(RmcGsmSimOpRequestHandler);

  public:
    RmcGsmSimOpRequestHandler(int slot_id, int channel_id);
    virtual ~RmcGsmSimOpRequestHandler();

    // Check if the handler have to process the Request or not
    virtual RmcSimBaseHandler::SIM_HANDLE_RESULT needHandle(const sp<RfxMclMessage>& msg);

    // Process Request here
    virtual void handleRequest(const sp<RfxMclMessage>& msg);

    // Return the list which you want to reqister
    virtual const int* queryTable(int channel_id, int* record_num);

  private:
    // Request handler
    void handleGetSharedKey(const sp<RfxMclMessage>& msg);
    void handleUpdateSimLockSettings(const sp<RfxMclMessage>& msg);
    void handleGetSimLockInfo(const sp<RfxMclMessage>& msg);
    void handleResetSimLockSettings(const sp<RfxMclMessage>& msg);
    void handleGetModemStatus(const sp<RfxMclMessage>& msg);
};
#endif /* __RMC_GSM_SIM_OP_REQUEST_HANDLER_H__ */
