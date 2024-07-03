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

#ifndef __RFX_SUPP_SERV_QUEUE_H__
#define __RFX_SUPP_SERV_QUEUE_H__

#include "RfxMessage.h"

using ::android::Vector;

enum SSLockState { SS_UNLOCKED = 0, SS_LOCKED = 1 };

class RtcSSEntry {
  public:
    RtcSSEntry() : message(NULL) {}
    RtcSSEntry(const sp<RfxMessage>& msg) : message(msg) {}

  public:
    sp<RfxMessage> message;
};

class RtcSuppServQueue {
  public:
    ~RtcSuppServQueue();
    static RtcSuppServQueue* getInstance();

    // Operation of queue
    void add(RtcSSEntry ssEntry);
    void removeFront();
    void clear();
    bool isEmpty();
    const RtcSSEntry& itemAt(int index);

    // Operation of lock
    void setSSLock(SSLockState lock);
    SSLockState getSSLock();

  private:
    RtcSuppServQueue();
    static RtcSuppServQueue* mInstance;

    Vector<RtcSSEntry> mSuspendedMsgQueue;
    SSLockState mSSLock;
    const char* lockToString(SSLockState lock);
};

#endif /* __RFX_SUPP_SERV_QUEUE_H__ */
