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

#ifndef __RTC_OP_CAPABILITY_SWITCH_CHECKER_H__
#define __RTC_OP_CAPABILITY_SWITCH_CHECKER_H__

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
#include "RtcCapabilitySwitchChecker.h"

/*****************************************************************************
 * Define
 *****************************************************************************/

/*****************************************************************************
 * Class RtcOpCapabilitySwitchChecker
 *****************************************************************************/
class RtcOpCapabilitySwitchChecker : public RtcCapabilitySwitchChecker {
    // Required: declare this class
    RFX_DECLARE_CLASS(RtcOpCapabilitySwitchChecker);

  public:
    RtcOpCapabilitySwitchChecker();
    virtual ~RtcOpCapabilitySwitchChecker();

    // Override
  protected:
    virtual int getHigherPrioritySlot();
    virtual bool isSkipCapabilitySwitch(int new_major_slot);

  private:
    bool isSatisfyOperatorRules(int new_major_slot);
    bool isSatisfyOp01Rules(int new_major_slot);
};

#endif /* __RTC_OP_CAPABILITY_SWITCH_CHECKER_H__ */
