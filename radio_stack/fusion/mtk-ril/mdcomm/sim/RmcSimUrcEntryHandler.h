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

#ifndef __RMC_SIM_URC_ENTRY_HANDLER_H__
#define __RMC_SIM_URC_ENTRY_HANDLER_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "RfxBaseHandler.h"
#include "RmcSimBaseHandler.h"
#include "RmcGsmSimUrcHandler.h"
#include "RmcCdmaSimUrcHandler.h"
#include "RmcCommSimUrcHandler.h"
#include "RmcCommSimDefs.h"

class RmcVsimUrcHandler : public RmcSimBaseHandler {
    RFX_DECLARE_HANDLER_CLASS(RmcVsimUrcHandler);

  public:
    RmcVsimUrcHandler(int slot_id, int channel_id);
    virtual ~RmcVsimUrcHandler();
};

/*****************************************************************************
 * Class RpSimController
 *****************************************************************************/

class RmcSimUrcEntryHandler : public RmcSimBaseHandler {
    RFX_DECLARE_HANDLER_CLASS(RmcSimUrcEntryHandler);

  public:
    RmcSimUrcEntryHandler(int slot_id, int channel_id);
    virtual ~RmcSimUrcEntryHandler();

    // Override
  protected:
    // Process URC here
    virtual void onHandleUrc(const sp<RfxMclMessage>& msg);

  private:
    RmcGsmSimUrcHandler* mGsmUrcHandler;
    RmcCdmaSimUrcHandler* mCdmaUrcHandler;
    RmcCommSimUrcHandler* mCommUrcHandler;
    // External SIM [Start]
    RmcVsimUrcHandler* mVsimUrcHandler;
    // External SIM [End]
};
#endif /* __RMC_SIM_URC_ENTRY_HANDLER_H__ */
