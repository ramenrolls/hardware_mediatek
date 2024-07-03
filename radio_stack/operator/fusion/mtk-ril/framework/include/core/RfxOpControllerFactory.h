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

#ifndef __RFX_OP_CONTROLLER_FACTORY_H__
#define __RFX_OP_CONTROLLER_FACTORY_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "RfxObject.h"
#include "RfxControllerFactory.h"

/*****************************************************************************
 * Typedef
 *****************************************************************************/

/*****************************************************************************
 * Class RfxOpControllerFactory
 *****************************************************************************/

class RfxOpControllerFactory {
  public:
    RfxOpControllerFactory();

  private:
    static void createControllerInternal(const RfxCreateControllerFuncptr* controller_list,
                                         int length, RfxObject* parent);

  public:
    static const rfx_ctrler_factory_init s_pre_non_slot_controllers[];
    static const rfx_ctrler_factory_init s_slot_controllers[];
    static const rfx_ctrler_factory_init s_non_slot_controllers[];
};
#endif /* __RFX_OP_CONTROLLER_FACTORY_H__ */
