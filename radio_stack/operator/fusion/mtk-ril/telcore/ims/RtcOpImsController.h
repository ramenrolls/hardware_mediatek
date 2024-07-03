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

#ifndef __RTC_OP_IMS_CONTROLLER_H__
#define __RTC_OP_IMS_CONTROLLER_H__

/*****************************************************************************
 * Include
 *****************************************************************************/

#include "RfxController.h"
#include <string>
#include <unordered_map>
#include "RfxAction.h"

/*****************************************************************************
 * Class RfxController
 *****************************************************************************/

class RfxStringsData;

class RtcOpImsController : public RfxController {
    // Required: declare this class
    RFX_DECLARE_CLASS(RtcOpImsController);

  public:
    RtcOpImsController();
    virtual ~RtcOpImsController();

    void getTrn(const char* fromMsisdn, const char* toMsisdn, const char* address,
                const sp<RfxAction>& action);

    // Override
  protected:
    virtual void onInit();
    virtual void onDeinit();
    virtual bool onHandleRequest(const sp<RfxMessage>& message);
    virtual bool onHandleUrc(const sp<RfxMessage>& message);
    virtual bool onHandleResponse(const sp<RfxMessage>& response);
    virtual bool onCheckIfRejectMessage(const sp<RfxMessage>& message, bool isModemPowerOff,
                                        int radioState);

  private:
    void handleSetTrn(const sp<RfxMessage>& message);

    sp<RfxAction> findAction(std::string addrString);

    std::unordered_map<std::string, sp<RfxAction>> mActionMap;
};

#endif /* __RTC_OP_IMS_CONTROLLER_H__ */
