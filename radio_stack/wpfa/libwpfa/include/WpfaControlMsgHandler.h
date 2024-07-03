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

#ifndef WPFA_CONTROL_MSG_HANDLER_H
#define WPFA_CONTROL_MSG_HANDLER_H

#include <sys/ioctl.h>
#include <semaphore.h>
#include <termios.h>

#include <Looper.h>
#include <threads.h>
#include "Mutex.h"

#include "WpfaDriverAdapter.h"
#include "WpfaDriverMessage.h"
#include "WpfaDriverBaseData.h"

#include "WpfaCcciReader.h"
#include "WpfaCcciSender.h"

#include "WpfaDriverUtilis.h"

#include <mtk_log.h>

using ::android::Looper;
using ::android::Message;
using ::android::MessageHandler;
using ::android::sp;
using ::android::Thread;

class WpfaControlMsgHandler : public Thread {
  private:
    class CcciMsgHandler : public MessageHandler {
      public:
        CcciMsgHandler(WpfaControlMsgHandler* _sender, const sp<WpfaDriverMessage>& _msg)
            : msg(_msg), sender(_sender) {}

        virtual ~CcciMsgHandler() {}

        virtual void handleMessage(const Message& message);

      private:
        sp<WpfaDriverMessage> msg;
        WpfaControlMsgHandler* sender;
    };

  private:
    WpfaControlMsgHandler();
    virtual ~WpfaControlMsgHandler() {}

  public:
    static void init(int ccciHandler);
    static void enqueueDriverMessage(const sp<WpfaDriverMessage>& message);
    static void enqueueDriverlMessageFront(const sp<WpfaDriverMessage>& message);

    static sp<Looper> waitLooper();
    virtual bool threadLoop();

    int openCcciDriver();
    void sendMessageToCcci(const sp<WpfaDriverMessage>& msg);
    void sendMessageToRuleHandler(const sp<WpfaDriverMessage>& msg);
    void sendMessageToShmReadMsgHandler(const sp<WpfaDriverMessage>& msg);
    void sendMessageToShmWriteMsgHandler(const sp<WpfaDriverMessage>& msg);

  private:
    int checkDriverAdapterState();

    static int mCcciFd;
    static WpfaControlMsgHandler* s_self;
    sp<Looper> mLooper;
    Message mDummyMsg;

    WpfaDriverAdapter* mDriverAdapter;
    WpfaCcciReader* mReader;
    WpfaCcciSender* mSender;

    WpfaDriverUtilis* mWpfaDriverUtilis;
};

#endif
