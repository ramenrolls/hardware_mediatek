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

#ifndef __RFX_TEST_SUIT_CONTROLLER_H__
#define __RFX_TEST_SUIT_CONTROLLER_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "RfxController.h"
#include "RfxTestBasicController.h"

typedef RfxObject* (*RfxCreateControllerFuncPtr)(RfxObject* parent);

#define RIL_REQUEST_LOCAL_TEST 9999

/*****************************************************************************
 * Class RfxTestSuitController
 *****************************************************************************/

class RfxTestSuitController : public RfxController {
    RFX_DECLARE_CLASS(RfxTestSuitController);
    RFX_OBJ_DECLARE_SINGLETON_CLASS(RfxTestSuitController);

    // Constructor / Destructor
  public:
    RfxTestSuitController() : m_testObj(NULL) {}

    virtual ~RfxTestSuitController() {}

    // Override
  protected:
    virtual bool onHandleRequest(const sp<RfxMessage>& message);

    virtual void onInit();

  public:
    bool isEnableTest();
    void checkSuccessAndEnqueueNext();

  public:
    static const RfxCreateControllerFuncPtr s_test_controllers[];

  private:
    bool mTestSwitcher = false;
    uint32_t m_index = 0;
    RfxTestBasicController* m_testObj;
};

#endif /* __RFX_TEST_SUIT_CONTROLLER_H__ */
