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
#include "RtcAgpsThread.h"
#include "RtcAgpsUtil.h"
#include "RtcAgpsNSlotController.h"
#include "RtcAgpsdAdapter.h"

/*****************************************************************************
 * Class RtcAgpsThread
 *****************************************************************************/
RtcAgpsThread::RtcAgpsThread() : m_looper(NULL) {
    RFX_LOG_V(AGPS_TAG, "[RtcAgpsThread]Construct 0x%zu", (size_t)this);
}

RtcAgpsThread::~RtcAgpsThread() {
    RFX_LOG_V(AGPS_TAG, "[RtcAgpsThread]Destructor 0x%zu", (size_t)this);
}

sp<RtcAgpsThread> RtcAgpsThread::create() {
    RFX_LOG_W(AGPS_TAG, "[RtcAgpsThread]create");
    RtcAgpsThread* t = new RtcAgpsThread();
    return t;
}

status_t RtcAgpsThread::readyToRun() {
    RFX_LOG_D(AGPS_TAG, "[RtcAgpsThread]readyToRun");
    m_looper = Looper::prepare(0);
    RfxRootController* root = RFX_OBJ_GET_INSTANCE(RfxRootController);
    RtcAgpsNSlotController* ctrl = (RtcAgpsNSlotController*)root->findController(
            RFX_OBJ_CLASS_INFO(RtcAgpsNSlotController));
    if (ctrl != NULL && ctrl->getAgpsdAdapter() != NULL) {
        ctrl->getAgpsdAdapter()->registerFd(m_looper);
        ctrl->sendAgpsMessage(EVENT_MTK_RILP_INIT, 0);
    }
    return android::NO_ERROR;
}

bool RtcAgpsThread::threadLoop() {
    m_looper->pollAll(-1);
    return true;
}

sp<Looper> RtcAgpsThread::getLooper() { return m_looper; }
