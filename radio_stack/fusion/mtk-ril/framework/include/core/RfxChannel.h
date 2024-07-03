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

#ifndef __RFX_CHANNEL_H__
#define __RFX_CHANNEL_H__

#include <termios.h>
#include "utils/String8.h"
#include "utils/RefBase.h"
#include "RfxMclMessage.h"
#include "RfxDefs.h"
#include "RfxLog.h"
#include "RfxSender.h"
#include "RfxReader.h"

using ::android::RefBase;
using ::android::sp;
using ::android::String8;

class RfxChannel {
  public:
    RfxChannel(int _channel_id, char* _mux_path);

  public:
    void enqueueMessage(const sp<RfxMclMessage>& msg);

    void enqueueMessageFront(const sp<RfxMclMessage>& msg);

    int getChannelId() const { return mChannelId; }
    RfxSender* getSender() const { return mSender; }

    RfxChannelContext* getContext() const { return mContext; }

    RfxReader* getReader() const { return mReader; }

    void setReader(RfxReader* reader) { mReader = reader; }

    void run();

  private:
    int mChannelId;
    int mFd;
    char* mMuxPath;
    RfxSender* mSender;
    RfxReader* mReader;
    RfxChannelContext* mContext;
};

#endif
