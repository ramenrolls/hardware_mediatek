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

#ifndef __RFX_TIMER_H__
#define __RFX_TIMER_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "RfxSignal.h"
#include "utils/Timers.h"
#include "utils/RefBase.h"
#include "utils/StrongPointer.h"
#include "utils/Looper.h"

using ::android::Looper;
using ::android::MessageHandler;
using ::android::RefBase;
using ::android::sp;

/*****************************************************************************
 * Class RfxTimer
 *****************************************************************************/

typedef sp<MessageHandler> TimerHandle;

class RfxTimer {
  public:
    static TimerHandle start(const RfxCallback0& callback, nsecs_t time);

    static void stop(const TimerHandle& timer_handle);
};

#endif /* __RFX_TIMER_H__ */
