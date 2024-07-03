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

#ifndef __RMC_DC_REQ_HANDLER_H__
#define __RMC_DC_REQ_HANDLER_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "RfxBaseHandler.h"
#include "RmcDataDefs.h"
#include "RmcDcDefaultReqHandler.h"
#include "RmcDcImsReqHandler.h"
#include "RmcDcOnDemandReqHandler.h"
#include "RmcDcPdnManager.h"

/*****************************************************************************
 * Class RmcDcReqHandler
 *****************************************************************************/
class RmcDcReqHandler : public RfxBaseHandler {
    RFX_DECLARE_HANDLER_CLASS(RmcDcReqHandler);

  public:
    RmcDcReqHandler(int slot_id, int channel_id);
    virtual ~RmcDcReqHandler();

  protected:
    virtual void onHandleRequest(const sp<RfxMclMessage>& msg);
    virtual void onHandleEvent(const sp<RfxMclMessage>& msg);

  private:
    void handleLastFailCauseRequest(const sp<RfxMclMessage>& msg);
    void handleSetupDataCallRequest(const sp<RfxMclMessage>& msg);
    void handleDeactivateDataCallRequest(const sp<RfxMclMessage>& msg);
    void handleDataCallListRequest(const sp<RfxMclMessage>& msg);
    int deactivateDataCall(int aid);
    void handleNwPdnAct(const sp<RfxMclMessage>& msg);
    void handleNwPdnDeact(const sp<RfxMclMessage>& msg);
    void handleNwModify(const sp<RfxMclMessage>& msg);
    void handleNwReact(const sp<RfxMclMessage>& msg);
    void handleMePdnAct(const sp<RfxMclMessage>& msg);
    void handleMePdnDeact(const sp<RfxMclMessage>& msg);
    void handlePdnChange(const sp<RfxMclMessage>& msg);
    void handleSyncApnTableRequest(const sp<RfxMclMessage>& msg);
    void handleSetInitialAttachApnRequest(const sp<RfxMclMessage>& msg);
    void handleSyncDataSettingsToMdRequest(const sp<RfxMclMessage>& msg);
    void handleQualifiedNetworkTypeChanged(const sp<RfxMclMessage>& msg);
    void handleResetMdDataRetryCount(const sp<RfxMclMessage>& msg);
    void handleUtTest(const sp<RfxMclMessage>& msg);
    void handleSetLteAccessStratumReportRequest(const sp<RfxMclMessage>& msg);
    void handleLteAccessStratumStateChange(const sp<RfxMclMessage>& msg);
    void handleSetLteUplinkDataTransferRequest(const sp<RfxMclMessage>& msg);
    void handleClearAllPdnInfoRequest(const sp<RfxMclMessage>& msg);
    void handleResendSyncDataSettingsToMd(const sp<RfxMclMessage>& msg);
    void handleQueryPco(const sp<RfxMclMessage>& msg);
    void handleResetAllConnections(const sp<RfxMclMessage>& msg);
    void handleSetPreferredDataModem(const sp<RfxMclMessage>& msg);
    bool isValidInitialAttachApn(const char* apn);
    void handleGetDataContextIds(const sp<RfxMclMessage>& msg);
    void handleSendQualifiedNetworkTypesChanged(const sp<RfxMclMessage>& msg);
    void handleIfVendorSelfIaNeeded(const sp<RfxMclMessage>& msg);
    void handleMobileDataUsageNotify(const sp<RfxMclMessage>& msg);
    void handleStartKeepaliveRequest(const sp<RfxMclMessage>& msg);
    void handleStopKeepaliveRequest(const sp<RfxMclMessage>& msg);
    void handleKeepaliveStatus(const sp<RfxMclMessage>& msg);
    void handleNwLimitNotify(const sp<RfxMclMessage>& msg);

  private:
    RmcDcDefaultReqHandler* m_pRmcDcDefaultReqHdlr;
    RmcDcImsReqHandler* m_pRmcDcImsReqHdlr;
    RmcDcOnDemandReqHandler* m_pRmcDcOnDemandReqHdlr;
    RmcDcPdnManager* m_pPdnManager;
    int iaMode;
};
#endif /* __RMC_DC_REQ_HANDLER_H__ */
