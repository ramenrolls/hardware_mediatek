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

#ifndef WPFA_SHM_WRITE_MSG_HANDLER_H
#define WPFA_SHM_WRITE_MSG_HANDLER_H

#include <sys/ioctl.h>
#include <semaphore.h>
#include <termios.h>
#include <Looper.h>
#include <threads.h>
#include "Mutex.h"
#include "hardware/ccci_intf.h"

#include "WpfaDriverMessage.h"
#include "WpfaDriverBaseData.h"

#include "WpfaDriverAdapter.h"
#include "WpfaShmSynchronizer.h"

#include <mtk_log.h>

using ::android::Looper;
using ::android::Message;
using ::android::MessageHandler;
using ::android::sp;
using ::android::Thread;

#define MODEM_TID_INIT (-1)

#define MAX_SIZE_RING_BURRER (512)

class WpfaShmWriteMsgHandler : public Thread {
  private:
    class ShmWriteMsgHandler : public MessageHandler {
      public:
        ShmWriteMsgHandler(WpfaShmWriteMsgHandler* _dispatcher, const sp<WpfaDriverMessage>& _msg)
            : msg(_msg), sender(_dispatcher) {}

        virtual ~ShmWriteMsgHandler() {}

        virtual void handleMessage(const Message& message);

      private:
        sp<WpfaDriverMessage> msg;
        WpfaShmWriteMsgHandler* sender;
    };

  private:
    WpfaShmWriteMsgHandler();
    virtual ~WpfaShmWriteMsgHandler() {}

  public:
    static void init();
    static void enqueueShmWriteMessage(const sp<WpfaDriverMessage>& message);
    static void enqueueShmWriteMessageFront(const sp<WpfaDriverMessage>& message);

    static sp<Looper> waitLooper();
    virtual bool threadLoop();

  private:
    void processMessage(const sp<WpfaDriverMessage>& msg);

    int checkDriverAdapterState();
    int checkShmControllerState();
    int sendMessageToModem(uint16_t msgId, uint16_t tId);

    static WpfaShmWriteMsgHandler* s_self;
    sp<Looper> mLooper;
    Message mDummyMsg;

    WpfaDriverAdapter* mDriverAdapter;
    WpfaShmSynchronizer* mShmSynchronizer;
};

#endif
