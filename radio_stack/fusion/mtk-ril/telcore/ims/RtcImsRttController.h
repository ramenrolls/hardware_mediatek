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

#ifndef __RFX_IMS_RTT_CONTROLLER_H__
#define __RFX_IMS_RTT_CONTROLLER_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "RfxController.h"
#include "RfxTimer.h"
#include "RfxIntsData.h"
#include "RfxAction.h"

/*****************************************************************************
 * Class RfxController
 *****************************************************************************/

class RtcImsRttController : public RfxController {
    // Required: declare this class
    RFX_DECLARE_CLASS(RtcImsRttController);

  public:
    RtcImsRttController();
    virtual ~RtcImsRttController();

    // Override
  protected:
    virtual void onInit();
    virtual void onDeinit();
    virtual bool onHandleRequest(const sp<RfxMessage>& message);
    virtual bool onHandleUrc(const sp<RfxMessage>& message);
    virtual bool onHandleResponse(const sp<RfxMessage>& message);
    virtual bool onCheckIfRejectMessage(const sp<RfxMessage>& message, bool isModemPowerOff,
                                        int radioState);

  private:
    // Private functions
    bool responseToRilj(const sp<RfxMessage>& msg);
};

#endif /* __RFX_IMS_CONTROLLER_H__ */
