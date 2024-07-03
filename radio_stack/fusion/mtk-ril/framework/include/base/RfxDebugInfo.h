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

#ifndef __RFX_DEBUG_INFO_H__
#define __RFX_DEBUG_INFO_H__

/*****************************************************************************
 * Include
 *****************************************************************************/

#include "utils/CallStack.h"
#include "utils/Printer.h"
#include "RfxDefs.h"

using ::android::CallStack;
using ::android::Printer;

#define RFX_DEBUG_INFO_TAG "RfxObjDebugInfo"

/*****************************************************************************
 * Interface IRfxDebugLogger
 *****************************************************************************/
class IRfxDebugLogger {
  public:
    virtual void dump(int level = 0) const = 0;

    virtual ~IRfxDebugLogger() {}
};

/*****************************************************************************
 * Class RfxDebugInfo
 *****************************************************************************/

class RfxDebugInfo : public IRfxDebugLogger, public Printer {
  public:
    RfxDebugInfo() : m_logger(NULL), m_user_data(NULL) { m_stack.update(); }

    RfxDebugInfo(IRfxDebugLogger* logger, void* user_data)
        : m_logger(logger), m_user_data(user_data) {
        m_stack.update();
    }

    virtual void dump(int level = 0) const;

    IRfxDebugLogger* getLogger() const { return m_logger; }

    void* getUserData() const { return m_user_data; }

    virtual void printLine(const char* string = "");

#ifdef RFX_OBJ_DEBUG
    static bool isRfxDebugInfoEnabled();

    static void updateDebugInfoSwitcher();

    static void dumpIfNeed();
#endif

  private:
    IRfxDebugLogger* m_logger;
    void* m_user_data;
    CallStack m_stack;

    static bool s_rfx_debug_info_enabled;
};

#endif  // __RFX_DEBUG_INFO_H__
