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

#ifndef __RFX_OP_UTILS__
#define __RFX_OP_UTILS__

/*****************************************************************************
 * Include
 *****************************************************************************/

#include <string.h>
#include <RfxLog.h>
#include "RfxControllerFactory.h"
/*****************************************************************************
 * Class RfxViaUtils
 *****************************************************************************/

class RfxOpUtils {
  public:
    static void initOpLibrary();
    static void* getOpHandler();
    static const rfx_ctrler_factory_init* getPreNonSlotOpCtlerList();
    static const rfx_ctrler_factory_init* getSlotOpCtlerList();
    static const rfx_ctrler_factory_init* getNonSlotOpCtlerList();
    static int getNumOfPreNonSlotOpCtler();
    static int getNumOfSlotOpCtler();
    static int getNumOfNonSlotOpCtler();
    static int getOpMsgIdFromRequestId(int requestId);
    static int getOpRequestIdFromMsgId(int msgId);
    static void* getSimOpHandler(int type, int slotId, int channelId);
    static void* getDcImsOpHandler(int slotId, int channelId, void* pRmcDcPdnManager);

  private:
    static void* sDlOpHandler;
};

#endif /* __RFX_OP_UTILS__ */
