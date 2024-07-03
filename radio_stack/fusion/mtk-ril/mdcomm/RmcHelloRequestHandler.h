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

#ifndef __RMC_HELLO_REQUEST_HANDLER_H__
#define __RMC_HELLO_REQUEST_HANDLER_H__

#include "RfxBaseHandler.h"
#include "RfxIntsData.h"
#include "RfxStringData.h"
#include "RfxMessageId.h"
// #include "RfxFooData.h"

#define LOG_TAG "RmcHelloRequestHandler"

class RmcHelloRequestHandler : public RfxBaseHandler {
    RFX_DECLARE_HANDLER_CLASS(RmcHelloRequestHandler);

  public:
    RmcHelloRequestHandler(int slot_id, int channel_id);
    virtual ~RmcHelloRequestHandler();

  protected:
    virtual void onHandleRequest(const sp<RfxMclMessage>& msg);

    virtual void onHandleUrc(const sp<RfxMclMessage>& msg) {}

    virtual void onHandleTimer();

    virtual void onHandleEvent(const sp<RfxMclMessage>& msg);

  private:
    void requestRadioPower(const sp<RfxMclMessage>& msg);
    void modemStateChange(const sp<RfxMclMessage>& msg);
};

#endif
