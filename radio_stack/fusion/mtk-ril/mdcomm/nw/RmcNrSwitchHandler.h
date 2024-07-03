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

#ifndef __RMC_NR_SWITCH_HANDLER_H__
#define __RMC_NR_SWITCH_HANDLER_H__

#undef NDEBUG
#ifdef LOG_NDEBUG
#undef LOG_NDEBUG
#endif

#include "RmcNetworkHandler.h"
#include "RfxMclDispatcherThread.h"
#include "mtk_log.h"

#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "RmcNrSwHdlr"

class RmcNrSwitchHandler : public RfxBaseHandler {
    RFX_DECLARE_HANDLER_CLASS(RmcNrSwitchHandler);

  public:
    RmcNrSwitchHandler(int slot_id, int channel_id);
    virtual ~RmcNrSwitchHandler();

  protected:
    virtual void onHandleRequest(const sp<RfxMclMessage>& msg);

    void requestNrSwitch(const sp<RfxMclMessage>& msg);

  protected:
    int m_slot_id;
    int m_channel_id;
    int mRetryCount;
};
#endif
