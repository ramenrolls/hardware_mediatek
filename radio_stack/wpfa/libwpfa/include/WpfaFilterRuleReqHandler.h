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

#ifndef WPFA_FILTER_RULE_REQ_HANDLER_H
#define WPFA_FILTER_RULE_REQ_HANDLER_H

#include <sys/ioctl.h>
#include <semaphore.h>
#include <termios.h>
#include <Looper.h>
#include <threads.h>
#include "Mutex.h"
#include "hardware/ccci_intf.h"

#include "WpfaDriver.h"
#include "WpfaDriverAdapter.h"
#include "WpfaDriverMessage.h"
#include "WpfaDriverBaseData.h"

#include <mtk_log.h>

using ::android::Looper;
using ::android::Message;
using ::android::MessageHandler;
using ::android::sp;
using ::android::Thread;

class WpfaFilterRuleReqHandler : public Thread {
  private:
    class RuleReqMsgHandler : public MessageHandler {
      public:
        RuleReqMsgHandler(WpfaFilterRuleReqHandler* _dispatcher, const sp<WpfaDriverMessage>& _msg)
            : msg(_msg), sender(_dispatcher) {}

        virtual ~RuleReqMsgHandler() {}

        virtual void handleMessage(const Message& message);

      private:
        sp<WpfaDriverMessage> msg;
        WpfaFilterRuleReqHandler* sender;
    };

  private:
    WpfaFilterRuleReqHandler();
    virtual ~WpfaFilterRuleReqHandler() {}

  public:
    static void init();
    static void enqueueFilterRuleReqMessage(const sp<WpfaDriverMessage>& message);
    static void enqueueFilterRuleReqMessageFront(const sp<WpfaDriverMessage>& message);

    static sp<Looper> waitLooper();
    virtual bool threadLoop();

  private:
    void processMessage(const sp<WpfaDriverMessage>& msg);
    int sendRegAcceptResponse(uint16_t tId, uint32_t fId, int32_t errCause);
    int sendDeregAcceptResponse(uint16_t tId, uint32_t fId, int32_t errCause);
    int checkDriverState();
    int checkDriverAdapterState();
    int DriverMsgIdToEventId(uint16_t msgId);

    static WpfaFilterRuleReqHandler* s_self;
    sp<Looper> mLooper;
    Message mDummyMsg;

    WpfaDriver* mWpfaDriver;
    WpfaDriverAdapter* mDriverAdapter;
};

#endif
