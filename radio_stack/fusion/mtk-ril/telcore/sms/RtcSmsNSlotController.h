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

#ifndef __RTC_SMS_NSLOT_CONTROLLER_H__
#define __RTC_SMS_NSLOT_CONTROLLER_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include <utils/Vector.h>
#include "RfxController.h"
#include "RtcSmsUtil.h"
#include "RtcSmsThread.h"
#include "parser/SuplMsgDispatcher.h"

/*****************************************************************************
 * Class Declaraion
 *****************************************************************************/
class RtcSmsParsingMessage;

/*****************************************************************************
 * Class RtcSmsController
 *****************************************************************************/

class RtcSmsNSlotController : public RfxController {
    // Required: declare this class
    RFX_DECLARE_CLASS(RtcSmsNSlotController);

  public:
    RtcSmsNSlotController();
    virtual ~RtcSmsNSlotController();

    void onHandleSmsMessage(const sp<RtcSmsParsingMessage>& message);

    void dispatchSms(const sp<RfxMessage>& msg);
    // Override
  protected:
    virtual void onInit();
    virtual void onDeinit();

  private:
    sp<RtcSmsThread> getSmsThread();

  private:
    // reference of sms working thread
    sp<RtcSmsThread> m_smsThread;
    Vector<SuplMsgDispatcher*>* mDispatcherList;
};

#endif /* __RTC_SMS_NSLOT_CONTROLLER_H__ */
