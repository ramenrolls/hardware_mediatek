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

#ifndef __RFX_TEST_BASIC_CONTROLLER_H__
#define __RFX_TEST_BASIC_CONTROLLER_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "RfxController.h"

/*****************************************************************************
 * Class RfxTestBasicController
 *****************************************************************************/

class RfxTestBasicController : public RfxController {
    RFX_DECLARE_CLASS(RfxTestBasicController);

    // Constructor / Destructor
  public:
    RfxTestBasicController() {}

    virtual ~RfxTestBasicController() {}

  public:
    void setUp(sp<RfxMessage>& message);
    void tearDown(sp<RfxMessage>& message);
    void run();
    void checkSuccess();
};

#endif /* __RFX_TEST_BASIC_CONTROLLER_H__ */
