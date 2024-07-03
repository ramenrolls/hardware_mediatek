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

#ifndef WPFA_CCCI_READER_H
#define WPFA_CCCI_READER_H

#include <Looper.h>
#include <threads.h>
#include "RefBase.h"
#include "String8.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdint.h>

#include <mtk_log.h>
#include "WpfaDriverMessage.h"

using ::android::Looper;
using ::android::Message;
using ::android::MessageHandler;
using ::android::RefBase;
using ::android::sp;
using ::android::String8;
using ::android::Thread;

// class RfxChannelContext;

class WpfaCcciReader : public Thread {
  public:
    WpfaCcciReader(int fd);
    void handleFilterRuleCtrlEvent(WpfaCcciDataHeader header, WpfaDriverBaseData data);
    void handleShmCtrlEvent(WpfaCcciDataHeader header);

  private:
    virtual bool threadLoop();
    void readerLoopForCcciData();

  private:
    sp<Looper> m_looper;
    int mFd;
    pthread_t m_threadId;
};
#endif
