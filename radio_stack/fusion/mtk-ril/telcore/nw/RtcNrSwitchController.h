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

#ifndef __RTC_NR_SWITCH_CONTROLLER_H__
#define __RTC_NR_SWITCH_CONTROLLER_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "RfxController.h"
#include <telephony/mtk_ril.h>
#include "rfx_properties.h"
#include "RfxIntsData.h"

/*****************************************************************************
 * Define
 *****************************************************************************/

/*****************************************************************************
 * Class RfxController
 *****************************************************************************/
class RtcNrSwitchController : public RfxController {
    RFX_DECLARE_CLASS(RtcNrSwitchController);

  public:
    RtcNrSwitchController();
    virtual ~RtcNrSwitchController();

  protected:
    virtual bool onHandleResponse(const sp<RfxMessage>& message);
    virtual void onInit();

    void responseNrSwitch(const sp<RfxMessage>& response);
    int getNrSlot();
    void onPreferredDataChanged(RfxStatusKeyEnum key, RfxVariant old_value, RfxVariant value);
    void onApVoiceCallCountChanged(int slotId, RfxStatusKeyEnum key, RfxVariant old_value,
                                   RfxVariant value);

  protected:
    int mNrSlot;
    int mPreferredDataSlot;
};
#endif /* __RTC_NR_SWITCH_CONTROLLER_H__ */
