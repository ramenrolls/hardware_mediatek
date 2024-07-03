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

#ifndef __RFX_IMS_CONTROLLER_H__
#define __RFX_IMS_CONTROLLER_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "RfxController.h"
#include "RfxTimer.h"
#include "RfxIntsData.h"
#include "RfxAction.h"

/*****************************************************************************
 * Class RfxController
 *****************************************************************************/

class RtcImsController : public RfxController {
    // Required: declare this class
    RFX_DECLARE_CLASS(RtcImsController);

  public:
    RtcImsController();
    virtual ~RtcImsController();

    void requestImsDisable(int slotId, const sp<RfxAction>& action);
    void requestImsResume(int slotId, const sp<RfxAction>& action);

    // Override
  protected:
    virtual void onInit();
    virtual void onDeinit();
    virtual bool onHandleRequest(const sp<RfxMessage>& message);
    virtual bool onHandleUrc(const sp<RfxMessage>& message);
    virtual bool onHandleResponse(const sp<RfxMessage>& message);
    virtual bool onCheckIfRejectMessage(const sp<RfxMessage>& message, bool isModemPowerOff,
                                        int radioState);
    virtual bool onPreviewMessage(const sp<RfxMessage>& message);
    virtual bool onCheckIfResumeMessage(const sp<RfxMessage>& message);

  private:
    // Private functions
    void handleImsRegStateRequest(const sp<RfxMessage>& message);
    void handleImsRegStateResponse(const sp<RfxMessage>& message);
    void onRadioStateChanged(RfxStatusKeyEnum key, RfxVariant oldValue, RfxVariant value);
    bool responseToRilj(const sp<RfxMessage>& msg);
    /// M: add for op09 default volte setting @{
    void initOp09Ims();
    void deinitOp09Ims();
    void onMainCapabilitySlotChanged(RfxStatusKeyEnum key, RfxVariant old_value,
                                     RfxVariant new_value);
    void onCardTypeChanged(RfxStatusKeyEnum key, RfxVariant old_value, RfxVariant value);
    void onNoIccIdTimeout();
    void setDefaultVolteState(int slot_id, char new_iccid[], int card_type);
    void setVolteStateProperty(int slot_id, bool isEnable);
    bool isOp09SimCard(int slot_id, char icc_id[], int card_type);
    void sendDefaultVolteStateUrc(int slot_id, int value);
    void handleSetImsConfigRequest(const sp<RfxMessage>& message);
    void handleSetImsConfigResponse(const sp<RfxMessage>& message);
    void setVolteSettingStatus(const sp<RfxMessage>& message);
    static const char* givePrintableStr(const char* iccId);
    /// @}
    void updateImsRegStatus();
    void handleImsDisableDoneUrc(const sp<RfxMessage>& message);
    void handleXui(const sp<RfxMessage>& response);
    void cacheimscfgparams(const sp<RfxMessage>& message);
    void mmsoverruleimscfg(const sp<RfxMessage>& message);
    void sendCachedXui();
    void handleImsRegInfoInd(const sp<RfxMessage>& message);
    void sendCachedImsRegInfo();

  private:
    // Private members
    /// M: add for op09 default volte setting @{
    TimerHandle mNoIccidTimerHandle;
    int mNoIccidRetryCount;
    int mMainSlotId;
    bool mIsBootUp;
    bool mIsSimSwitch;
    bool mIsImsDisabling;
    static char sLastBootIccId[4][21];
    static int sLastBootVolteState;
    static bool sInitDone;
    /// @}
    static bool mImsDisByMms[4];
    static int imscfgcache[4][6];  //[slot][params]
    sp<RfxMessage> mCachedXuiUrc;
};

#endif /* __RFX_IMS_CONTROLLER_H__ */
