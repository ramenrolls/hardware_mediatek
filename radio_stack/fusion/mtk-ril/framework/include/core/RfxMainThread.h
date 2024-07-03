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

#ifndef __RFX_MAIN_THREAD_H__
#define __RFX_MAIN_THREAD_H__

/*****************************************************************************
 * Include
 *****************************************************************************/

#include <utils/Looper.h>
#include <utils/threads.h>
#include "utils/Timers.h"
#include "RfxMessage.h"

using ::android::Looper;
using ::android::Message;
using ::android::MessageHandler;
using ::android::sp;
using ::android::Thread;

/*****************************************************************************
 * Class RfxMainHandler
 *****************************************************************************/

class RfxMainHandler : public MessageHandler {
  public:
    virtual ~RfxMainHandler() {}

  public:
    virtual void handleMessage(const Message& message);

  protected:
    // Sub-class should override onHandleMessage(), not handleMessage()
    virtual void onHandleMessage(const Message& message) = 0;
};

/*****************************************************************************
 * Class RfxMainThread
 *****************************************************************************/

class RfxMainThread : public Thread {
  private:
    RfxMainThread();

    virtual ~RfxMainThread();

  public:
    static void init();

    static sp<Looper> waitLooper();  // Must invoke after calling rfx_init()

    static void enqueueMessage(const sp<RfxMessage>& message);

    static void enqueueMessageFront(const sp<RfxMessage>& message);

    static sp<Looper> getLooper();

    static void clearMessages();

  private:
    virtual bool threadLoop();
    void initControllers();

  private:
    static RfxMainThread* s_self;

    sp<Looper> m_looper;

    Message m_dummy_msg;
};

#endif  // __RFX_MAIN_THREAD_H__
