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

#ifndef __RMC_WP_URC_HANDLER_H__
#define __RMC_WP_URC_HANDLER_H__

#include "RfxBaseHandler.h"
#include "RfxIntsData.h"
#include "RfxVoidData.h"
#include "RfxMessageId.h"

class RmcWpURCHandler : public RfxBaseHandler {
    RFX_DECLARE_HANDLER_CLASS(RmcWpURCHandler);

  public:
    RmcWpURCHandler(int slot_id, int channel_id);
    virtual ~RmcWpURCHandler();
    static int getSlotId();
    static int getEcsraUrcParam(int index);

  protected:
    virtual void onHandleUrc(const sp<RfxMclMessage>& msg);
    virtual bool onCheckIfRejectMessage(const sp<RfxMclMessage>& msg, RIL_RadioState radioState);

  private:
    void handleWorldModeUrc(const sp<RfxMclMessage>& msg);

  private:
    static int ecsraUrcParams[5];
    static int mSlotId;
};

#endif /*__RMC_WP_URC_HANDLER_H__*/
