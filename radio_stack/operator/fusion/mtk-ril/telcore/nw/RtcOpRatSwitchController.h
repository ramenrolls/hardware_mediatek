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

#ifndef __RTC_OP_RAT_SWITCH_CONTROLLER_H__
#define __RTC_OP_RAT_SWITCH_CONTROLLER_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "RfxController.h"
#include "RfxVoidData.h"
#include "RfxIntsData.h"
#include <telephony/mtk_ril.h>
#include "libmtkrilutils.h"

/*****************************************************************************
 * Class RfxController
 *****************************************************************************/
class RtcOpRatSwitchController : public RfxController {
    // Required: declare this class
    RFX_DECLARE_CLASS(RtcOpRatSwitchController);

  public:
    RtcOpRatSwitchController();
    virtual ~RtcOpRatSwitchController();

  protected:
    void onInit();
    bool onHandleRequest(const sp<RfxMessage>& message);
    bool onHandleResponse(const sp<RfxMessage>& message);
    bool onPreviewMessage(const sp<RfxMessage>& message);
    bool onCheckIfResumeMessage(const sp<RfxMessage>& message);
    bool onCheckIfRejectMessage(const sp<RfxMessage>& message, bool isModemPowerOff,
                                int radioState);

    void requestRadioPower(bool state);
    void onRequestRadioOffDone(int slotId);
    void onRequestRadioOnDone(int slotId);

  private:
    sp<RfxMessage> mMessage;
    RIL_Errno mError;
    bool backupRadioPower[MAX_SIM_COUNT];
    int radioCount;
};
#endif /* __RTC_OP_RAT_SWITCH_CONTROLLER_H__ */
