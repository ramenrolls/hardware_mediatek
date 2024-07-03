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

#ifndef __RMC_COMM_SIM_OP_URC_HANDLER_H__
#define __RMC_COMM_SIM_OP_URC_HANDLER_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "RmcSimBaseHandler.h"
#include "RmcCommSimUrcHandler.h"

/*****************************************************************************
 * Class RpSimController
 *****************************************************************************/

class RmcCommSimOpUrcHandler : public RmcCommSimUrcHandler {
    RFX_DECLARE_HANDLER_CLASS(RmcCommSimOpUrcHandler);

  public:
    RmcCommSimOpUrcHandler(int slot_id, int channel_id);
    virtual ~RmcCommSimOpUrcHandler();

    // Process URC here
    virtual void handleUrc(const sp<RfxMclMessage>& msg, RfxAtLine* urc);

    // Check if the handler have to process the URC or not
    virtual RmcSimBaseHandler::SIM_HANDLE_RESULT needHandle(const sp<RfxMclMessage>& msg);

    virtual const char** queryUrcTable(int* record_num);

  private:
    void handleAttRsuSimLockEvent(const sp<RfxMclMessage>& msg, String8 urc);
    void handleRsuEvent(const sp<RfxMclMessage>& msg, RfxAtLine* urc);
};
#endif /* __RMC_COMM_SIM_OP_URC_HANDLER_H__ */
