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

#ifndef __RFX_CHANNEL_CONTEXT_H__
#define __RFX_CHANNEL_CONTEXT_H__

#include "utils/Condition.h"
#include "utils/Mutex.h"
#include "utils/RefBase.h"
#include "RfxDefs.h"
#include "RfxLog.h"
#include "RfxAtResponse.h"

using ::android::Condition;
using ::android::Mutex;
using ::android::RefBase;
using ::android::sp;

class RfxChannelContext {
  public:
    RfxChannelContext();

  public:
    void setType(int type) { m_type = type; }
    int getType() const { return m_type; }
    void setResponse(sp<RfxAtResponse> response) { m_response = response; }
    sp<RfxAtResponse> getResponse() const { return m_response; }
    void setNeedWaitRestartCondition(bool need) { m_needWaitRestartCondition = need; }
    bool getNeedWaitRestartCondition() const { return m_needWaitRestartCondition; }

  public:
    Mutex m_commandMutex;
    Mutex m_readerMutex;
    Mutex m_restartMutex;
    Condition m_commandCondition;
    Condition m_restartCondition;

  private:
    int m_readerClose;
    int m_type;  // channel usage. for request or urc
    sp<RfxAtResponse> m_response;
    bool m_needWaitRestartCondition;
};

#endif
