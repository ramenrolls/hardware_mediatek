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

#ifndef __RTC_VT_CONTROLLER_H__
#define __RTC_VT_CONTROLLER_H__

#define MAX_SIM_COUNT 4
#define DEFAULT_OP_ID 8
#define DEFAULT_MCCMNC 310260

/*****************************************************************************
 * Include
 *****************************************************************************/
#include <map>
#include <utils/Vector.h>
#include "RfxController.h"
#include "RfxVoidData.h"
#include "RfxVtCallStatusData.h"
#include "RfxVtSendMsgData.h"

/*****************************************************************************
 * Class RpDataAllowController
 *****************************************************************************/

/* {mccMnc-start, mccMnc-end, OP-ID} */
typedef struct OperatorMapStruct {
    int mccMnc_range_start;
    int mccMnc_range_end;
    int opId;
} OperatorMap;

class RtcVtController : public RfxController {
    RFX_DECLARE_CLASS(RtcVtController);  // Required: declare this class

  public:
    RtcVtController();

    virtual ~RtcVtController();
    bool isVTLogEnable(void);
    bool isImsVideoCallon(void);

    // Override
  protected:
    virtual void onInit();
    virtual void onDeinit();
    virtual bool onHandleRequest(const sp<RfxMessage>& message);
    virtual bool onHandleResponse(const sp<RfxMessage>& message);
    virtual bool onPreviewMessage(const sp<RfxMessage>& message);
    virtual bool onCheckIfResumeMessage(const sp<RfxMessage>& message);
    virtual void handleGetInfoRequest(const sp<RfxMessage>& request);
    virtual void handleGetInfoResponse(const sp<RfxMessage>& response);
    virtual void handleUpdateOpidResponse(const sp<RfxMessage>& response);

    void onCallStatusChanged(RfxStatusKeyEnum key, RfxVariant old_value, RfxVariant new_value);

  private:
    void onUiccMccMncChanged(RfxStatusKeyEnum key, RfxVariant oldValue, RfxVariant value);
    void onCardTypeChanged(RfxStatusKeyEnum key, RfxVariant oldValue, RfxVariant value);
    void updateOpId(int mccmnc);

    int mCurrentOpid;
};
#endif
