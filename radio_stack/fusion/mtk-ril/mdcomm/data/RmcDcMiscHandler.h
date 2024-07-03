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

#ifndef __RMC_DC_MISC_HANDLER_H__
#define __RMC_DC_MISC_HANDLER_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "RfxBaseHandler.h"
#include "RmcDcMiscImpl.h"

/*****************************************************************************
 * Class RmcDcMiscHandler
 *****************************************************************************/
class RmcDcMiscHandler : public RfxBaseHandler {
    RFX_DECLARE_HANDLER_CLASS(RmcDcMiscHandler);

  public:
    RmcDcMiscHandler(int slotId, int channelId);
    virtual ~RmcDcMiscHandler();

  protected:
    virtual void onHandleRequest(const sp<RfxMclMessage>& msg);
    virtual void onHandleEvent(const sp<RfxMclMessage>& msg);

  private:
    void init();
    void handleSetFdModeRequest(const sp<RfxMclMessage>& msg);
    void handleStartLceRequest(const sp<RfxMclMessage>& msg);
    void handleStopLceRequest(const sp<RfxMclMessage>& msg);
    void handlePullLceDataRequest(const sp<RfxMclMessage>& msg);
    void handleLceStatusChanged(const sp<RfxMclMessage>& msg);
    void handleSetLinkCapacityReportingCriteriaRequest(const sp<RfxMclMessage>& msg);
    void handleLinkCapacityEstimate(const sp<RfxMclMessage>& msg);
    void handleNetworkRejectCause(const sp<RfxMclMessage>& msg);
    void handleSendDeviceStateRequest(const sp<RfxMclMessage>& msg);

  private:
    RmcDcMiscImpl* m_pRmcDcMiscImpl;
};
#endif /* __RMC_MISC_REQ_HANDLER_H__ */
