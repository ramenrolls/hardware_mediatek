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

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "RfxBasics.h"
#include "RfxObject.h"
#include "RfxRootController.h"
#include "RtcSmsThread.h"
#include "RtcSmsUtil.h"
#include "RtcSmsNSlotController.h"

/*****************************************************************************
 * Class RtcSmsThread
 *****************************************************************************/
RtcSmsThread::RtcSmsThread() : m_looper(NULL) {
    RFX_LOG_V(SMS_TAG, "[RtcSmsThread]Construct 0x%zu", (size_t)this);
}

RtcSmsThread::~RtcSmsThread() {
    RFX_LOG_V(SMS_TAG, "[RtcSmsThread]Destructor 0x%zu", (size_t)this);
}

sp<RtcSmsThread> RtcSmsThread::create() {
    RFX_LOG_W(SMS_TAG, "[RtcSmsThread]create");
    RtcSmsThread* t = new RtcSmsThread();
    return t;
}

status_t RtcSmsThread::readyToRun() {
    RFX_LOG_D(SMS_TAG, "[RtcSmsThread]readyToRun");
    m_looper = Looper::prepare(0);
    return android::NO_ERROR;
}

bool RtcSmsThread::threadLoop() {
    m_looper->pollAll(-1);
    return true;
}

sp<Looper> RtcSmsThread::getLooper() { return m_looper; }
