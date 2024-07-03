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

#include "RtcSuppServQueue.h"
#include "RfxRilUtils.h"
#include "RfxLog.h"

#ifdef RFX_LOG_TAG
#undef RFX_LOG_TAG
#endif
#define RFX_LOG_TAG "RtcSuppServQueue"

// The instance of RtcSuppServQueue will be initialized by RtcSuppServController
RtcSuppServQueue* RtcSuppServQueue::mInstance = NULL;

RtcSuppServQueue* RtcSuppServQueue::getInstance() {
    if (mInstance == NULL) {
        mInstance = new RtcSuppServQueue();
    }

    return mInstance;
}

RtcSuppServQueue::RtcSuppServQueue() {
    RFX_LOG_D(RFX_LOG_TAG, "Init RtcSuppServQueue's singleton instance!");
    mSSLock = SS_UNLOCKED;
}

RtcSuppServQueue::~RtcSuppServQueue() {
    RFX_LOG_D(RFX_LOG_TAG, "Delete RtcSuppServQueue's singleton instance!");
    delete mInstance;
}

void RtcSuppServQueue::add(RtcSSEntry ssEntry) { mSuspendedMsgQueue.add(ssEntry); }

void RtcSuppServQueue::clear() { mSuspendedMsgQueue.clear(); }

void RtcSuppServQueue::removeFront() { mSuspendedMsgQueue.removeAt(0); }

bool RtcSuppServQueue::isEmpty() { return mSuspendedMsgQueue.isEmpty(); }

const RtcSSEntry& RtcSuppServQueue::itemAt(int index) { return mSuspendedMsgQueue.itemAt(index); }

void RtcSuppServQueue::setSSLock(SSLockState lock) {
    RFX_LOG_D(RFX_LOG_TAG, "setSSLock: %s -> %s", lockToString(mSSLock), lockToString(lock));
    if (mSSLock == lock) {
        return;
    }
    mSSLock = lock;
}

SSLockState RtcSuppServQueue::getSSLock() { return mSSLock; }

const char* RtcSuppServQueue::lockToString(SSLockState lock) {
    switch (lock) {
        case SS_UNLOCKED:
            return "SS_UNLOCKED";
        case SS_LOCKED:
            return "SS_LOCKED";
        default:
            // not possible here!
            return NULL;
    }
}
