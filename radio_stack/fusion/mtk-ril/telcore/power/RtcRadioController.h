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

#ifndef __RMC_RADIO_CONTROLLER_H__
#define __RMC_RADIO_CONTROLLER_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "RfxController.h"
#include <unordered_map>
#include "RfxAction.h"

/*****************************************************************************
 * Class RfxController
 *****************************************************************************/

class RtcRadioController : public RfxController {
    RFX_DECLARE_CLASS(RtcRadioController);  // Required: declare this class

  public:
    RtcRadioController();
    virtual ~RtcRadioController();

    // Override
  protected:
    virtual bool onHandleRequest(const sp<RfxMessage>& message);
    virtual bool onHandleUrc(const sp<RfxMessage>& message);
    virtual bool onHandleResponse(const sp<RfxMessage>& message);
    virtual bool onPreviewMessage(const sp<RfxMessage>& message);
    virtual bool onCheckIfResumeMessage(const sp<RfxMessage>& message);
    virtual void onInit();
    virtual bool onCheckIfRejectMessage(const sp<RfxMessage>& message, bool isModemPowerOff,
                                        int radioState);

  public:
    void moduleRequestRadioPower(bool power, const sp<RfxAction>& action, RadioCause cause);

  private:
    void registerForStatusChange();

    bool canHandleRequest(const sp<RfxMessage>& message);

    void requestRadioPower(const sp<RfxMessage>& message);

    bool handleRadioPowerResponse(const sp<RfxMessage>& message);

    void onRadioStateChanged(RfxStatusKeyEnum key, RfxVariant old_value, RfxVariant value);

    sp<RfxMessage> findPendingRequest(std::unordered_map<int, sp<RfxMessage>>& pendingRequest,
                                      const sp<RfxMessage>& msg);

    sp<RfxAction> findAction(std::unordered_map<int, sp<RfxAction>>& mActionMap,
                             const sp<RfxMessage>& msg);

    void onSimStateChanged(RfxStatusKeyEnum key, RfxVariant old_value, RfxVariant value);

    std::unordered_map<int, sp<RfxMessage>> mPendingRequest;
    std::unordered_map<int, sp<RfxAction>> mActionMap;
};

#endif /* __RP_RADIO_CONTROLLER_H__ */
