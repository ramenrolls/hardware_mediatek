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

#ifndef __RFX_READER_H__
#define __RFX_READER_H__

#include <utils/Looper.h>
#include <utils/threads.h>
#include "utils/RefBase.h"
#include "utils/String8.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "RfxDefs.h"
#include "RfxMisc.h"
// #include "RfxAtLine.h"
#include "RfxAtResponse.h"
#include "RfxFragmentEncoder.h"

using ::android::Looper;
using ::android::Message;
using ::android::MessageHandler;
using ::android::RefBase;
using ::android::sp;
using ::android::String8;
using ::android::Thread;

class RfxChannelContext;

class RfxReader : public Thread {
  public:
    RfxReader(int fd, int channel_id, RfxChannelContext* context);
    int getChannelId() const { return m_channel_id; }
    void setChannelId(int channelId);

    RfxChannelContext* getChannelContext() const { return m_context; }
    void setChannelContext(RfxChannelContext* context) { m_context = context; }

  private:
    virtual bool threadLoop();
    void readerLoop();
    void readerLoopForFragData();
    void processLine(const char* line);
    char* readline(char* buffer);
    void handleUnsolicited(RfxAtLine* line1, RfxAtLine* line2);
    void handleFinalResponse(RfxAtLine* line);
    int isSMSUnsolicited(const char* line);
    char* findNextEOL(char* cur);
    void handleUserDataEvent(int clientId, char* data, size_t length);
    void handleRequestAck();
    void printLog(int level, String8 log);

  private:
    sp<Looper> m_looper;
    int m_fd;
    int m_channel_id;
    RfxChannelContext* m_context;
    pthread_t m_threadId;
    const char* mName;
    char* m_pATBufferCur;
    char m_aTBuffer[MAX_AT_RESPONSE + 1];
};
#endif
