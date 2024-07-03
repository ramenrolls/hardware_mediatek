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

#ifndef __RMC_EMBMS_URC_HANDLER_H__
#define __RMC_EMBMS_URC_HANDLER_H__

#include "RfxBaseHandler.h"
#include "RfxIntsData.h"
#include "RfxVoidData.h"
#include "RfxStringData.h"
#include "RfxMessageId.h"

#include <telephony/mtk_ril.h>
#include "embms/RfxEmbmsLocalOosNotifyData.h"
#include "embms/RfxEmbmsLocalStartSessionRespData.h"

#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "RmcEmbmsUrc"
#define RIL_CMD_PROXY_EMBMS RIL_CMD_PROXY_6

class RmcEmbmsURCHandler : public RfxBaseHandler {
    RFX_DECLARE_HANDLER_CLASS(RmcEmbmsURCHandler);

  public:
    RmcEmbmsURCHandler(int slot_id, int channel_id);
    virtual ~RmcEmbmsURCHandler();

  protected:
    virtual void onHandleUrc(const sp<RfxMclMessage>& msg);
    virtual void onHandleEvent(const sp<RfxMclMessage>& msg);

  private:
    void onEmbmsSrvStatus(const sp<RfxMclMessage>& msg);
    void onEmbmsSessionListUpdate(const sp<RfxMclMessage>& msg);
    void onEmbmsSaiListUpdate(const sp<RfxMclMessage>& msg);
    void onEmbmsSessionActiveUpdate(const sp<RfxMclMessage>& msg);
    void onEpsNetworkUpdate(const sp<RfxMclMessage>& msg);
    void onEmbmsHvolteUpdate(const sp<RfxMclMessage>& msg);

    void handleInitialVariable(const sp<RfxMclMessage>& msg);
    bool isHvolteDisable();

  private:
    RIL_EMBMS_LocalOosNotify oos_tmgi;
    int oos_tmgi_count;
};

#endif
