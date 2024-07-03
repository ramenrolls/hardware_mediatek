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

#ifndef __RFX_OP_CALL_CONTROLLER_H__
#define __RFX_OP_CALL_CONTROLLER_H__

/*****************************************************************************
 * Include
 *****************************************************************************/

#include "RfxAction.h"
#include "RfxController.h"
#include "RfxIntsData.h"
#include "RfxRilUtils.h"
#include "RfxRootController.h"
#include "RfxSignal.h"
#include "RfxStringsData.h"
#include "RfxTimer.h"

#include "cc/RtcCallController.h"
#include "ims/RtcOpImsController.h"

#include "rfx_properties.h"
#include "libmtkrilutils.h"

#include <telephony/mtk_rilop.h>

/*****************************************************************************
 * Class RtcOpCallController
 *****************************************************************************/

typedef struct {
    TimerHandle timerHandle;
    sp<RfxMessage> message;
} NoTrnTimeoutHandler;

class RtcOpCallController : public RtcCallController {
    // Required: declare this class
    RFX_DECLARE_CLASS(RtcOpCallController);

  public:
    RtcOpCallController();
    virtual ~RtcOpCallController();

    // Override
  protected:
    virtual void onInit();
    virtual bool onHandleRequest(const sp<RfxMessage>& message);
    virtual bool onHandleUrc(const sp<RfxMessage>& message);
    virtual bool onHandleResponse(const sp<RfxMessage>& message);
    virtual void createRedialController();

  private:
    bool handleOp08DialRequest(const sp<RfxMessage>& message);
    void onGetTrnInternal(const sp<RfxMessage> origMsg);
    void onGetTrnInternalTimeout();
    void onGetTrnForModem(const char* fromMsisdn, const char* toMsisdn);
    void handleQueryTrn(const sp<RfxMessage>& message);
    bool handleOp08IncomingCall(const sp<RfxMessage>& message);

    NoTrnTimeoutHandler mNoTrnTimeoutHandler;
};

#endif
