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

#ifndef __RFX_CONTROLLER_FACTORY_H__
#define __RFX_CONTROLLER_FACTORY_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "RfxObject.h"

/*****************************************************************************
 * Typedef
 *****************************************************************************/

typedef RfxObject* (*RfxCreateControllerFuncptr)(RfxObject* parent);
typedef RfxClassInfo* (*RfxControClassInfo)();

typedef struct _rfx_ctrler_factory_initt {
    RfxObject* (*getCreateControllerFuncptr)(RfxObject* parent);
    RfxClassInfo* (*getControClassInfo)();
    bool skip;
} rfx_ctrler_factory_init;

/*****************************************************************************
 * Class RfxControllerFactory
 *****************************************************************************/

class RfxControllerFactory {
  public:
    static void createControllers();

  private:
    static void createControllerInternal(const Vector<rfx_ctrler_factory_init>* controller_list,
                                         int length, RfxObject* parent);
    static void mergeControllerFactorList();

  private:
    static Vector<rfx_ctrler_factory_init>* sPreNonSlotControllerList;
    static Vector<rfx_ctrler_factory_init>* sPreNonSlotOpControllerList;
    static Vector<rfx_ctrler_factory_init>* sSlotControllerList;
    static Vector<rfx_ctrler_factory_init>* sSlotOpControllerList;
    static Vector<rfx_ctrler_factory_init>* sNonSlotControllerList;
    static Vector<rfx_ctrler_factory_init>* sNonSlotOpControllerList;
    static const rfx_ctrler_factory_init s_slot_controllers[];
    static const rfx_ctrler_factory_init s_non_slot_controllers[];
    static const rfx_ctrler_factory_init s_pre_non_slot_controllers[];
};

#endif /* __RFX_CONTROLLER_FACTORY_H__ */
