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

#ifndef __RMC_OEM_CONTROLLER_H__
#define __RMC_OEM_CONTROLLER_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "RfxController.h"

/*****************************************************************************
 * Class RfxController
 *****************************************************************************/

class RtcOemController : public RfxController {
    RFX_DECLARE_CLASS(RtcOemController);  // Required: declare this class

  public:
    RtcOemController();
    virtual ~RtcOemController();

    // Override
  protected:
    void onInit();
    bool onCheckIfRejectMessage(const sp<RfxMessage>& message, bool isModemPowerOff,
                                int radioState);
    bool onHandleRequest(const sp<RfxMessage>& message);
    bool onHandleResponse(const sp<RfxMessage>& message);

  private:
    void onHandleOemHookRaw(const sp<RfxMessage>& message);
    void onHandleOemHookStrings(const sp<RfxMessage>& message);
    void onHandleGetActivityInfo(const sp<RfxMessage>& message);
    bool isMainProtocolCommand(char* target);
    void onTelephonyAssistantStatusChanged(RfxStatusKeyEnum key, RfxVariant old_value,
                                           RfxVariant value);
    bool handleExtraState(const sp<RfxMessage>& message);
};

#endif /* __RP_OEM_CONTROLLER_H__ */
