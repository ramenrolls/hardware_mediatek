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

#ifndef __RMC_MESSAGE_HANDLER__
#define __RMC_MESSAGE_HANDLER__
/*****************************************************************************
 * Include
 *****************************************************************************/
#include "RfxBaseHandler.h"
#include "RmcAtInfo.h"
#include "RmcData.h"

/*****************************************************************************
 * Class RmcMessageHandler
 *****************************************************************************/
class RmcMessageHandler {
    // Constructor / Destructor
  public:
    RmcMessageHandler(RfxBaseHandler* handler  // [IN] the pointer of RfxBaseHandler
                      )
        : m_handler(handler) {}

    virtual ~RmcMessageHandler() {}

    // Overridable
  public:
    virtual void onHandleMessage(const sp<RfxMclMessage>& msg) = 0;

    // Method for sub class
  protected:
    RfxBaseHandler* getHandler() { return m_handler; }

    // Implementation
  private:
    RfxBaseHandler* m_handler;
};

/*****************************************************************************
 * Class RmcBaseRequestHandler
 *****************************************************************************/
class RmcBaseRequestHandler : public RmcMessageHandler {
    // Constructor / Destructor
  public:
    RmcBaseRequestHandler(RfxBaseHandler* handler) : RmcMessageHandler(handler) {}

    virtual ~RmcBaseRequestHandler();

    // Override
  public:
    virtual void onHandleMessage(const sp<RfxMclMessage>& msg);

    // Overridable
  protected:
    // Callback when get the Response Data Object
    //
    // RETURNS: the response data object
    virtual RmcBaseRspData* onGetRspData(RmcBaseReqData* data  // [IN] The request data object
    );

    // Callback after call response to telcore
    //
    // RETURNS: void
    virtual void onAfterResponse() {}

    // Implementation
  private:
    void sendAtCmds(RmcBaseReqData* data);
    void releaseRspData(RmcBaseRspData* data);
    RmcBaseRspData* getRspData(RmcBaseReqData* data);

    // Implementation
  private:
    Vector<RmcAtRspInfo*> m_atRspInfos;
};

/*****************************************************************************
 * Class RmcBaseUrcHandler
 *****************************************************************************/
class RmcBaseUrcHandler : public RmcMessageHandler {
    // Constructor / Destructor
  public:
    RmcBaseUrcHandler(RfxBaseHandler* h) : RmcMessageHandler(h) {}
    virtual ~RmcBaseUrcHandler() {}

    // Override
  protected:
    virtual void onHandleMessage(const sp<RfxMclMessage>& msg);

    // Overridable
  protected:
    virtual bool onHandleRawUrc(RfxAtLine* line) {
        RFX_UNUSED(line);
        return true;
    }

    virtual bool onHandleRawUrc2(RfxAtLine* line) {
        RFX_UNUSED(line);
        return true;
    }
    virtual RmcBaseUrspData* onGetUrcData(int slotId) {
        RFX_UNUSED(slotId);
        return NULL;
    }

    // Implementation
  private:
    void releaseUrspData(RmcBaseUrspData* data);
    RmcBaseUrspData* getUrspData(const sp<RfxMclMessage>& msg);
};

#endif /* __RMC_MESSAGE_HANDLER__ */
