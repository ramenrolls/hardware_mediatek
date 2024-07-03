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

#ifndef __RFX_MCL_DISPATCH_THREAD__H__
#define __RFX_MCL_DISPATCH_THREAD__H__

#include <utils/Looper.h>
#include <utils/threads.h>
#include "utils/Timers.h"
#include "RfxMclMessage.h"
#include "RfxChannelManager.h"

using ::android::Looper;
using ::android::Message;
using ::android::MessageHandler;
using ::android::sp;
using ::android::Thread;

class RfxMclBaseMessenger : public MessageHandler {
  public:
    virtual ~RfxMclBaseMessenger() {}

  public:
    virtual void handleMessage(const Message& message);

  protected:
    // Sub-class should override onHandleMessage(), not handleMessage()
    virtual void onHandleMessage(const Message& message) = 0;
};

class RfxMclMessenger : public RfxMclBaseMessenger {
  public:
    RfxMclMessenger(const sp<RfxMclMessage>& _msg) : msg(_msg) {}

  protected:
    // Sub-class should override onHandleMessage(), not handleMessage()
    virtual void onHandleMessage(const Message& message);

  private:
    sp<RfxMclMessage> msg;
};

class RfxMclDispatcherThread : public Thread {
  private:
    RfxMclDispatcherThread();

    virtual ~RfxMclDispatcherThread() {}

  public:
    static void init();

    static void enqueueMclMessage(const sp<RfxMclMessage>& message);

    static void enqueueMclMessageFront(const sp<RfxMclMessage>& message);

    static void enqueueMclMessageDelay(const sp<RfxMclMessage>& message);

    static sp<Looper> waitLooper();

  private:
    virtual bool threadLoop();

  private:
    static RfxMclDispatcherThread* s_self;
    sp<Looper> m_looper;
    Message m_dummy_msg;
};

#endif
