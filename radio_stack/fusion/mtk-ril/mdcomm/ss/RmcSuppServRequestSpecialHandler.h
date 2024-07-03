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

#ifndef __RMC_SUPP_SERV_REQUEST_SPECIAL_HANDLER_H__
#define __RMC_SUPP_SERV_REQUEST_SPECIAL_HANDLER_H__

#include "RmcSuppServRequestBaseHandler.h"
#include "SuppServDef.h"

#ifdef TAG
#undef TAG
#endif
#define TAG "RmcSSHandlerSp"

class RmcSuppServRequestSpecialHandler : public RmcSuppServRequestBaseHandler {
    RFX_DECLARE_HANDLER_CLASS(RmcSuppServRequestSpecialHandler);

  public:
    RmcSuppServRequestSpecialHandler(int slot_id, int channel_id);
    virtual ~RmcSuppServRequestSpecialHandler();

  protected:
    virtual void onHandleRequest(const sp<RfxMclMessage>& msg);
    virtual void onHandleTimer();

  private:
    void requestQueryCallForwardStatus(const sp<RfxMclMessage>& msg);
    void requestQueryCallForwardInTimeSlotStatus(const sp<RfxMclMessage>& msg);
    void requestGetClir(const sp<RfxMclMessage>& msg);
    void requestQueryCallWaiting(const sp<RfxMclMessage>& msg);
    void requestQueryCallBarring(const sp<RfxMclMessage>& msg);
    void requestSetCallBarring(const sp<RfxMclMessage>& msg);
};

#endif
