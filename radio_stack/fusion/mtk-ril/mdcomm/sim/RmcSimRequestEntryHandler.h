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

#ifndef __RMC_SIM_REQUEST_ENTRY_HANDLER_H__
#define __RMC_SIM_REQUEST_ENTRY_HANDLER_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "RfxBaseHandler.h"
#include "RmcSimBaseHandler.h"
#include "RmcGsmSimRequestHandler.h"
#include "RmcCdmaSimRequestHandler.h"
#include "RmcCommSimRequestHandler.h"
#include "RmcCommSimDefs.h"

class RmcVsimRequestHandler : public RmcSimBaseHandler {
    RFX_DECLARE_HANDLER_CLASS(RmcVsimRequestHandler);

  public:
    RmcVsimRequestHandler(int slot_id, int channel_id);
    virtual ~RmcVsimRequestHandler();
};

/*****************************************************************************
 * Class RpSimController
 *****************************************************************************/

class RmcSimRequestEntryHandler : public RmcSimBaseHandler {
    RFX_DECLARE_HANDLER_CLASS(RmcSimRequestEntryHandler);

  public:
    RmcSimRequestEntryHandler(int slot_id, int channel_id);
    virtual ~RmcSimRequestEntryHandler();

    // Override
  protected:
    // Process Request here
    virtual void onHandleRequest(const sp<RfxMclMessage>& msg);
    // Process event here
    virtual void onHandleEvent(const sp<RfxMclMessage>& msg);

    RmcGsmSimRequestHandler* mGsmReqHandler;
    RmcCdmaSimRequestHandler* mCdmaReqHandler;
    RmcCommSimRequestHandler* mCommReqHandler;
    // External SIM [Start]
    RmcVsimRequestHandler* mVsimReqHandler;
    // External SIM [End]
};
#endif /* __RMC_SIM_REQUEST_ENTRY_HANDLER_H__ */
