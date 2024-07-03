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

#include "WpfaShmDataReader.h"

WpfaShmDataReader::WpfaShmDataReader() {
    mReaderThread = 0;
    mWpfaRingBuffer = NULL;
    cout << "-new()" << endl;
}

WpfaShmDataReader::~WpfaShmDataReader() { cout << "-del()" << endl; }

bool WpfaShmDataReader::init(WpfaRingBuffer* ringBuf) {
    mWpfaRingBuffer = ringBuf;
    return true;
}

void WpfaShmDataReader::start() {
    int ret;
    pthread_attr_t attr;

    // creat reader thread
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    ret = pthread_create(&mReaderThread, &attr, WpfaShmDataReader::readerThreadStart, this);
    pthread_attr_destroy(&attr);
    if (ret != 0) {
        cout << "-start() fail" << endl;
    } else {
        // cout << "WpfaShmDataReader-start() create reader thread OK!" << endl;
    }
}

void* WpfaShmDataReader::readerThreadStart(void* arg) {
    // cout << "WpfaShmDataReader-readerThreadStart()" << endl;
    WpfaShmDataReader* me = (WpfaShmDataReader*)arg;
    me->runReaderLoop();
    return NULL;
}

void WpfaShmDataReader::runReaderLoop() {
    // read data from share momery and copy to internal ring buffer
    int ret = 0;
    unsigned char testChars1[] = "AAAAA";
    cout << "-runReaderLoop()" << endl;

    // while(1) {
    for (int i = 0; i < 2; i++) {
        mWpfaRingBuffer->lock("Producer");
        mWpfaRingBuffer->waitCanWrite("Producer", 5);
        ret = mWpfaRingBuffer->writeDataToRingBuffer(testChars1, 5);
        if (ret <= 0) {
            cout << "runReaderLoop() write fail!!!" << endl;
        } else {
            cout << "[Producer] ==> write size:" << ret << endl;
        }
        mWpfaRingBuffer->signalCanRead("Producer");
        mWpfaRingBuffer->unlock("Producer");
    }
}
