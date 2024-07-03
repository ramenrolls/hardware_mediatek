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

#ifndef __RTC_OP_DATA_CONTROLLER_H__
#define __RTC_OP_DATA_CONTROLLER_H__

#include <data/RtcDataController.h>
#include "RfxMessage.h"

/*****************************************************************************
 * Class RtcOpDataController
 *****************************************************************************/
class RtcOpDataController : public RtcDataController {
    RFX_DECLARE_CLASS(RtcOpDataController);  // Required: declare this class
  public:
    RtcOpDataController();
    virtual ~RtcOpDataController();
    // Override
  protected:
    void onInit();
    bool onHandleRequest(const sp<RfxMessage>& message);
    bool onHandleResponse(const sp<RfxMessage>& message);
};
#endif /* __RTC_OP_DATA_CONTROLLER_H__ */
