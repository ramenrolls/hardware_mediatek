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

#ifndef __RTC_CAPABILITY_SWITCH_CHECKER_H__
#define __RTC_CAPABILITY_SWITCH_CHECKER_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include <telephony/mtk_ril.h>
#include "RfxIntsData.h"
#include "RfxController.h"
#include "RfxDefs.h"
#include "RfxStatusManager.h"
#include "RfxStringData.h"
#include "RfxStringsData.h"
#include "RfxVoidData.h"

/*****************************************************************************
 * Define
 *****************************************************************************/
#define ENHANCEMENT_T_PLUS_T (0x01)
#define ENHANCEMENT_T_PLUS_W (0x02)
#define ENHANCEMENT_T_PLUS_C (0x04)
#define ENHANCEMENT_W_PLUS_C (0x08)
#define ENHANCEMENT_W_PLUS_W (0x10)
#define ENHANCEMENT_W_PLUS_NA (0x20)
#define ENHANCEMENT_C_PLUS_C (0x40)

#define CURRENT_SLOT (-2)
#define UNKNOWN_SLOT (-1)
#define SLOT_0 (0)
#define SLOT_1 (1)
#define SLOT_2 (2)

#define SIM_ON (11)

/*****************************************************************************
 * Class RtcCapabilitySwitchChecker
 *****************************************************************************/
class RtcCapabilitySwitchChecker : public RfxController {
    // Required: declare this class
    RFX_DECLARE_CLASS(RtcCapabilitySwitchChecker);

  public:
    RtcCapabilitySwitchChecker();
    virtual ~RtcCapabilitySwitchChecker();

    bool isInCalling(bool log);
    bool isInPukLock(bool log);
    bool isReadyForSwitch(bool log);
    virtual bool isSkipCapabilitySwitch(int new_major_slot);
    virtual int getHigherPrioritySlot();
    bool isCapabilitySwitching();

  protected:
    bool isCdmaOnlySim(int slot_id);
    bool isCdmaDualModeSim(int slot_id);
    bool isCommonGsmSim(int slot_id);
    bool imsiReady(bool log);
    const String8& getImsi(int slot_id);
    bool isSupportSimSwitchEnhancement(int sim_type);
    bool isLteNetworkType(int nw_type);

  private:
    bool radioLockReady(bool log);
    bool eccStateReady(bool log);
    bool modemOffStateReady(bool log);
    bool simStateReady(bool log);
    bool waitingFlagReady(bool log);
    void getWaiveEnhancementConfig();

  protected:
    long int m_waive_enhance_config;
    // add one more status manage for no-slot controllers
    RfxStatusManager* m_status_managers[MAX_RFX_SLOT_ID + 1];
};

#endif /* __RTC_CAPABILITY_SWITCH_CHECKER_H__ */
