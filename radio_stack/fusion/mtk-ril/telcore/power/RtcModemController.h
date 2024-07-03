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

#ifndef __RTC_MODEM_CONTROLLER_H__
#define __RTC_MODEM_CONTROLLER_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "RfxController.h"

#define INITIAL_RETRY_INTERVAL_MSEC 200  // 200ms

/*****************************************************************************
 * Class RfxController
 *****************************************************************************/

class RtcModemController : public RfxController {
    RFX_DECLARE_CLASS(RtcModemController);  // Required: declare this class

  public:
    RtcModemController();
    virtual ~RtcModemController();

  protected:
    virtual void onInit();
    virtual void onDeinit();
    virtual bool onHandleRequest(const sp<RfxMessage>& message);
    virtual bool onHandleResponse(const sp<RfxMessage>& message);
    virtual bool onPreviewMessage(const sp<RfxMessage>& message);
    virtual bool onCheckIfResumeMessage(const sp<RfxMessage>& message);
    virtual bool onCheckIfRejectMessage(const sp<RfxMessage>& message, bool isModemPowerOff,
                                        int radioState);

  private:
    void requestModemPower(bool power, const sp<RfxMessage>& message);
    bool canHandleRequest(const sp<RfxMessage>& message);
    void responseModemPower(const sp<RfxMessage>& message);
};
#endif /* __RP_MODEM_CONTROLLER_H__ */
