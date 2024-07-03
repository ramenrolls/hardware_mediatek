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

#ifndef WPFA_CCCI_SENDER_H
#define WPFA_CCCI_SENDER_H

#include <Looper.h>
#include <threads.h>
#include <RefBase.h>
#include <Mutex.h>

#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdint.h>
#include <semaphore.h>

#include <mtk_log.h>
#include "WpfaDriverMessage.h"

using ::android::Looper;
using ::android::Message;
using ::android::MessageHandler;
using ::android::Mutex;
using ::android::RefBase;
using ::android::sp;
using ::android::Thread;

class WpfaCcciSender : public Thread {
  public:
    WpfaCcciSender(int fd);

  private:
    class CcciMsgHandler : public MessageHandler {
      public:
        CcciMsgHandler(WpfaCcciSender* _sender, const sp<WpfaDriverMessage>& _msg)
            : msg(_msg), sender(_sender) {}

        virtual ~CcciMsgHandler() {}

      public:
        virtual void handleMessage(const Message& message);

      private:
        sp<WpfaDriverMessage> msg;
        WpfaCcciSender* sender;
    };

  public:
    void enqueueCcciMessage(const sp<WpfaDriverMessage>& msg);

    void enqueueCcciMessageFront(const sp<WpfaDriverMessage>& msg);

    sp<Looper> waitLooper();  // Must invoke after new RfxSender

  private:
    virtual bool threadLoop();
    void processMessage(const sp<WpfaDriverMessage>& msg);

    int driverMsgToCcciMsg(const sp<WpfaDriverMessage>& driverMsg, ccci_msg_send_t* pMsgSend);

    sp<Looper> m_looper;
    int m_fd;
    Message m_dummy_msg;
    friend CcciMsgHandler;
    // int m_atTimeoutMsec;
    pthread_t m_threadId;
    // const char* mName;

    sem_t mWaitLooperSem;
    bool mNeedWaitLooper;
    Mutex mWaitLooperMutex;
};
#endif
