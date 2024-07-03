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

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "RfxBaseHandler.h"
#include "RfxVoidData.h"
#include "RfxLceStatusResponseData.h"
#include "RfxLceDataResponseData.h"
#include "RfxLinkCapacityEstimateData.h"

/*****************************************************************************
 * Class RmcDcMiscImpl
 *****************************************************************************/
class RmcDcMiscImpl {
  public:
    RmcDcMiscImpl(RfxBaseHandler* handler, int slotId);
    virtual ~RmcDcMiscImpl();
    void init();
    void setFdMode(const sp<RfxMclMessage>& msg);
    void requestStartLce(const sp<RfxMclMessage>& msg);
    void requestStopLce(const sp<RfxMclMessage>& msg);
    void requestPullLceData(const sp<RfxMclMessage>& msg);
    void onLceStatusChanged(const sp<RfxMclMessage>& msg);
    void requestSetLinkCapacityReportingCriteria(const sp<RfxMclMessage>& msg);
    void onLinkCapacityEstimate(const sp<RfxMclMessage>& msg);

  protected:
    int getIntValue(int slotId, const RfxStatusKeyEnum key, int default_value);
    RfxMclStatusManager* getMclStatusManager(int slotId);
    sp<RfxAtResponse> atSendCommand(const String8 cmd);
    sp<RfxAtResponse> atSendCommandSingleline(const String8 cmd, const char* rspPrefix);
    void responseToTelCore(const sp<RfxMclMessage> msg);

  private:
    void initializeFastDormancy();
    int isFastDormancySupport();
    void syncEpdgConfigToMd();
    void syncIwlanOperationModeToMd();
    void disableLegacyFastDormancy();

  private:
    RfxBaseHandler* mHandler;
    int mSlotId;
};
