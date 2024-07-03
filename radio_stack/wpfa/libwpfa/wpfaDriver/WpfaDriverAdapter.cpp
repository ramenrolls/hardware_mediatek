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

#include "WpfaDriverAdapter.h"
#include "WpfaControlMsgHandler.h"
#include "WpfaFilterRuleReqHandler.h"
#include "WpfaShmReadMsgHandler.h"
#include "WpfaShmWriteMsgHandler.h"

#define WPFA_D_LOG_TAG "WpfaDriverAdapter"

WpfaDriverAdapter* WpfaDriverAdapter::sInstance = NULL;
Mutex WpfaDriverAdapter::sWpfaDriverAdapterInitMutex;
int WpfaDriverAdapter::mCcciHandler = -1;

WpfaDriverAdapter::WpfaDriverAdapter() {
    mtkLogD(WPFA_D_LOG_TAG, "-new()");
    init();
}

void WpfaDriverAdapter::init() {
    mtkLogD(WPFA_D_LOG_TAG, "-init()");
    WpfaFilterRuleReqHandler::init();
    WpfaShmWriteMsgHandler::init();
    WpfaShmReadMsgHandler::init();
    WpfaControlMsgHandler::init(mCcciHandler);
}

WpfaDriverAdapter::~WpfaDriverAdapter() { mtkLogD(WPFA_D_LOG_TAG, "-del()"); }

WpfaDriverAdapter* WpfaDriverAdapter::getInstance() {
    if (sInstance != NULL) {
        return sInstance;
    } else {
        sWpfaDriverAdapterInitMutex.lock();
        sInstance = new WpfaDriverAdapter();
        if (sInstance == NULL) {
            mtkLogE(WPFA_D_LOG_TAG, "new WpfaDriverAdapter fail");
        }
        sWpfaDriverAdapterInitMutex.unlock();
        return sInstance;
    }
}

int WpfaDriverAdapter::sendMsgToControlMsgDispatcher(const sp<WpfaDriverMessage>& message) {
    int retValue = 0;
    WpfaControlMsgHandler::enqueueDriverMessage(message);
    return retValue;
}

int WpfaDriverAdapter::sendMsgToFilterRuleReqHandler(const sp<WpfaDriverMessage>& message) {
    int retValue = 0;
    WpfaFilterRuleReqHandler::enqueueFilterRuleReqMessage(message);
    return retValue;
}

int WpfaDriverAdapter::sendMsgToShmReadMsgHandler(const sp<WpfaDriverMessage>& message) {
    int retValue = 0;
    WpfaShmReadMsgHandler::enqueueShmReadMessage(message);
    return retValue;
}

int WpfaDriverAdapter::sendMsgToShmWriteMsgHandler(const sp<WpfaDriverMessage>& message) {
    int retValue = 0;
    WpfaShmWriteMsgHandler::enqueueShmWriteMessage(message);
    return retValue;
}

void WpfaDriverAdapter::setCcciHandler(int fd) {
    mCcciHandler = fd;
    mtkLogD(WPFA_D_LOG_TAG, "setCcciHandler:%d", mCcciHandler);
}

int WpfaDriverAdapter::getCcciHandler() { return mCcciHandler; }
