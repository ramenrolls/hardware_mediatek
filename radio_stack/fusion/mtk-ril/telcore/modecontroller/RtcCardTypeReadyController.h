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

#ifndef __RTC_CARD_TYPE_READY_CONTROLLER_H__
#define __RTC_CARD_TYPE_READY_CONTROLLER_H__

/************************************************************************
 * Include
 ************************************************************************/
#include "RfxController.h"
#include "RfxTimer.h"
#include "RtcModeDefs.h"

/************************************************************************
 * Class RtcCardTypeReadyController
 ************************************************************************/

class RtcCardTypeReadyController : public RfxController {
    RFX_DECLARE_CLASS(RtcCardTypeReadyController);
    RFX_OBJ_DECLARE_SINGLETON_CLASS(RtcCardTypeReadyController);

  public:
    RtcCardTypeReadyController();
    void onInit();
    void onDeinit();
    virtual ~RtcCardTypeReadyController();
    void setTimeoutMescForCommonSlotUT(int msec);

  private:
    void onSimTaskReady(int slotId, RfxStatusKeyEnum key, RfxVariant old_value, RfxVariant value);
    bool isCommontSlotSupport();
    bool isVsimEnabled();
    bool needMergeNotify(int slotId);
    void onMergeNotifyTimeout();
    bool isAnyCardInvalid();
    void onSimSwitchChanged(RfxStatusKeyEnum key, RfxVariant old_value, RfxVariant value);

  private:
    TimerHandle mTimeoutHandle;
    int* m_card_types;
    int* m_card_type_states;
    int m_timeout_msec;
};

#endif /* __RTC_CARD_TYPE_READY_CONTROLLER_H__ */
