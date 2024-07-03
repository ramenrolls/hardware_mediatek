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

#ifndef __RTC_OP_RIL_CLIENT_CONTROLLER_H__
#define __RTC_OP_RIL_CLIENT_CONTROLLER_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "client/RtcRilClientController.h"
/*****************************************************************************
 * Class RfxController
 *****************************************************************************/

class RtcOpRilClientController : public RtcRilClientController {
    RFX_DECLARE_CLASS(RtcOpRilClientController);

  public:
    RtcOpRilClientController();
    virtual ~RtcOpRilClientController();

  protected:
    virtual void initRilClient();
};

#endif /* __RTC_OP_RIL_CLIENT_CONTROLLER_H__ */
