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

#ifndef __RMC_NETWORK_URC_HANDLER_H__
#define __RMC_NETWORK_URC_HANDLER_H__

#include "RmcNetworkHandler.h"
#include "wp/RmcWpRequestHandler.h"

#define MAX_NITZ_TZ_DST_LENGTH 10

typedef struct {
    int cell_data_speed;
    int max_data_bearer;
} RIL_PS_BEARER_CACHE;

class RmcNetworkUrcHandler : public RmcNetworkHandler {
    RFX_DECLARE_HANDLER_CLASS(RmcNetworkUrcHandler);

  public:
    RmcNetworkUrcHandler(int slot_id, int channel_id);
    virtual ~RmcNetworkUrcHandler();

  protected:
    virtual void onHandleRequest(const sp<RfxMclMessage>& msg) { RFX_UNUSED(msg); }

    virtual void onHandleUrc(const sp<RfxMclMessage>& msg);

    virtual void onHandleTimer();

    virtual void onHandleEvent(const sp<RfxMclMessage>& msg);

    virtual bool onCheckIfRejectMessage(const sp<RfxMclMessage>& msg, RIL_RadioState radioState);

  private:
    void handleCsNetworkStateChanged(const sp<RfxMclMessage>& msg);
    void handlePsDataServiceCapability(const sp<RfxMclMessage>& msg);
    void handleSignalStrength(const sp<RfxMclMessage>& msg);
    void handlePsNetworkStateChanged(const sp<RfxMclMessage>& msg);
    void handleOtaProvisionStatus(const sp<RfxMclMessage>& msg);
    void handleConfirmRatBegin(const sp<RfxMclMessage>& msg);
    void handleGmssRatChanged(const sp<RfxMclMessage>& msg);
    void handleSystemInPrlIndication(const sp<RfxMclMessage>& msg);
    void handleDefaultRoamingIndicator(const sp<RfxMclMessage>& msg);
    void handleNeighboringCellInfo(const sp<RfxMclMessage>& msg);
    void handleNetworkInfo(const sp<RfxMclMessage>& msg);
    void handleGsmFemtoCellInfo(const sp<RfxMclMessage>& msg);
    void handleCdmaFemtoCellInfo(const sp<RfxMclMessage>& msg);
    void handleCellInfoList(const sp<RfxMclMessage>& msg);
    void handleNitzTzReceived(const sp<RfxMclMessage>& msg);
    void handleNitzOperNameReceived(const sp<RfxMclMessage>& msg);
    void handleSib16TimeInfoReceived(const sp<RfxMclMessage>& msg);
    void handleNetworkEventReceived(const sp<RfxMclMessage>& msg);
    void handleMMRRStatusChanged(const sp<RfxMclMessage>& msg);
    void handleWfcStateChanged(const sp<RfxMclMessage>& msg);
    void handleACMT(const sp<RfxMclMessage>& msg);
    void handleModulationInfoReceived(const sp<RfxMclMessage>& msg);
    void handleEnhancedOperatorNameDisplay(const sp<RfxMclMessage>& msg);
    void handlePseudoCellInfo(const sp<RfxMclMessage>& msg);
    void handleNetworkScanResult(const sp<RfxMclMessage>& msg);
    void handleLteNetworkInfo(const sp<RfxMclMessage>& msg);
    void handleMccMncChanged(const sp<RfxMclMessage>& msg);
    void onImsEmergencySupportR9(const sp<RfxMclMessage>& msg);
    void handleCellularQualityReport(const sp<RfxMclMessage>& msg);

    int convertOtaProvisionStatus(int rawState);
    void updatePrlInfo(int system, int mode);
    unsigned int combineWfcEgregState();
    bool enableReportSignalStrengthWithWcdmaEcio();

    const char** allowed_urc;

  protected:
    static int bSIB16Received;
    static int bNitzTzAvailble;
    static int bNitzDstAvailble;
    static char ril_nw_nitz_tz[MAX_NITZ_TZ_DST_LENGTH];
    static char ril_nw_nitz_dst[MAX_NITZ_TZ_DST_LENGTH];
    RIL_PS_BEARER_CACHE ril_ps_bearer_cache;

    /* GPRS network registration status URC value */
    int ril_data_urc_status;
    int ril_data_urc_rat;
};

#endif
