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

#ifndef __RFX_HELLO_CONTROLLER_H__
#define __RFX_HELLO_CONTROLLER_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "RfxController.h"
#include "RfxTimer.h"
#include "RfxVoidData.h"
#include "RfxStringData.h"
#include "RfxStringsData.h"
#include "RfxIntsData.h"

/*****************************************************************************
 * Class RfxController
 *****************************************************************************/
class RtcHelloController : public RfxController {
    // Required: declare this class
    RFX_DECLARE_CLASS(RtcHelloController);

  public:
    RtcHelloController();
    virtual ~RtcHelloController();

    // Override
  protected:
    virtual bool onHandleRequest(const sp<RfxMessage>& message);
    virtual bool onHandleUrc(const sp<RfxMessage>& message);
    virtual bool onHandleResponse(const sp<RfxMessage>& message);
    virtual void onInit();

  public:
    void responseCallBack(const sp<RfxMessage>& message);

  private:
    void onCardTypeChanged(RfxStatusKeyEnum key, RfxVariant old_value, RfxVariant value);
    void onGetIMEI();
    void onSampleControlerSomethingChanged(int d1, int d2, int d3, int d4);
    void onTimer();

  private:
    int m_member_data1;
    int m_member_data2;
    TimerHandle m_timer_handle;
};

#endif /* __RFX_HELLO_CONTROLLER_H__ */
