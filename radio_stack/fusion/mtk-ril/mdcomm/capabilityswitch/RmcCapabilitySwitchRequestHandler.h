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

#ifndef __RMC_CAPABILITY_SWITCH_REQUEST_HANDLER_H__
#define __RMC_CAPABILITY_SWITCH_REQUEST_HANDLER_H__

#include "utils/Mutex.h"
#include "RfxBaseHandler.h"
#include "RfxIntsData.h"
#include "RfxStringData.h"
#include "RfxMessageId.h"

using ::android::Mutex;

class RmcCapabilitySwitchRequestHandler : public RfxBaseHandler {
    RFX_DECLARE_HANDLER_CLASS(RmcCapabilitySwitchRequestHandler);

  public:
    RmcCapabilitySwitchRequestHandler(int slot_id, int channel_id);
    virtual ~RmcCapabilitySwitchRequestHandler();
    static void switchChannel(int channel, int old_major_slot, int new_major_slot);
    static void switchChannelByRealId(int channel_id1, int channel_id2);
    static void lockRestartMutex(int channel_id);
    static void unlockRestartMutex(int channel_id);
    static int getSIMInitState();
    static void setSIMInitState(int val);
    static void notifySIMInitDone(int slot_id);

  protected:
    virtual void onHandleRequest(const sp<RfxMclMessage>& msg);

  private:
    void queryBearer();
    void queryActiveMode();
    int queryMainProtocol();
    void sendRadioCapabilityDoneIfNeeded();
    void requestGetRadioCapability(const sp<RfxMclMessage>& msg);
    void requestSetRadioCapability(const sp<RfxMclMessage>& msg);
    void setSimSwitchProp(int old_major_slot, int new_major_slot);
    void switchCapability(int old_major_slot, int new_major_slot);
    void resetRadio();
    bool isVsimEnabledByRid(int rid);
    bool isPersistVsim();
    bool isVsimEnabled();
    int getActiveMode();
    void queryTplusWSupport();
    void queryKeep3GMode();
    void sendEGRAT();
    void queryNoResetSupport();
    bool waitSIMInitDone();
    void shiftCapability(int old_major_slot, int new_major_slot);
    void switchFixedCapability(int slot_a, int slot_b);
    void setCapabilityByConfig(int slot, bool is_major_slot);

  private:
    char rat_properties[MAX_SIM_COUNT][RFX_PROPERTY_VALUE_MAX] = {
            "ro.vendor.mtk_protocol1_rat_config", "persist.vendor.radio.mtk_ps2_rat",
            "persist.vendor.radio.mtk_ps3_rat", "persist.vendor.radio.mtk_ps4_rat"};
    static Mutex s_sim_init_state_mutex;
    static int s_sim_init_state;
    static Mutex s_first_instance_mutex;
    static bool s_first_instance;
};

#endif
