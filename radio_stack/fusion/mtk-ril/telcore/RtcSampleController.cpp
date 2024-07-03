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
#include "RtcHelloController.h"
#include "RtcSampleController.h"

/*****************************************************************************
 * Class RfxController
 *****************************************************************************/

RFX_IMPLEMENT_CLASS("RtcSampleController", RtcSampleController, RfxController);

void RtcSampleController::onInit() {
    RfxController::onInit();  // Required: invoke super class implementation

    getStatusManager()->registerStatusChanged(
            RFX_STATUS_KEY_CARD_TYPE,
            RfxStatusChangeCallback(this, &RtcSampleController::onCardTypeChanged));

    // use wp<> to retain other object's pointer,
    // if use *, it would be dangling pointer if the object is deleted by others
    // if use sp<>, it will impact the object's life cycel principle which is,
    // who create the object, who delete it
    m_hello_controller =
            (RtcHelloController*)findController(RFX_OBJ_CLASS_INFO(RtcHelloController));
}

void RtcSampleController::onCardTypeChanged(RfxStatusKeyEnum key, RfxVariant old_value,
                                            RfxVariant value) {
    // do something
    // post emit the signal, it's not sync invoke, that means
    // the real invoke will not happen in postEmit(), it will
    // happen in another callstack, but still in the handler
    // of the same external message (request, URC, response)
    m_something_changed_singal.postEmit(0, 1, 2, 3);
}
