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

#include "RfxTimer.h"
#include "RfxMainThread.h"
#include "RfxAsyncSignal.h"
#include "RfxRootController.h"

#define RFX_LOG_TAG "RfxTimer"

/*****************************************************************************
 * Class TimerHandler
 *****************************************************************************/
class TimerHandler : public RfxMainHandler {
  public:
    TimerHandler(const RfxCallback0& _callback) : callback(_callback) {}
    virtual ~TimerHandler() {}

  protected:
    virtual void onHandleMessage(const Message& message) {
        RFX_UNUSED(message);
        RFX_LOG_D(RFX_LOG_TAG, "onHandleMessage() begin, this = %p", this);

        // callback to slot
        callback.invoke();

        RFX_LOG_D(RFX_LOG_TAG, "onHandleMessage() end, this = %p", this);
    }

  private:
    RfxCallback0 callback;
};

/*****************************************************************************
 * Class RfxTimer
 *****************************************************************************/

TimerHandle RfxTimer::start(const RfxCallback0& callback, nsecs_t time) {
    Looper* looper = RfxMainThread::getLooper().get();

    if (looper != NULL) {
        Message dummy_msg;
        sp<MessageHandler> handler = new TimerHandler(callback);
        RFX_LOG_D(RFX_LOG_TAG, "start(), timer = %p", handler.get());
        looper->sendMessageDelayed(time, handler, dummy_msg);
        return handler;
    } else {
        return TimerHandle(NULL);
    }
}

void RfxTimer::stop(const TimerHandle& timer_handle) {
    Looper* looper = RfxMainThread::getLooper().get();

    if (looper != NULL) {
        RFX_LOG_D(RFX_LOG_TAG, "stop(), timer = %p", timer_handle.get());
        looper->removeMessages(timer_handle);
    }
}
