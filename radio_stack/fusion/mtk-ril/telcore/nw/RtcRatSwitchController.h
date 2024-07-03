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

#ifndef __RTC_RAT_SWITCH_CONTROLLER_H__
#define __RTC_RAT_SWITCH_CONTROLLER_H__

#undef NDEBUG
#ifdef LOG_NDEBUG
#undef LOG_NDEBUG
#endif
/*****************************************************************************
 * Include
 *****************************************************************************/
#include "RfxController.h"
#include "RfxAction.h"
#include "RtcNwDefs.h"
#include "RfxVoidData.h"
#include "RfxRilUtils.h"
#include "RfxStringData.h"
#include "RfxStringsData.h"
#include "RfxIntsData.h"
#include "RfxMessageId.h"
#include "modecontroller/RtcModeSwitchController.h"
#include "capabilityswitch/RtcCapabilitySwitchUtil.h"
#include "wp/RtcWpController.h"
#include <telephony/mtk_ril.h>
#include "rfx_properties.h"
#include <libmtkrilutils.h>

/*****************************************************************************
 * Define
 *****************************************************************************/
typedef struct {
    int prefNwTypeFromRilj = -1;  // RILJ setting
    int prefNwTypeDefault = -1;   // Default setting
    int prefNwType = -1;          // MD setting
    RatSwitchCaller ratSwitchCaller = RAT_SWITCH_UNKNOWN;
    sp<RfxAction> action;
    sp<RfxMessage> message;
} RatSettings;

typedef enum { SWITCH_RAT, GMSS_RAT } PHONE_CHANGE_SOURCE;

typedef enum {
    PS_RAT_FAMILY_UNKNOWN = 0,
    PS_RAT_FAMILY_GSM = 1,
    PS_RAT_FAMILY_CDMA = 2,
    PS_RAT_FAMILY_IWLAN = 4,
} PsRatFamily;

/*****************************************************************************
 * Class RfxController
 *****************************************************************************/
class RtcRatSwitchController : public RfxController {
    // Required: declare this class
    RFX_DECLARE_CLASS(RtcRatSwitchController);

  public:
    RtcRatSwitchController();
    virtual ~RtcRatSwitchController();
    void setPreferredNetworkType(RatSwitchInfo ratSwtichInfo);
    void setPreferredNetworkType(const int prefNwType, const sp<RfxAction>& action);
    PsRatFamily getPsRatFamily(int radioTechnology);
    void setLocalAbortAvailableNetwork(const sp<RfxAction>& action);
    bool isECBMRestrictedModeSupport();

  protected:
    virtual bool onHandleRequest(const sp<RfxMessage>& message);
    virtual bool onHandleUrc(const sp<RfxMessage>& message);
    virtual bool onHandleResponse(const sp<RfxMessage>& message);
    virtual void onInit();
    virtual void onDeInit();

    virtual bool onCheckIfRejectMessage(const sp<RfxMessage>& message, bool isModemPowerOff,
                                        int radioState);
    virtual bool onPreviewMessage(const sp<RfxMessage>& message);
    virtual bool onCheckIfResumeMessage(const sp<RfxMessage>& message);

  private:
    int calculateDefaultNetworkType(RatSwitchInfo ratSwtichInfo);
    void setPreferredNetworkType(const sp<RfxMessage>& message);
    void getPreferredNetworkType(const sp<RfxMessage>& message);
    void requestVoiceRadioTech(const sp<RfxMessage>& message);
    bool onHandleAtciRequest(const sp<RfxMessage>& message);
    void handleGmssRatChanged(const sp<RfxMessage>& message);

    void responseSetPreferredNetworkType(const sp<RfxMessage>& response);
    void responseGetPreferredNetworkType(const sp<RfxMessage>& response);
    void responseGetVoiceRadioTech(const sp<RfxMessage>& response);

    void switchNwRat(int prefNwType, const RatSwitchCaller ratSwitchCaller,
                     const sp<RfxAction>& action, const sp<RfxMessage>& message);
    void configRatMode();
    void queueRatSwitchRecord(int prefNwType, const RatSwitchCaller ratSwitchCaller,
                              const sp<RfxAction>& action, const sp<RfxMessage>& message);
    void processPendingRatSwitchRecord();
    bool hasPendingRecord();
    void updateState(int prefNwType, RatSwitchResult switchResult);
    void updatePhoneMode(PHONE_CHANGE_SOURCE source);
    void onApVoiceCallCountChanged(int slotId, RfxStatusKeyEnum key, RfxVariant old_value,
                                   RfxVariant value);
    void onVolteStateChanged(RfxStatusKeyEnum key, RfxVariant old_value, RfxVariant value);
    void onImsRegistrationStatusChanged(RfxStatusKeyEnum key, RfxVariant old_value,
                                        RfxVariant value);

    int isNetworkScanOngoing();
    bool isAPInCall();
    bool isGsmOnlySimFromMode(RatSwitchInfo ratSwtichInfo);
    bool isNoSimInserted();
    bool isCT4GCard();
    bool isGsmOnlySim();
    bool isCdmaOnlySim();
    bool isCdmaDualModeSimCard();
    bool isSlaveInDualCDMACard();
    bool isNetworkTypeContainLteCdma();
    int getMajorSlotId();
    int getCCapabilitySlotId();
    bool isHvolteSupport();

    void setSlotCapability(int slotId, int val);
    int getSlotCapability(int slotId);
    int getDefaultNetWorkType(int slotId);
    void setPreferredNetWorkTypeToSysProp(int slotId, int val);
    int getPreferredNetWorkTypeFromSysProp(int slotId);
    void setIntSysProp(char* propName, int val);
    int getIntSysProp(char* propName, int defaultVal);
    int isNewSimCard(int slotId);
    int isRafContainsCdma(int raf);
    int isRafContainsGsm(int raf);

    const char* switchCallerToString(int callerEnum);

    int filterPrefNwTypeFromRilj(const int prefNwType);

    void responseGetGmssRatMode(const sp<RfxMessage>& response);
    void getGmssRatMode();
    void handleGmssRat(const sp<RfxMessage>& message);

    int getIccIdCacheIndex(int slotId);
    int getNwTypeCacheByIndex(int index);
    void setNwTypeCacheByIndex(int index, int type);

    void onUiccMccMncChanged(RfxStatusKeyEnum key, RfxVariant old_value, RfxVariant value);
    bool needDisableCdmaRat();
    bool needDisableCdmaRatByMccMnc(const char* mccmnc);

    void onRestrictedModeChanged(RfxStatusKeyEnum key, RfxVariant old_value, RfxVariant value);

  public:
    RfxSignal2<int, int> mRatSwitchSignal;

  private:
    static bool sIsInSwitching;
    Vector<int> mTokensGetEgmssForHvolte;
    RatSettings mRatSettings;

    RatSettings mPendingInitRatSwitchRecord;
    RatSettings mPendingNormalRatSwitchRecord;
    RatSettings mPendingRestrictedRatSwitchRecord;

    int mDefaultNetworkType;
    int mCurPreferedNetWorkType;
    int mPhoneMode;
    NwsMode mNwsMode;
    int mNetworkTypeIndex;
    int mNetworkTypeCache;
    int mRestrictedCount;
    bool mInRestrictedMode;
    sp<RfxAction> mAbortAction;
};
#endif /* __RTC_RAT_SWITCH_CONTROLLER_H__ */
