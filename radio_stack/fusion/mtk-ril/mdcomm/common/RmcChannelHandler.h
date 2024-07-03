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

#ifndef __RMC_CHANNEL_HANDLER_H__
#define __RMC_CHANNEL_HANDLER_H__
/*****************************************************************************
 * Include
 *****************************************************************************/
#include "RfxBaseHandler.h"
#include "RmcMessageHandler.h"

/*****************************************************************************
 * Class RmcBaseHandler
 *****************************************************************************/
class RmcBaseHandler : public RfxBaseHandler {
    // Constructor / Destructor
  public:
    RmcBaseHandler(int slotId, int channelId) : RfxBaseHandler(slotId, channelId) {}

    virtual ~RmcBaseHandler() {}

    // Override
  protected:
    virtual void onHandleRequest(const sp<RfxMclMessage>& msg) {
        RmcMessageHandler* handler = onCreateReqHandler(msg);
        handleMessage(handler, msg);
    }

    virtual void onHandleUrc(const sp<RfxMclMessage>& msg) {
        RmcMessageHandler* handler = onCreateUrcHandler(msg);
        handleMessage(handler, msg);
    }

    // Overridable
  protected:
    // Create a RmcMessageHandler for a request from heap
    //
    // RETURNS: the message handler object
    virtual RmcMessageHandler* onCreateReqHandler(
            const sp<RfxMclMessage>& msg  // [IN] RfxMclMessage for the request
    ) {
        RFX_UNUSED(msg);
        return NULL;
    }

    // Create a RmcMessageHandler for a URC from heap
    //
    // RETURNS: the message handler object
    virtual RmcMessageHandler* onCreateUrcHandler(
            const sp<RfxMclMessage>& msg  // [IN] RfxMclMessage for the urc
    ) {
        RFX_UNUSED(msg);
        return NULL;
    }

    // Implementation
  private:
    void destroyMessageHandler(RmcMessageHandler* h);
    void handleMessage(RmcMessageHandler* h, const sp<RfxMclMessage>& msg);
};
#endif /* __RMC_CHANNEL_HANDLER_H__ */
