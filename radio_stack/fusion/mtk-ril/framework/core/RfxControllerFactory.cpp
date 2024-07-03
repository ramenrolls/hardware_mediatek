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
#include "RfxControllerFactory.h"
#include "RfxController.h"
#include "RfxRootController.h"
#include "RfxSlotRootController.h"
#include "RfxOpUtils.h"
#include "RfxLog.h"
#include "data/RtcDataAllowController.h"
#include "data/RtcDataController.h"
#include "ims/RtcImsController.h"
#include "ims/config/RtcImsConfigController.h"
#include "ims/RtcImsConferenceController.h"

#include "RfxRilAdapter.h"
#include "RtcWpController.h"
#include "nw/RtcRatSwitchController.h"
#include "nw/RtcNetworkController.h"
#include "nw/RtcNrSwitchController.h"
#include "cc/RtcCallController.h"
#include "capabilityswitch/RtcCapabilityGetController.h"
#include "capabilityswitch/RtcCapabilitySwitchChecker.h"
#include "capabilityswitch/RtcCapabilitySwitchController.h"
#include "sim/RtcCommSimController.h"
#include "sim/RtcCommSimRadioConfigController.h"
#include "ss/RtcSuppServController.h"
#include "oem/RtcOemController.h"
#include "phb/RtcPhbController.h"
#include "phb/RtcPhbSimIoController.h"

#include "cc/RtcEccNumberController.h"

// M: eMBMS
#include "embms/RtcEmbmsControllerProxy.h"
#include "embms/RtcEmbmsAtController.h"

// power-on
#include "power/RtcRadioController.h"
#include "power/RtcModemController.h"
#include "modecontroller/RtcModeSwitchController.h"
// STK
#include "cat/RtcCatController.h"
// M: MwiService
#include "mwi/RtcMobileWifiController.h"
#include "carrierconfig/RtcCarrierConfigController.h"
#include "client/RtcRilClientController.h"
// AGPS
#include "agps/RtcAgpsNSlotController.h"
#include "sms/RtcImsSmsController.h"
#include "sms/RtcSmsNSlotController.h"
// ATCI
#include "atci/RtcAtciController.h"
// VT
#include "vt/RtcVtController.h"
// RTT
#include "ims/RtcImsRttController.h"

/*****************************************************************************
 * Define
 *****************************************************************************/
#define RFX_LOG_TAG "RfxContFactory"

#define RFX_CONTROLLER_CREATION_ENTRY(_className) \
    { _className::createInstance, _className::getClassInfoInstance, false }

Vector<rfx_ctrler_factory_init>* RfxControllerFactory::sPreNonSlotControllerList = NULL;
Vector<rfx_ctrler_factory_init>* RfxControllerFactory::sPreNonSlotOpControllerList = NULL;
Vector<rfx_ctrler_factory_init>* RfxControllerFactory::sSlotControllerList = NULL;
Vector<rfx_ctrler_factory_init>* RfxControllerFactory::sSlotOpControllerList = NULL;
Vector<rfx_ctrler_factory_init>* RfxControllerFactory::sNonSlotControllerList = NULL;
Vector<rfx_ctrler_factory_init>* RfxControllerFactory::sNonSlotOpControllerList = NULL;

/*****************************************************************************
 * Class RfxControllerFactory
 *****************************************************************************/
const rfx_ctrler_factory_init RfxControllerFactory::s_pre_non_slot_controllers[] = {};

const rfx_ctrler_factory_init RfxControllerFactory::s_slot_controllers[] = {
        //    RFX_CONTROLLER_CREATION_ENTRY(RfxHelloController),
        RFX_CONTROLLER_CREATION_ENTRY(RtcRadioController),
        RFX_CONTROLLER_CREATION_ENTRY(RtcRatSwitchController),
        RFX_CONTROLLER_CREATION_ENTRY(RtcNetworkController),
        RFX_CONTROLLER_CREATION_ENTRY(RtcDataController),
        RFX_CONTROLLER_CREATION_ENTRY(RtcCommSimController),
        RFX_CONTROLLER_CREATION_ENTRY(RtcSuppServController),
        RFX_CONTROLLER_CREATION_ENTRY(RtcEccNumberController),
        RFX_CONTROLLER_CREATION_ENTRY(RtcCatController),
        RFX_CONTROLLER_CREATION_ENTRY(RtcImsController),
        RFX_CONTROLLER_CREATION_ENTRY(RtcImsConfigController),
        RFX_CONTROLLER_CREATION_ENTRY(RtcOemController),
        RFX_CONTROLLER_CREATION_ENTRY(RtcCallController),
        RFX_CONTROLLER_CREATION_ENTRY(RtcImsConferenceController),
        RFX_CONTROLLER_CREATION_ENTRY(RtcMobileWifiController),
        RFX_CONTROLLER_CREATION_ENTRY(RtcCarrierConfigController),
        // M: eMBMS
        RFX_CONTROLLER_CREATION_ENTRY(RtcEmbmsControllerProxy),
        RFX_CONTROLLER_CREATION_ENTRY(RtcImsSmsController),
        // ATCI
        RFX_CONTROLLER_CREATION_ENTRY(RtcAtciController),
        RFX_CONTROLLER_CREATION_ENTRY(RtcPhbController),
        RFX_CONTROLLER_CREATION_ENTRY(RtcVtController),
        RFX_CONTROLLER_CREATION_ENTRY(RtcPhbSimIoController),
        // RTT
        RFX_CONTROLLER_CREATION_ENTRY(RtcImsRttController),
};

