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

#ifndef __RTC_PHB_CONTROLLER_H__
#define __RTC_PHB_CONTROLLER_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "RfxController.h"

/*****************************************************************************
 * Class RfxController
 *****************************************************************************/
class RtcPhbController : public RfxController {
    // Required: declare this class
    RFX_DECLARE_CLASS(RtcPhbController);

  public:
    RtcPhbController();
    virtual ~RtcPhbController();

    // Override
  protected:
    virtual void onInit();
    virtual bool onCheckIfRejectMessage(const sp<RfxMessage>& message, bool isModemPowerOff,
                                        int radioState);
    virtual bool onHandleRequest(const sp<RfxMessage>& message);

  private:
    void onMdOffChanged(RfxStatusKeyEnum key, RfxVariant oldValue, RfxVariant newValue);
};

#endif /* __RTC_PHB_CONTROLLER_H__ */
