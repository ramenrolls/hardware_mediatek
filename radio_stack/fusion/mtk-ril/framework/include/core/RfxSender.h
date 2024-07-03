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

#ifndef __RFX_SENDER_H__
#define __RFX_SENDER_H__

#include <utils/Looper.h>
#include <utils/threads.h>
#include "utils/RefBase.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "RfxMclMessage.h"
#include "RfxAtResponse.h"
#include "RfxLog.h"
#include "RfxRilUtils.h"
#include "rfx_properties.h"
#include <sys/time.h>
#include "RfxChannelContext.h"
#include <semaphore.h>
#include "utils/Mutex.h"

using ::android::Looper;
using ::android::Message;
using ::android::MessageHandler;
using ::android::Mutex;
using ::android::RefBase;
using ::android::sp;
using ::android::Thread;

class RfxSender : public Thread {
  public:
    RfxSender(int fd, int channel_id, RfxChannelContext* context);

  private:
    class MclMessageHandler : public MessageHandler {
      public:
        MclMessageHandler(RfxSender* _sender, const sp<RfxMclMessage>& _msg)
            : msg(_msg), sender(_sender) {}
        virtual ~MclMessageHandler() {}

      public:
        virtual void handleMessage(const Message& message);

      private:
        sp<RfxMclMessage> msg;
        RfxSender* sender;
    };

  public:
    void enqueueMessage(const sp<RfxMclMessage>& msg);

    void enqueueMessageFront(const sp<RfxMclMessage>& msg);

    sp<Looper> waitLooper();  // Must invoke after new RfxSender

  private:
    virtual bool threadLoop();

    void processMessage(const sp<RfxMclMessage>& msg);

  public:
    sp<RfxAtResponse> atSendCommandSinglelineAck(const char* command, const char* responsePrefix,
                                                 RIL_Token ackToken);
    sp<RfxAtResponse> atSendCommandSingleline(const char* command, const char* responsePrefix);
    sp<RfxAtResponse> atSendCommandNumericAck(const char* command, RIL_Token ackToken);
    sp<RfxAtResponse> atSendCommandNumeric(const char* command);
    sp<RfxAtResponse> atSendCommandMultilineAck(const char* command, const char* responsePrefix,
                                                RIL_Token ackToken);
    sp<RfxAtResponse> atSendCommandMultiline(const char* command, const char* responsePrefix);
    sp<RfxAtResponse> atSendCommandAck(const char* command, RIL_Token ackToken);
    sp<RfxAtResponse> atSendCommand(const char* command);
    sp<RfxAtResponse> atSendCommandRawAck(const char* command, RIL_Token ackToken);
    sp<RfxAtResponse> atSendCommandRaw(const char* command);

    int sendUserData(int clientId, unsigned char* data, size_t length);
    int sendUserData(int clientId, int config, unsigned char* data, size_t length);

    int getFd() const { return m_fd; }
    void setFd(int fd) { m_fd = fd; }

  private:
    sp<RfxAtResponse> atSendCommandFullAck(const char* command, AtCommandType type,
                                           const char* responsePrefix, long long timeoutMsec,
                                           RIL_Token ackToken);
    sp<RfxAtResponse> atSendCommandFull(const char* command, AtCommandType type,
                                        const char* responsePrefix, long long timeoutMsec);
    sp<RfxAtResponse> atSendCommandFullNolockAck(const char* command, AtCommandType type,
                                                 const char* responsePrefix, long long timeoutMsec,
                                                 RIL_Token ackToken);
    sp<RfxAtResponse> atSendCommandFullNolock(const char* command, AtCommandType type,
                                              const char* responsePrefix, long long timeoutMsec);

    int getATCommandTimeout(const char* command);

    int writeline(const char* s);
    int writelineUserData(unsigned char* frame, size_t length);
    void printLog(int level, String8 log);

  private:
    sp<Looper> m_looper;
    int m_fd;
    int m_channel_id;
    Message m_dummy_msg;
    friend MclMessageHandler;
    int m_atTimeoutMsec;
    RfxChannelContext* m_context;
    pthread_t m_threadId;
    const char* mName;

    sem_t mWaitLooperSem;
    bool mNeedWaitLooper;
    Mutex mWaitLooperMutex;
    int mIsFuzzyTesting;
    int mFuzzyTestingTimeout;
};

#endif
