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

#ifndef __RFX_WP_CONTROLLER_H__
#define __RFX_WP_CONTROLLER_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "RfxController.h"
#include "RfxTimer.h"
#include "RfxVoidData.h"
#include "RfxStringData.h"
#include "RfxStringsData.h"
#include "RfxIntsData.h"
#include "RfxMessageId.h"
#include "RfxTimer.h"

/*****************************************************************************
 * Class RfxController
 *****************************************************************************/
class RtcWpController : public RfxController {
    // Required: declare this class
    RFX_DECLARE_CLASS(RtcWpController);

  public:
    RtcWpController();
    virtual ~RtcWpController();

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

  public:
    // other module check if world mode is switching
    bool isWorldModeSwitching();

  private:
    void processWorldModeResumingResponse(const sp<RfxMessage>& message);
    void resumeWorldModeChange();
    int getCallingSlotId();
    void handleWorldModePendedByCall(int slotId);
    void onCallCountChanged(int slotId, RfxStatusKeyEnum key, RfxVariant old_value,
                            RfxVariant value);
    void resumeWorldModeChangeWithRadioOff();
    void requestRadioOn();
    void requestRadioOff();
    void onRequestRadioOffDone(int slotId);
    void onRequestRadioOnDone(int slotId);
    // handle start URC from MCL
    void handleWorldModeChangeStart(bool flag, int cause);
    bool canhandleMessage(bool log_flag);
    void onRetryResumeTimeOut();
    void onTelephonyAssistantStatusChanged(int slotId, RfxStatusKeyEnum key, RfxVariant old_value,
                                           RfxVariant value);
    void createNewMessageToRmc(int messageId, int slotId, int value);
    // other module needs to block world mode switch
    int blockWorldModeChanged();
    // other module agrees with world mode switch
    int resumeBlockedWorldModeChanged();

  private:
    // record the number other module blocks world mode switch
    static int worldModeBlockedNum;
    // record the slot start urc coming
    static int mainSlotId;
    static int mainWorldModeId;
    int getBlockedWorldModeStatusKeyValue(bool log_flag);
    int setBlockedWorldModeStatusKeyValue(int value);
    bool backupRadioPower[MAX_SIM_COUNT];
    int closeRadioCount;
    int openRadioCount;
    TimerHandle mWpTimerHandle;
    int mRetryResumeCount;
};

#endif /* __RFX_WP_CONTROLLER_H__ */
