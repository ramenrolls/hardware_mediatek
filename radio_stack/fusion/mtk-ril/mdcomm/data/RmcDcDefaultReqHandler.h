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

#ifndef __RMC_DC_DEFAULT_REQ_HANDLER_H__
#define __RMC_DC_DEFAULT_REQ_HANDLER_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "RmcDcCommonReqHandler.h"

/*****************************************************************************
 * Class RmcDcDefaultReqHandler
 *****************************************************************************/
class RmcDcDefaultReqHandler : public RmcDcCommonReqHandler {
  public:
    RmcDcDefaultReqHandler(int slot_id, int channel_id, RmcDcPdnManager* pdnManager);
    virtual ~RmcDcDefaultReqHandler();
    virtual void requestSetupDataCall(const sp<RfxMclMessage>& msg);
    virtual void requestDeactivateDataCall(const sp<RfxMclMessage>& msg);
    virtual void onNwPdnAct(const sp<RfxMclMessage>& msg);
    virtual void onNwPdnDeact(const sp<RfxMclMessage>& msg);
    virtual void onNwModify(const sp<RfxMclMessage>& msg);
    virtual void onNwReact(const sp<RfxMclMessage>& msg);
    virtual void onMePdnAct(const sp<RfxMclMessage>& msg);
    virtual void onMePdnDeact(const sp<RfxMclMessage>& msg);
    virtual void onPdnChange(const sp<RfxMclMessage>& msg);
    virtual void onQualifiedNetworkTypeChanged(const sp<RfxMclMessage>& msg);
    virtual void requestSyncApnTable(const sp<RfxMclMessage>& msg);
    virtual void requestSyncDataSettingsToMd(const sp<RfxMclMessage>& msg);
    virtual void requestResetMdDataRetryCount(const sp<RfxMclMessage>& msg);
    void requestLastFailCause(const sp<RfxMclMessage>& msg);
    virtual void requestOrSendDataCallList(const sp<RfxMclMessage>& msg);
    virtual void requestOrSendDataCallList(const sp<RfxMclMessage>& msg, Vector<int>* vAidList);
    virtual void requestOrSendDataCallList(const sp<RfxMclMessage>& msg, int aid);
    virtual void requestQueryPco(int aid, int ia, const char* apn, const char* iptype);
    void requestSetLteAccessStratumReport(const sp<RfxMclMessage>& msg);
    void onLteAccessStratumStateChange(const sp<RfxMclMessage>& msg);
    void requestSetLteUplinkDataTransfer(const sp<RfxMclMessage>& msg);
    void requestSetLteUplinkDataTransferCompleted(bool bSuccess, const sp<RfxMclMessage> msg);
    int convertNetworkType(int nAct);
    virtual void requestClearAllPdnInfo(const sp<RfxMclMessage>& msg);
    virtual void requestResendSyncDataSettingsToMd(const sp<RfxMclMessage>& msg);
    virtual void handleResetAllConnections(const sp<RfxMclMessage>& msg);
    virtual void requestSetPreferredDataModem(const sp<RfxMclMessage>& msg);
    virtual void requestGetDataContextIds(const sp<RfxMclMessage>& msg);
    virtual void requestSendQualifiedNetworkTypesChanged(const sp<RfxMclMessage>& msg);
    virtual void mobileDataUsageNotify(const sp<RfxMclMessage>& msg);
    virtual void requestStartKeepalive(const sp<RfxMclMessage>& msg);
    virtual void requestStopKeepalive(const sp<RfxMclMessage>& msg);
    virtual void updateKeepaliveStatus(const sp<RfxMclMessage>& msg);
    virtual void onNwLimitNotify(const sp<RfxMclMessage>& msg);
};
#endif /* __RMC_DC_DEFAULT_REQ_HANDLER_H__ */