const rfx_ctrler_factory_init RfxControllerFactory::s_non_slot_controllers[] = {
        RFX_CONTROLLER_CREATION_ENTRY(RtcWpController),
        RFX_CONTROLLER_CREATION_ENTRY(RtcCapabilityGetController),
        RFX_CONTROLLER_CREATION_ENTRY(RtcCapabilitySwitchChecker),
        RFX_CONTROLLER_CREATION_ENTRY(RtcCapabilitySwitchController),
        RFX_CONTROLLER_CREATION_ENTRY(RtcDataAllowController),
        RFX_CONTROLLER_CREATION_ENTRY(RtcModeSwitchController),
        RFX_CONTROLLER_CREATION_ENTRY(RtcModemController),
        RFX_CONTROLLER_CREATION_ENTRY(RtcRilClientController),
        RFX_CONTROLLER_CREATION_ENTRY(RtcAgpsNSlotController),
        RFX_CONTROLLER_CREATION_ENTRY(RtcCommSimRadioConfigController),
        RFX_CONTROLLER_CREATION_ENTRY(RtcSmsNSlotController),
        RFX_CONTROLLER_CREATION_ENTRY(RtcNrSwitchController),
};

void RfxControllerFactory::createControllers() {
    RfxRootController* root = RFX_OBJ_GET_INSTANCE(RfxRootController);

    // TODO
    // RFX_OBJ_GET_INSTANCE(RfxRilAdapter)->initSocketNotifyByFwk();

    RfxSlotRootController* non_slot_controller;

    // create a special slot root controller as non-slot based controllers parent
    RFX_OBJ_CREATE_EX(non_slot_controller, RfxSlotRootController, root, (RFX_SLOT_ID_UNKNOWN));
    root->setSlotRootController(RFX_SLOT_ID_UNKNOWN, non_slot_controller);

    // init controller list vector
    sPreNonSlotControllerList = new Vector<rfx_ctrler_factory_init>();
    sPreNonSlotOpControllerList = new Vector<rfx_ctrler_factory_init>();
    sSlotControllerList = new Vector<rfx_ctrler_factory_init>();
    sSlotOpControllerList = new Vector<rfx_ctrler_factory_init>();
    sNonSlotControllerList = new Vector<rfx_ctrler_factory_init>();
    sNonSlotOpControllerList = new Vector<rfx_ctrler_factory_init>();

    int numOfPreOm = sizeof(s_pre_non_slot_controllers) / sizeof(rfx_ctrler_factory_init);
    for (int i = 0; i < numOfPreOm; i++) {
        sPreNonSlotControllerList->add(s_pre_non_slot_controllers[i]);
    }

    const rfx_ctrler_factory_init* preControllerList = RfxOpUtils::getPreNonSlotOpCtlerList();
    int numOfPreOp = RfxOpUtils::getNumOfPreNonSlotOpCtler();
    for (int i = 0; i < numOfPreOp; i++) {
        sPreNonSlotOpControllerList->add(preControllerList[i]);
    }

    int numOfOm = sizeof(s_slot_controllers) / sizeof(rfx_ctrler_factory_init);
    for (int i = 0; i < numOfOm; i++) {
        sSlotControllerList->add(s_slot_controllers[i]);
    }

    const rfx_ctrler_factory_init* controllerList = RfxOpUtils::getSlotOpCtlerList();
    int numOfOp = RfxOpUtils::getNumOfSlotOpCtler();
    for (int i = 0; i < numOfOp; i++) {
        sSlotOpControllerList->add(controllerList[i]);
    }

    int numOfNonSlotOm = sizeof(s_non_slot_controllers) / sizeof(rfx_ctrler_factory_init);
    for (int i = 0; i < numOfNonSlotOm; i++) {
        sNonSlotControllerList->add(s_non_slot_controllers[i]);
    }

    const rfx_ctrler_factory_init* nonSlotControllerList = RfxOpUtils::getNonSlotOpCtlerList();
    int numOfNonSlotOp = RfxOpUtils::getNumOfNonSlotOpCtler();
    for (int i = 0; i < numOfNonSlotOp; i++) {
        sNonSlotOpControllerList->add(nonSlotControllerList[i]);
    }

    RFX_LOG_D(RFX_LOG_TAG,
              "numOfOmSlot:%d, numofOpSlot:%d, numOfNonSlotOm: %d,\
numOfNonSlotOp: %d",
              numOfOm, numOfOp, numOfNonSlotOm, numOfNonSlotOp);

    // Replace OM controller with OP controller extending from OM controller
    mergeControllerFactorList();

    // create some non-slot based controllers firstly
    createControllerInternal(sPreNonSlotControllerList, numOfPreOm, non_slot_controller);
    createControllerInternal(sPreNonSlotOpControllerList, numOfPreOp, non_slot_controller);

    // create slot controllers
    RfxSlotRootController* slot_controller;
    for (int i = 0; i < RFX_SLOT_COUNT; i++) {
        RFX_OBJ_CREATE_EX(slot_controller, RfxSlotRootController, root, (i));
        root->setSlotRootController(i, slot_controller);
        createControllerInternal(sSlotControllerList, numOfOm, slot_controller);
        createControllerInternal(sSlotOpControllerList, numOfOp, slot_controller);
    }

    // create non-slot controllers
    createControllerInternal(sNonSlotControllerList, numOfNonSlotOm, non_slot_controller);
    createControllerInternal(sNonSlotOpControllerList, numOfNonSlotOp, non_slot_controller);

    delete sPreNonSlotControllerList;
    delete sPreNonSlotOpControllerList;
    delete sSlotControllerList;
    delete sSlotOpControllerList;
    delete sNonSlotControllerList;
    delete sNonSlotOpControllerList;
}

