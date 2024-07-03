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

#include <stdlib.h>
#include "RfxLog.h"
#include "RfxDebugInfo.h"
#include "RfxObject.h"
#include "RfxMessage.h"
#include "RfxAction.h"
#include "rfx_properties.h"

/*****************************************************************************
 * Define
 *****************************************************************************/
#define RFX_PROPERTY_DEBUG_INFO_ENABLED "persist.vendor.radio.rfxdbg.enabled"
#define RFX_PROPERTY_DUMP_OBJ_TREE "persist.vendor.radio.dumpobjtree"
#define RFX_PROPERTY_DUMP_OBJ_CREATION_STACK "persist.vendor.radio.dumpobjstack"
#define RFX_PROPERTY_DUMP_MSG_LIST "persist.vendor.radio.dumpmsglist"
#define RFX_PROPERTY_DUMP_ACT_LIST "persist.vendor.radio.dumpactlist"
#define RFX_PROPERTY_DUMP_ACT_CREATION_STACK "persist.vendor.radio.dumpactstack"

#define RFX_LOG_TAG "RfxDebugInfo"
/*****************************************************************************
 * Class RfxDebugInfo
 *****************************************************************************/

bool RfxDebugInfo::s_rfx_debug_info_enabled = false;

void RfxDebugInfo::dump(int level) const {
    if (m_logger) {
        m_logger->dump(level);
    }
    m_stack.print(*((Printer*)this));
}

void RfxDebugInfo::printLine(const char* string) { RFX_LOG_D(RFX_DEBUG_INFO_TAG, "%s", string); }

#ifdef RFX_OBJ_DEBUG
bool RfxDebugInfo::isRfxDebugInfoEnabled() { return s_rfx_debug_info_enabled; }

void RfxDebugInfo::updateDebugInfoSwitcher() {
    char property_value[RFX_PROPERTY_VALUE_MAX] = {0};

    rfx_property_get(RFX_PROPERTY_DEBUG_INFO_ENABLED, property_value, "0");

    int value = atoi(property_value);

    RFX_LOG_D(RFX_LOG_TAG, "Debug info switcher property_value = %s, value = %d", property_value,
              value);

    if (value == 1) {
        s_rfx_debug_info_enabled = true;
    } else {
        s_rfx_debug_info_enabled = false;
    }
}

void RfxDebugInfo::dumpIfNeed() {
    if (!isRfxDebugInfoEnabled()) {
        return;
    }

    char property_value[RFX_PROPERTY_VALUE_MAX] = {0};

    // dump RfxObject tree
    rfx_property_get(RFX_PROPERTY_DUMP_OBJ_TREE, property_value, "0");

    int value = atoi(property_value);

    RFX_LOG_D(RFX_LOG_TAG, "Dump RfxObject tree property_value = %s, value = %d", property_value,
              value);

    if (value == 1) {
        RfxObject::dumpAllObjTree();
        rfx_property_set(RFX_PROPERTY_DUMP_OBJ_TREE, "0");
    }

    // dump RfxObject creation stack of specific object address
    rfx_property_get(RFX_PROPERTY_DUMP_OBJ_CREATION_STACK, property_value, "0");

    long value16 = strtol(property_value, NULL, 16);

    RFX_LOG_D(RFX_LOG_TAG, "Dump RfxObject creation stack property_value = %s, value = %p",
              property_value, (void*)value16);

    if (value16 != 0) {
        RfxObject::dumpObjConstructionCallStack((void*)value16);
        rfx_property_set(RFX_PROPERTY_DUMP_OBJ_CREATION_STACK, "0");
    }

    // dump RfxMessage list
    rfx_property_get(RFX_PROPERTY_DUMP_MSG_LIST, property_value, "0");

    value = atoi(property_value);

    RFX_LOG_D(RFX_LOG_TAG, "Dump Msg list property_value = %s, value = %d", property_value, value);

    if (value == 1) {
        RfxMessage::dumpMsgList();
        rfx_property_set(RFX_PROPERTY_DUMP_MSG_LIST, "0");
    }

    // dump RfxAction list
    rfx_property_get(RFX_PROPERTY_DUMP_ACT_LIST, property_value, "0");

    value = atoi(property_value);

    RFX_LOG_D(RFX_LOG_TAG, "Dump RfxAction list property_value = %s, value = %d", property_value,
              value);

    if (value == 1) {
        RfxAction::dumpActionList();
        rfx_property_set(RFX_PROPERTY_DUMP_ACT_LIST, "0");
    }

    // dump RfxAction creation stack of specific object address
    rfx_property_get(RFX_PROPERTY_DUMP_ACT_CREATION_STACK, property_value, "0");

    value16 = strtol(property_value, NULL, 16);

    RFX_LOG_D(RFX_LOG_TAG, "Dump RfxAction creation stack property_value = %s, value = %p",
              property_value, (void*)value16);

    if (value16 != 0) {
        RfxAction::dumpActionConstructionCallStack((void*)value16);
        rfx_property_set(RFX_PROPERTY_DUMP_ACT_CREATION_STACK, "0");
    }
}
#endif  //#ifdef RFX_OBJ_DEBUG
