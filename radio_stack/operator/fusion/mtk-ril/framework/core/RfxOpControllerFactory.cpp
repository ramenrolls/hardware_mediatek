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
#include "RfxOpControllerFactory.h"
#include "RfxController.h"
#include "RfxRootController.h"
#include "RfxSlotRootController.h"

#include "RfxRilAdapter.h"
#include "RfxLog.h"

#include "RtcCommSimOpController.h"
#include "RtcOpOemController.h"
#include "client/RtcOpRilClientController.h"
#include "RtcOpImsController.h"
#include "RtcOpCallController.h"
#include "RtcOpCapabilitySwitchChecker.h"
#include "RtcOpSuppServController.h"
#include <data/RtcOpDataController.h>
#include <nw/RtcOpRatSwitchController.h>

/*****************************************************************************
 * Define
 *****************************************************************************/
#define RFX_LOG_TAG "RfxOpContFac"

#define RFX_CONTROLLER_CREATION_ENTRY(_className) \
    { _className::createInstance, _className::getClassInfoInstance, false }

/*****************************************************************************
 * Class RfxOpControllerFactory
 *****************************************************************************/
const rfx_ctrler_factory_init RfxOpControllerFactory::s_pre_non_slot_controllers[] = {};

const rfx_ctrler_factory_init RfxOpControllerFactory::s_slot_controllers[] = {
        // RFX_CONTROLLER_CREATION_ENTRY(RtcOpOemController) // Example for reference
        RFX_CONTROLLER_CREATION_ENTRY(RtcOpDataController),
        RFX_CONTROLLER_CREATION_ENTRY(RtcOpImsController),
        RFX_CONTROLLER_CREATION_ENTRY(RtcOpCallController),
        RFX_CONTROLLER_CREATION_ENTRY(RtcCommSimOpController),
        RFX_CONTROLLER_CREATION_ENTRY(RtcOpSuppServController),
        RFX_CONTROLLER_CREATION_ENTRY(RtcOpRatSwitchController)};

const rfx_ctrler_factory_init RfxOpControllerFactory::s_non_slot_controllers[] = {
        RFX_CONTROLLER_CREATION_ENTRY(RtcOpRilClientController),
        RFX_CONTROLLER_CREATION_ENTRY(RtcOpCapabilitySwitchChecker),
};

extern "C" const rfx_ctrler_factory_init* getPreNonSlotOpCtlerList() {
    RFX_LOG_D(RFX_LOG_TAG, "getPreNonSlotOpCtlerList");
    return &RfxOpControllerFactory::s_pre_non_slot_controllers[0];
}

extern "C" const rfx_ctrler_factory_init* getSlotOpCtlerList() {
    RFX_LOG_D(RFX_LOG_TAG, "getSlotOpCtlerList");
    return &RfxOpControllerFactory::s_slot_controllers[0];
}

extern "C" const rfx_ctrler_factory_init* getNonSlotOpCtlerList() {
    RFX_LOG_D(RFX_LOG_TAG, "getNonSlotOpCtlerList");
    return &RfxOpControllerFactory::s_non_slot_controllers[0];
}

extern "C" int getNumOfPreNonSlotOpCtler() {
    int num = sizeof(RfxOpControllerFactory::s_pre_non_slot_controllers) /
              sizeof(rfx_ctrler_factory_init);
    RFX_LOG_D(RFX_LOG_TAG, "getNumOfPreNonSlotOpCtler num:%d", num);
    return num;
}

extern "C" int getNumOfSlotOpCtler() {
    int num = sizeof(RfxOpControllerFactory::s_slot_controllers) / sizeof(rfx_ctrler_factory_init);
    RFX_LOG_D(RFX_LOG_TAG, "getNumOfSlotOpCtler num:%d", num);
    return num;
}

extern "C" int getNumOfNonSlotOpCtler() {
    int num = sizeof(RfxOpControllerFactory::s_non_slot_controllers) /
              sizeof(rfx_ctrler_factory_init);
    RFX_LOG_D(RFX_LOG_TAG, "getNumOfNonSlotOpCtler num:%d", num);
    return num;
}
