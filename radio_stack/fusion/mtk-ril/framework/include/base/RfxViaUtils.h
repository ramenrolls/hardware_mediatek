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

#ifndef __RFX_VIA_UTILS__
#define __RFX_VIA_UTILS__

/*****************************************************************************
 * Include
 *****************************************************************************/

#include <rfx_properties.h>
#include <stdlib.h>
#include <string.h>
#include <RfxLog.h>
#include "ViaBaseHandler.h"

/*****************************************************************************
 * Class RfxViaUtils
 *****************************************************************************/

class RfxViaUtils {
  public:
    static void initViaHandler();
    static ViaBaseHandler* getViaHandler();
    static void deInitViaHandler();

  private:
    static ViaBaseHandler* m_viaHandler;
    static void* m_dlHandler;
};

#endif /* __RFX_VIA_UTILS__ */
