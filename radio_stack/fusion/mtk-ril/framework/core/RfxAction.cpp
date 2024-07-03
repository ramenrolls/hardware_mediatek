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

#include "RfxAction.h"

#define RFX_LOG_TAG "RfxAction"
/*****************************************************************************
 * Class RfxAction
 *****************************************************************************/
#ifdef RFX_OBJ_DEBUG
Vector<RfxDebugInfo*>* RfxAction::s_root_action_debug_info = NULL;

RfxAction::RfxAction() {
    if (RfxDebugInfo::isRfxDebugInfoEnabled()) {
        m_debug_info = new RfxDebugInfo(static_cast<IRfxDebugLogger*>(this), (void*)this);
        if (s_root_action_debug_info == NULL) {
            s_root_action_debug_info = new Vector<RfxDebugInfo*>();
        }
        s_root_action_debug_info->add(m_debug_info);
    } else {
        m_debug_info = NULL;
    }
}

RfxAction::~RfxAction() {
    if (m_debug_info != NULL) {
        size_t size = s_root_action_debug_info->size();
        for (size_t i = 0; i < size; i++) {
            const RfxDebugInfo* item = s_root_action_debug_info->itemAt(i);
            if (item == m_debug_info) {
                s_root_action_debug_info->removeAt(i);
                break;
            }
        }
        delete (m_debug_info);
        m_debug_info = NULL;
    }
}

void RfxAction::dumpActionList() {
    size_t size = RfxAction::s_root_action_debug_info->size();
    RFX_LOG_D(RFX_DEBUG_INFO_TAG, "dumpActionList() Action count is %zu", size);
    for (size_t i = 0; i < size; i++) {
        const RfxDebugInfo* item = RfxAction::s_root_action_debug_info->itemAt(i);
        item->getLogger()->dump();
    }
}

void RfxAction::dumpActionConstructionCallStack(void* action_address) {
    size_t size = RfxAction::s_root_action_debug_info->size();
    for (size_t i = 0; i < size; i++) {
        const RfxDebugInfo* item = RfxAction::s_root_action_debug_info->itemAt(i);
        if (item->getUserData() == action_address) {
            RFX_LOG_D(RFX_DEBUG_INFO_TAG,
                      "dumpActionConstructionCallStack() Found action, start to dump callstack");
            item->dump();
            return;
        }
    }
    RFX_LOG_D(RFX_DEBUG_INFO_TAG, "dumpActionConstructionCallStack() Can't find action %p",
              action_address);
}

#endif  //#ifdef RFX_OBJ_DEBUG
