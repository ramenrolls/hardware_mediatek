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

#ifndef __RTC_EMBMS_AT_CONTROLLER_H__
#define __RTC_EMBMS_AT_CONTROLLER_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include <utils/Vector.h>
#include "RfxController.h"
#include "RfxTimer.h"
#include "RtcEmbmsSessionInfo.h"
#include <telephony/mtk_ril.h>

/*****************************************************************************
 * Class RtcEmbmsAtController
 *****************************************************************************/

#define EMBMS_ASSERT(_c) assert(_c)
#define EMBMS_UNUSED(x) ((void)(x))

extern "C" int strStartsWith(const char* line, const char* prefix);

/*****************************************************************************
 * PRIVATE ENUM
 *****************************************************************************/

/*****************************************************************************
 * Class RtcEmbmsAtController
 *****************************************************************************/
class RtcEmbmsAtController : public RfxController {
    RFX_DECLARE_CLASS(RtcEmbmsAtController);  // Required: declare this class

  public:
    RtcEmbmsAtController();

    virtual ~RtcEmbmsAtController();

    // Override
  protected:
    virtual void onInit();
    virtual void onDeinit();
    virtual bool onHandleRequest(const sp<RfxMessage>& message);
    virtual bool onHandleUrc(const sp<RfxMessage>& message);
    virtual bool onHandleResponse(const sp<RfxMessage>& message);
    virtual bool onCheckIfRejectMessage(const sp<RfxMessage>& message, bool isModemPowerOff,
                                        int radioState);
    virtual void handleRequestDefault(const sp<RfxMessage>& request);

  private:
    // Private functions
    bool isEmbmsSupported();
    int strStartsWith(const char* line, const char* prefix);
    void responseToSocket(const sp<RfxMessage>& msg, const char* responseStr);
    void urcToSocket(const sp<RfxMessage>& msg, const char* responseStr,
                     const char* responseStr_mask);
    void responseErrorString(const sp<RfxMessage>& msg);
    void handleAtRequest(const sp<RfxMessage>& request);
    void handleAtResponse(const sp<RfxMessage>& response);
    void handleAtUrc(const sp<RfxMessage>& message);

    void requestAtEnable(const sp<RfxMessage>& request);
    void handleEnableResponse(const sp<RfxMessage>& response);
    void handleDisableResponse(const sp<RfxMessage>& response);

    void requestAtGetNetworkTime(const sp<RfxMessage>& request);
    void handleGetNetworkTimeResponse(const sp<RfxMessage>& response);

    void requestAtGetCoverage(const sp<RfxMessage>& request);
    void handleGetCoverageResponse(const sp<RfxMessage>& response);

    void requestAtSetAvailbility(const sp<RfxMessage>& request);
    void requestAtActivate(const sp<RfxMessage>& request, embms_at_activesession_enum type);
    void handleStartSessionResponse(const sp<RfxMessage>& response);
    void handleStartSessionUrc(const sp<RfxMessage>& message);
    void handleStopSessionResponse(const sp<RfxMessage>& response);

    void handleTriggerCellInfoNotifyResponse(const sp<RfxMessage>& response);

    void handleCellInfoUrc(const sp<RfxMessage>& message);
    void handleCoverageUrc(const sp<RfxMessage>& message);
    void handleSessionListUrc(const sp<RfxMessage>& message);
    void handleActiveSessionStatus(const sp<RfxMessage>& response);
    void handleOosUrc(const sp<RfxMessage>& message);
    void handleSaiUrc(const sp<RfxMessage>& message);
    int getCoverageState(int status);

    void onMainCapabilityChanged(RfxStatusKeyEnum key, RfxVariant old_value, RfxVariant value);
    void onDefaultDataChanged(RfxStatusKeyEnum key, RfxVariant old_value, RfxVariant value);

  private:
    // Private members
    Vector<RtcEmbmsSessionInfo*>* mSessionInfoList;
    bool mIsActiveSession;
    bool mIsFlightOn;
    bool mIsEmbmsSupport;
    bool mEmbmsEnabled;
    int mSdkVersion;  // XX.YY.ZZ
};

#endif /* __RP_DATA_CONTROLLER_H__ */
