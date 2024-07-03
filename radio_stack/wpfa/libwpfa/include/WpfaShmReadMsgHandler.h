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

#ifndef WPFA_SHM_READ_MSG_HANDLER_H
#define WPFA_SHM_READ_MSG_HANDLER_H

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

#include <mtk_log.h>

using ::android::Looper;
using ::android::Message;
using ::android::MessageHandler;
using ::android::sp;
using ::android::Thread;

#define MAX_SIZE_RING_BURRER (512)

class WpfaShmReadMsgHandler : public Thread {
  private:
    class ShmReadMsgHandler : public MessageHandler {
      public:
        ShmReadMsgHandler(WpfaShmReadMsgHandler* _dispatcher, const sp<WpfaDriverMessage>& _msg)
            : msg(_msg), sender(_dispatcher) {}

        virtual ~ShmReadMsgHandler() {}

        virtual void handleMessage(const Message& message);

      private:
        sp<WpfaDriverMessage> msg;
        WpfaShmReadMsgHandler* sender;
    };

  private:
    WpfaShmReadMsgHandler();
    virtual ~WpfaShmReadMsgHandler() {}

  public:
    static void init();
    static void enqueueShmReadMessage(const sp<WpfaDriverMessage>& message);
    static void enqueueShmReadMessageFront(const sp<WpfaDriverMessage>& message);

    static sp<Looper> waitLooper();
    virtual bool threadLoop();

  private:
    void processMessage(const sp<WpfaDriverMessage>& msg);
    void onDataReady(uint16_t tId);
    void onRequestDataAck(uint16_t tId);
    void onRequestULIpPkt(const sp<WpfaDriverMessage>& msg);

    int checkDriverAdapterState();
    int sendMessageToModem(uint16_t msgId, uint16_t tId);
    int dump_hex(unsigned char* data, int len);
    int getDestAddress(unsigned char* data, int len);
    int sendPktToKernel(unsigned char* data, int len);

    static WpfaShmReadMsgHandler* s_self;
    sp<Looper> mLooper;
    Message mDummyMsg;

    uint16_t mTid;
    WpfaDriverAdapter* mDriverAdapter;
};

#endif
