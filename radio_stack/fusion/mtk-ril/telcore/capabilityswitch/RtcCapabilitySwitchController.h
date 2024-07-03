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

#ifndef __RTC_CAPABILITY_SWITCH_CONTROLLER_H__
#define __RTC_CAPABILITY_SWITCH_CONTROLLER_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "RfxController.h"
#include "RfxVoidData.h"
#include "RfxStringData.h"
#include "RfxStringsData.h"
#include "RfxIntsData.h"
#include "RtcCapabilitySwitchChecker.h"
#include <telephony/mtk_ril.h>

/*****************************************************************************
 * Class RtcCapabilitySwitchController
 *****************************************************************************/
class RtcCapabilitySwitchController : public RfxController {
    // Required: declare this class
    RFX_DECLARE_CLASS(RtcCapabilitySwitchController);

  public:
    RtcCapabilitySwitchController();
    virtual ~RtcCapabilitySwitchController();
    void notifySetRatDone();

    // Override
  protected:
    virtual bool onHandleRequest(const sp<RfxMessage>& message);
    virtual void onInit();
    virtual bool onHandleResponse(const sp<RfxMessage>& message);
    virtual bool onCheckIfRejectMessage(const sp<RfxMessage>& message, bool isModemPowerOff,
                                        int radioState);
    virtual bool onPreviewMessage(const sp<RfxMessage>& message);
    virtual bool onCheckIfResumeMessage(const sp<RfxMessage>& message);

  private:
    void calculateNewMainSlot(int capability, int slot, char* modemId);
    void processSetMajorSimResponse(const sp<RfxMessage>& message);
    bool isReadyForMessage(const sp<RfxMessage>& message, bool log);
    void powerOffRadio();
    void onRequestRadioOffDone(int slotId);
    void requestSetMajorSim();
    void handleRequestSwitchCapability();
    void setAllLocks();
    void setLock(int slot_id, RfxStatusKeyEnum key);
    int getLockState(int slot_id, RfxStatusKeyEnum key);
    void resetLock(int slot_id, RfxStatusKeyEnum key);
    void backupRadioPower();
    void powerOnRadio();
    void onDefaultDataChanged(RfxStatusKeyEnum key, RfxVariant old_value, RfxVariant new_value);
    void onVolteStateChanged(RfxStatusKeyEnum key, RfxVariant old_value, RfxVariant new_value);
    void registerStatusKeys();
    void unregisterStatusKeys();
    void onStatusKeyChanged(RfxStatusKeyEnum key, RfxVariant old_value, RfxVariant new_value);
    void checkAndSwitchCapability();
    void updateRadioCapability();
    bool isPendingUndoneSwitch();
    int getUndoneSwitch();

  private:
    int m_request_count;
    int m_max_capability;
    int m_new_main_slot;
    int m_close_radio_count;
    int m_new_main_slot_by_modem_id;
    bool m_backup_radio_power[MAX_SIM_COUNT];
    bool m_is_started;
    sp<RfxMessage> m_pending_request;
    RtcCapabilitySwitchChecker* m_checker_controller;
};

#endif /* __RTC_CAPABILITY_SWITCH_CONTROLLER_H__ */
