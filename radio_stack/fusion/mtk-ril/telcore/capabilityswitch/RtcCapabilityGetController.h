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

#ifndef __RTC_CAPABILITY_GET_CONTROLLER_H__
#define __RTC_CAPABILITY_GET_CONTROLLER_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "RfxController.h"
#include "RfxVoidData.h"
#include "RfxStringData.h"
#include "RfxStringsData.h"
#include "RfxIntsData.h"

/*****************************************************************************
 * Class RtcCapabilityGetController
 *****************************************************************************/
class RtcCapabilityGetController : public RfxController {
    // Required: declare this class
    RFX_DECLARE_CLASS(RtcCapabilityGetController);

  public:
    RtcCapabilityGetController();
    virtual ~RtcCapabilityGetController();

    void updateRadioCapability(int cslot);
    void updateRadioCapability(int cslot, int session_id);
    void updateRadioCapabilityForWMChange(int world_mode);

    // Override
  protected:
    virtual bool onHandleUrc(const sp<RfxMessage>& message);
    virtual void onInit();

  private:
    void sendRadioCapabilityURC(int slot_id, int session_id, int radio_capability);
    void onModeSwitchFinished(RfxStatusKeyEnum key, RfxVariant old_value, RfxVariant value);

  private:
    int m_cslot;
    bool m_first_urc;
};

#endif /* __RTC_CAPABILITY_GET_CONTROLLER_H__ */
