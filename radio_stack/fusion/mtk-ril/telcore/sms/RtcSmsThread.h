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

#ifndef __RTC_SMS_THREAD_H__
#define __RTC_SMS_THREAD_H__
/*****************************************************************************
 * Include
 *****************************************************************************/
#include <utils/Looper.h>
#include <utils/threads.h>

/*****************************************************************************
 * Name Space Declaration
 *****************************************************************************/
using ::android::Looper;
using ::android::sp;
using ::android::status_t;
using ::android::Thread;

/*****************************************************************************
 * Class RtcSmsThread
 *****************************************************************************/
/*
 * RtcSmsThread is SMS working thread that is used to communicate with
 * SMSD.
 */
class RtcSmsThread : public Thread {
  public:
    // Constructor
    RtcSmsThread();

    // Destructor
    virtual ~RtcSmsThread();

    // Create the SMS working thread
    static sp<RtcSmsThread> create();

    // Get the Looper of the SMS working thread
    // RETURNS: the looper of the thead
    sp<Looper> getLooper();

    // Override
  protected:
    virtual bool threadLoop();
    virtual status_t readyToRun();

    // Implement
  private:
    // the looper that is attached to this thread
    sp<Looper> m_looper;
};

#endif /* __RTC_SMS_THREAD_H__ */
