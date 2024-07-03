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
#include "RfxTestSuitController.h"
#include "RfxTestBasicController.h"
#include "RfxMainThread.h"
#include "RfxLog.h"
#include "RfxVoidData.h"

#define RFX_TEST_SUIT_LOG_TAG "RFX_TEST_SUIT_CONTROLLER"

#define RFX_TEST_CONTROLLER_ENTRY(_className) _className::createInstance

/*****************************************************************************
 * Class RfxTestSuitController
 *****************************************************************************/

RFX_IMPLEMENT_CLASS("RfxTestSuitController", RfxTestSuitController, RfxController);
RFX_OBJ_IMPLEMENT_SINGLETON_CLASS(RfxTestSuitController);

RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxVoidData, RIL_REQUEST_LOCAL_TEST);

const RfxCreateControllerFuncPtr RfxTestSuitController::s_test_controllers[] = {
        //    RFX_TEST_CONTROLLER_ENTRY(RfxTestHelloController)
};

void RfxTestSuitController::onInit() {
    RfxController::onInit();
    if (isEnableTest()) {
        RFX_LOG_D(RFX_TEST_SUIT_LOG_TAG, "onInit");
        const int request_id_list[] = {RIL_REQUEST_LOCAL_TEST};
        registerToHandleRequest(request_id_list, 1);
        // Assume we have array to send
        RfxMainThread::enqueueMessage(
                RfxMessage::obtainRequest(0, RIL_REQUEST_LOCAL_TEST, RfxVoidData()));
    }
}

bool RfxTestSuitController::onHandleRequest(const sp<RfxMessage>& message) {
    RFX_UNUSED(message);
    uint32_t test_controller_size = sizeof(s_test_controllers) / sizeof(RfxCreateControllerFuncPtr);
    RFX_LOG_D(RFX_TEST_SUIT_LOG_TAG, "onHandleRequest, size: %d", test_controller_size);
    if (m_index < test_controller_size) {
        m_testObj = (RfxTestBasicController*)s_test_controllers[m_index](this);
        m_testObj->run();
    }
    return true;
}

bool RfxTestSuitController::isEnableTest() { return mTestSwitcher; }

void RfxTestSuitController::checkSuccessAndEnqueueNext() {
    uint32_t test_controller_size = sizeof(s_test_controllers) / sizeof(RfxCreateControllerFuncPtr);
    RFX_LOG_D(RFX_TEST_SUIT_LOG_TAG, "checkSuccessAndEnqueueNext, size: %d", test_controller_size);
    if (m_index < test_controller_size) {
        m_testObj->checkSuccess();
        m_index++;
        RFX_OBJ_CLOSE(m_testObj);
    }
    if (m_index < test_controller_size) {
        RfxMainThread::enqueueMessage(
                RfxMessage::obtainRequest(0, RIL_REQUEST_LOCAL_TEST, RfxVoidData()));
    }
}
