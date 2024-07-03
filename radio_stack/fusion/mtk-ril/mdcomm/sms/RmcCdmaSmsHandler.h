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

#ifndef __RMC_CDMA_SMS_HANDLER__
#define __RMC_CDMA_SMS_HANDLER__
/*****************************************************************************
 * Include
 *****************************************************************************/
#include "common/RmcChannelHandler.h"

/*****************************************************************************
 * Class RmcCdmaSmsReqHandler
 *****************************************************************************/
class RmcCdmaSmsReqHandler : public RmcBaseHandler {
    RFX_DECLARE_HANDLER_CLASS(RmcCdmaSmsReqHandler);

    // Constructor / Destructor
  public:
    RmcCdmaSmsReqHandler(int slotId, int channelId);
    virtual ~RmcCdmaSmsReqHandler();

    // Override
  protected:
    virtual void onHandleEvent(const sp<RfxMclMessage>& msg);
    virtual RmcMessageHandler* onCreateReqHandler(const sp<RfxMclMessage>& msg);
};

/*****************************************************************************
 * Class RmcCdmaMoSmsReqHandler
 *****************************************************************************/
class RmcCdmaMoSmsReqHandler : public RmcBaseHandler {
    RFX_DECLARE_HANDLER_CLASS(RmcCdmaMoSmsReqHandler);

    // Constructor / Destructor
  public:
    RmcCdmaMoSmsReqHandler(int slotId, int channelId);
    virtual ~RmcCdmaMoSmsReqHandler();

    // Override
  protected:
    virtual void onHandleEvent(const sp<RfxMclMessage>& msg);
    virtual RmcMessageHandler* onCreateReqHandler(const sp<RfxMclMessage>& msg);
};

/*****************************************************************************
 * Class RmcCdmaMtSmsAckReqHandler
 *****************************************************************************/
class RmcCdmaMtSmsAckReqHandler : public RmcBaseHandler {
    RFX_DECLARE_HANDLER_CLASS(RmcCdmaMtSmsAckReqHandler);

    // Constructor / Destructor
  public:
    RmcCdmaMtSmsAckReqHandler(int slotId, int channelId);
    virtual ~RmcCdmaMtSmsAckReqHandler();

    // Override
  protected:
    virtual void onHandleEvent(const sp<RfxMclMessage>& msg);
    virtual RmcMessageHandler* onCreateReqHandler(const sp<RfxMclMessage>& msg);
};

/*****************************************************************************
 * Class RmcCdmaSmsUrcHandler
 *****************************************************************************/
class RmcCdmaSmsUrcHandler : public RmcBaseHandler {
    RFX_DECLARE_HANDLER_CLASS(RmcCdmaSmsUrcHandler);

    // Constructor / Destructor
  public:
    RmcCdmaSmsUrcHandler(int slotId, int channelId);
    virtual ~RmcCdmaSmsUrcHandler();

    // Override
  protected:
    virtual void onHandleEvent(const sp<RfxMclMessage>& msg);
    virtual RmcMessageHandler* onCreateUrcHandler(const sp<RfxMclMessage>& msg);
};
#endif /* __RMC_CDMA_SMS_HANDLER__ */
