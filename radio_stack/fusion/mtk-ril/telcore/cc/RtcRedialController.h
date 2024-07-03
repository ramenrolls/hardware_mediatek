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

#ifndef __RTC_REDIAL_CONTROLLER_H__
#define __RTC_REDIAL_CONTROLLER_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "RfxController.h"
#include "RfxDefs.h"
#include "RfxRedialData.h"
#include "RfxRootController.h"
#include "RfxStatusDefs.h"

/*****************************************************************************
 * MACRO
 *****************************************************************************/
#define LOG_REDIAL_TAG "RtcRedialController"

#define ECPI_MSG_TYPE_CALL_CONNECT 6
#define ECPI_MSG_TYPE_CALL_ID_ASSIGN 130
#define ECPI_MSG_TYPE_CALL_DISCONNECT 133

#define REDIAL_RIL_PARCEL_HEADER (2 * sizeof(int))
#define REDIAL_NET_TYPE_GLOBAL_MODE PREF_NET_TYPE_LTE_CMDA_EVDO_GSM_WCDMA
#define REDIAL_NET_TYPE_NONE 0
#define REDIAL_NET_TYPE_RESUME_RAT_MODE -1

#define RAT_OPRATION_RESUME_RAT_MODE 0
#define RAT_OPRATION_SWITCH_GLOBAL_MODE 1

#define CALL_DISC_CAUSE_NEED_CHANGE_TO_GLOBAL_MODE 2003

#define EMERGENCY_CALLBACK_MODE_NONE -1
#define EMERGENCY_CALLBACK_MODE_ENTER 1
#define EMERGENCY_CALLBACK_MODE_EXIT 0

/*****************************************************************************
 * Const
 *****************************************************************************/
static const bool DBG = false;

/*****************************************************************************
 * TYPE
 *****************************************************************************/
enum RedialState {
    REDIAL_NONE = 0,
    REDIAL_DIALING = 1,
    REDIAL_DIALED = 2,
    REDIAL_DONE = 3,
    REDIAL_END = 100,
};

enum RatState {
    REDIAL_RAT_NONE = 0,
    REDIAL_RAT_CHANGED = 1,
    REDIAL_RAT_RESUME = 2,
    REDIAL_RAT_END = 100,
};

enum RedialRatOp {
    REDIAL_RAT_OP_NONE = 0,
    REDIAL_RAT_OP_GLOBAL = 1,
    REDIAL_RAT_OP_RESUME = 2,
    REDIAL_RAT_OP_RESUME_PENDING = 3,
    REDIAL_RAT_OP_END = 100,
};

/*****************************************************************************
 * Class MoCallContext
 *****************************************************************************/
class MoCallContext {
  public:
    explicit MoCallContext(const sp<RfxMessage>& msg, bool isEcc = true);
    explicit MoCallContext(char* callNumber, bool isEcc = true);
    virtual ~MoCallContext();

    int getCallId() const { return mCallId; }

    int getClientId() const { return mClientId; }

    bool isEccCall() const { return mIsEcc; }

    char* getNumber() const;
    RfxRedialData* getRedialData();
    void setCallId(int callId);

  private:
    bool mIsEcc;
    int mCallId;
    int mClientId;
    char* mNumber;
    RfxDialData* mDialData;
    RfxRedialData* mRedialData;
};

/*****************************************************************************
 * Class RtcRedialController
 *****************************************************************************/
class RtcRedialController : public RfxController {
    RFX_DECLARE_CLASS(RtcRedialController);

  public:
    RtcRedialController();
    virtual ~RtcRedialController();
    virtual bool isEmergencyModeSupported(int slotId);

  protected:
    virtual void onInit();
    virtual void onDeinit();
    virtual bool onHandleRequest(const sp<RfxMessage>& message);
    virtual bool onHandleUrc(const sp<RfxMessage>& message);
    virtual bool onHandleResponse(const sp<RfxMessage>& message);
    virtual bool onHandleAtciRequest(const sp<RfxMessage>& msg);
    virtual bool onHandleAtciResponse(const sp<RfxMessage>& msg);
    virtual bool onCheckIfRejectMessage(const sp<RfxMessage>& message, bool isModemPowerOff,
                                        int radioState);

    void resetController();
    void increaseEmcsCount();
    void setEmergencyMode(bool emergencyMode);

  public:
    bool notifyRilRequest(const sp<RfxMessage>& message);
    bool notifyRilResponse(const sp<RfxMessage>& message);
    bool notifyRilUrc(const sp<RfxMessage>& message);
    void setEmergencyModeExt(bool emergencyMode);
    void setEccCallId(int callId);  // for 380 case.

  private:
    void handleEmergencyDial(const sp<RfxMessage>& message);
    bool handleAtciDialRequest(const char* data, int dataLength);
    bool handleCallInfoUpdate(const sp<RfxMessage>& message);
    void handleEmergencyCallbackMode(int mode);

    void ForceReleaseEmergency();
    void checkMoCallAndFree();
    bool checkFeatureEnabled();
    void setUserHangUping(bool userHangUping);
    void setRedialState(RedialState state);
    void setRatMode(RedialRatOp ratOp);
    void destroyMoCallContext();
    bool canReleaseEmergencySession();
    bool decreaseEmcsCountAndIsZero();

    int getGlobalRatMode();
    int getCurrentRatMode();
    void onRatModeSwitchDone(RedialRatOp ratOp);
    sp<RfxMessage> getRedialRequest();
    void redial();

    void deferMsg(const sp<RfxMessage>& msg);
    void resumeDeferMsg();
    void clearDeferMsg();
    void onCallCountChanged(RfxStatusKeyEnum key, RfxVariant old_value, RfxVariant value);

    void setPendingExitEmergencySession(bool pending);
    bool isPendingExitEmergencySession();

  protected:
    MoCallContext* mMoCall;
    bool mInEmergencyMode;
    bool mIsEccModeSent;  // means EMCS sent earlier under flight mode

  private:
    bool mIsUserHangUping;
    bool mPendingExitES;
    int mEcbm;
    int mEmcsCount;

    RedialState mRedialState;
    RatState mRatState;
    RedialRatOp mRatOp;

    Vector<RfxSuspendedMsgEntry> mSuspendedMsgQueue;
};

const char* boolToString(bool value);

#endif /* __RTC_REDIAL_CONTROLLER_H__ */