void RfxControllerFactory::createControllerInternal(
        const Vector<rfx_ctrler_factory_init>* controller_list, int length, RfxObject* parent) {
    rfx_ctrler_factory_init ctrlerInit;
    for (int i = 0; i < length; i++) {
        ctrlerInit = (rfx_ctrler_factory_init)(controller_list->itemAt(i));
        RfxClassInfo* classInfo = ctrlerInit.getControClassInfo();
        if (ctrlerInit.skip == false) {
            RFX_LOG_D(RFX_LOG_TAG, "createControllerInternalEx init %s", classInfo->getClassName());
            const RfxCreateControllerFuncptr ptr = ctrlerInit.getCreateControllerFuncptr;
            if (ptr != NULL) {
                ptr(parent);
            }
        }
    }
}

void RfxControllerFactory::mergeControllerFactorList() {
    // Merge slot controller, replace platform controller with op controller extends from it
    int NumOfPreOmContro = sPreNonSlotControllerList->size();
    int NumOfPreOpContro = sPreNonSlotOpControllerList->size();
    for (int i = 0; i < NumOfPreOmContro; i++) {
        rfx_ctrler_factory_init parent = sPreNonSlotControllerList->itemAt(i);
        RfxClassInfo* parentClassInfo = parent.getControClassInfo();
        for (int j = 0; j < NumOfPreOpContro; j++) {
            rfx_ctrler_factory_init opController = sPreNonSlotOpControllerList->itemAt(j);
            RfxClassInfo* classInfo = opController.getControClassInfo();
            if (classInfo->isKindOf(parentClassInfo)) {
                sPreNonSlotControllerList->replaceAt(opController, i);
                opController.skip = true;
                sPreNonSlotOpControllerList->replaceAt(opController, j);
                RFX_LOG_I(RFX_LOG_TAG, "%s merged", parentClassInfo->getClassName());
                break;
            }
        }
    }

    int NumOfOmContro = sSlotControllerList->size();
    int NumOfOpContro = sSlotOpControllerList->size();
    for (int i = 0; i < NumOfOmContro; i++) {
        rfx_ctrler_factory_init parent = sSlotControllerList->itemAt(i);
        RfxClassInfo* parentClassInfo = parent.getControClassInfo();
        for (int j = 0; j < NumOfOpContro; j++) {
            rfx_ctrler_factory_init opController = sSlotOpControllerList->itemAt(j);
            RfxClassInfo* classInfo = opController.getControClassInfo();
            if (classInfo->isKindOf(parentClassInfo)) {
                sSlotControllerList->replaceAt(opController, i);
                opController.skip = true;
                sSlotOpControllerList->replaceAt(opController, j);
                RFX_LOG_I(RFX_LOG_TAG, "%s merged", parentClassInfo->getClassName());
                break;
            }
        }
    }

    // Merge non-slot controller, replace platform controller with op controller extends from it
    int NumOfNonOmContro = sNonSlotControllerList->size();
    int NumOfNonOpContro = sNonSlotOpControllerList->size();
    for (int i = 0; i < NumOfNonOmContro; i++) {
        rfx_ctrler_factory_init parent = sNonSlotControllerList->itemAt(i);
        RfxClassInfo* parentClassInfo = parent.getControClassInfo();
        for (int j = 0; j < NumOfNonOpContro; j++) {
            rfx_ctrler_factory_init opController = sNonSlotOpControllerList->itemAt(j);
            RfxClassInfo* classInfo = opController.getControClassInfo();
            if (classInfo->isKindOf(parentClassInfo)) {
                sNonSlotControllerList->replaceAt(opController, i);
                opController.skip = true;
                sNonSlotOpControllerList->replaceAt(opController, j);
                RFX_LOG_I(RFX_LOG_TAG, "%s merged", parentClassInfo->getClassName());
                break;
            }
        }
    }
}
