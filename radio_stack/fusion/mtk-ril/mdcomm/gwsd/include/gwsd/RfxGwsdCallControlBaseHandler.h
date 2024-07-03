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

#ifndef __RFX_GWSD_CALL_CONTROL_BASE_HANDLER_H__
#define __RFX_GWSD_CALL_CONTROL_BASE_HANDLER_H__

/*****************************************************************************
 * Include
 *****************************************************************************/

#include "RfxBaseHandler.h"
#include "RfxController.h"

/*****************************************************************************
 * Class RfxGwsdCallControlBaseHandler
 *****************************************************************************/

class RfxGwsdCallControlBaseHandler {
  public:
    RfxGwsdCallControlBaseHandler() {}
    virtual ~RfxGwsdCallControlBaseHandler() {}

    virtual void registerForGwsdRequest(RfxBaseHandler* handler);
    virtual void registerForGwsdEvent(RfxBaseHandler* handler);
    virtual void registerForGwsdUrc(RfxBaseHandler* handler);
    virtual bool handleGwsdRequest(RfxBaseHandler* handler, const sp<RfxMclMessage>& msg);
    virtual bool handleGwsdEvent(RfxBaseHandler* handler, const sp<RfxMclMessage>& msg);
    virtual void handleGwsdUrc(RfxBaseHandler* handler, const sp<RfxMclMessage>& msg, int slotId);

    virtual bool fakeGetCurrentCalls(RfxBaseHandler* handler, const sp<RfxMclMessage>& msg,
                                     int speechCodec);
    virtual void requestFakeAccept(RfxBaseHandler* handler, const sp<RfxMclMessage>& msg);
    virtual void requestFakeReject(RfxBaseHandler* handler, const sp<RfxMclMessage>& msg);

    virtual void handleIcpgIndicationMessage(RfxBaseHandler* handler, const sp<RfxMclMessage>& msg,
                                             int slotId);
    virtual void handleIcpgResultMessage(RfxBaseHandler* handler, const sp<RfxMclMessage>& msg,
                                         int slotId);

    virtual bool onHandleTelCoreRequest(RfxController* controller, const sp<RfxMessage>& message);
    virtual bool onHandleTelCoreUrc(RfxController* controller, const sp<RfxMessage>& message);
    virtual bool onHandleTelCoreResponse(RfxController* controller, const sp<RfxMessage>& message);
    virtual void onRadioOff(RfxController* controller);
    virtual RIL_CALL_INFO_TYPE handleImsIncomingCall(RfxController* controller);
    virtual void notifyGwsdImsIncomingCall(RfxController* controller, int callId);

    virtual void updateRQ(RfxController* controller, int slotId, bool RQ);
    virtual void onAutoFakeAcceptTimer(RfxController* controller);
};

typedef RfxGwsdCallControlBaseHandler* createCallControlHandler_t();
typedef void destroyCallControlHandler_t(RfxGwsdCallControlBaseHandler*);

#endif /* __RFX_GWSD_CALL_CONTROL_BASE_HANDLER_H__ */
