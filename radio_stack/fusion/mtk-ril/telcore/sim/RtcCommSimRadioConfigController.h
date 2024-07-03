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

#ifndef __RFX_RC_CONTROLLER_H__
#define __RFX_RC_CONTROLLER_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "RfxController.h"
#include "RfxTimer.h"
#include "RfxVoidData.h"
#include "RfxStringData.h"
#include "RfxStringsData.h"
#include "RfxIntsData.h"
#include "RfxMessageId.h"
#include "RfxTimer.h"

/*****************************************************************************
 * Class RfxController
 *****************************************************************************/
class RtcCommSimRadioConfigController : public RfxController {
    // Required: declare this class
    RFX_DECLARE_CLASS(RtcCommSimRadioConfigController);

  public:
    RtcCommSimRadioConfigController();
    virtual ~RtcCommSimRadioConfigController();

    // Override
  protected:
    virtual void onInit();
    virtual void onDeinit();
    virtual bool onHandleRequest(const sp<RfxMessage>& message);
    virtual bool onHandleUrc(const sp<RfxMessage>& message);
    virtual bool onHandleResponse(const sp<RfxMessage>& message);
    virtual bool onCheckIfRejectMessage(const sp<RfxMessage>& message, bool isModemPowerOff,
                                        int radioState);
    virtual bool onPreviewMessage(const sp<RfxMessage>& message);
    virtual bool onCheckIfResumeMessage(const sp<RfxMessage>& message);

  public:
  private:
    RIL_SimSlotStatus** pp_slotStatus;
    // bool need_send;
    int sending_count;
};

#endif /* __RFX_RC_CONTROLLER_H__ */
