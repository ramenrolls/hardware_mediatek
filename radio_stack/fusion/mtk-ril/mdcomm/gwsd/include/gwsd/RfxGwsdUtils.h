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

#ifndef __RFX_GWSD_UTILS__
#define __RFX_GWSD_UTILS__

/*****************************************************************************
 * Include
 *****************************************************************************/

#include <rfx_properties.h>
#include <stdlib.h>
#include <string.h>
#include <RfxLog.h>
#include <RfxRilUtils.h>
#include "RfxGwsdSettingBaseHandler.h"
#include "RfxGwsdCallControlBaseHandler.h"

/*****************************************************************************
 * Class RfxGwsdUtils
 *****************************************************************************/

class RfxGwsdUtils {
  public:
    static void initHandler();
    static RfxGwsdSettingBaseHandler* getSettingHandler();
    static RfxGwsdCallControlBaseHandler* getCallControlHandler();
    static void deInitHandler();

  private:
    static RfxGwsdSettingBaseHandler* m_settingHandler;
    static RfxGwsdCallControlBaseHandler* m_callcontrolHandler;
    static void* m_dlHandler;
};

#endif /* __RFX_GWSD_UTILS__ */
