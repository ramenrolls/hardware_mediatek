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

#ifndef __RMC_GSM_SIM_OP_URC_HANDLER_H__
#define __RMC_GSM_SIM_OP_URC_HANDLER_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "RmcSimBaseHandler.h"
#include "RmcGsmSimUrcHandler.h"

/*****************************************************************************
 * Class RpSimController
 *****************************************************************************/

class RmcGsmSimOpUrcHandler : public RmcGsmSimUrcHandler {
    RFX_DECLARE_HANDLER_CLASS(RmcGsmSimOpUrcHandler);

  public:
    RmcGsmSimOpUrcHandler(int slot_id, int channel_id);
    virtual ~RmcGsmSimOpUrcHandler();

    // Process URC here
    virtual void handleUrc(const sp<RfxMclMessage>& msg, RfxAtLine* urc);

    // Check if the handler have to process the URC or not
    virtual RmcSimBaseHandler::SIM_HANDLE_RESULT needHandle(const sp<RfxMclMessage>& msg);

    virtual const char** queryUrcTable(int* record_num);

  private:
    void handleRemoteSimUnlockEvent(const sp<RfxMclMessage>& msg, String8 urc);
};
#endif /* __RMC_GSM_SIM_URC_HANDLER_H__ */
