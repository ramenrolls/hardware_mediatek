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

#ifndef __RMC_VT_REQ_HANDLER_H__
#define __RMC_VT_REQ_HANDLER_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "RfxBaseHandler.h"
#include "RmcVtDataThreadController.h"

class RmcVtReqHandler : public RfxBaseHandler {
    RFX_DECLARE_HANDLER_CLASS(RmcVtReqHandler);

  public:
    RmcVtReqHandler(int slot_id, int channel_id);
    virtual ~RmcVtReqHandler();
    static bool isImsVideoCallon(void);
    static bool isVTLogEnable(void);

  private:
    void handleRequestGetInfo(const sp<RfxMclMessage>& msg);
    void handleRequestUpdateOpid(const sp<RfxMclMessage>& msg);
    void handleEventCallStatusUpdate(const sp<RfxMclMessage>& msg);
    void handleEventVtSendMsg(const sp<RfxMclMessage>& msg);
    void handleEventVtReceiveMsg(const sp<RfxMclMessage>& msg);
    void sendMsgToVTS(char* outBuffer, int length, const char* user);

    sp<RmcVtDataThreadController> mThdController;
    int mPendingCapReqBySim[MAX_SIM_COUNT] = {0};

  protected:
    virtual void onHandleRequest(const sp<RfxMclMessage>& msg);
    virtual void onHandleEvent(const sp<RfxMclMessage>& msg);
};

#endif /* __RMC_VT_REQ_HANDLER_H__ */
