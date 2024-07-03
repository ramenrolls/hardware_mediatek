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

#ifndef __RMC_NETWORK_REQUEST_HANDLER_H__
#define __RMC_NETWORK_REQUEST_HANDLER_H__

#include "RmcNetworkHandler.h"

#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "RmcNwRTReqHdlr"

class RmcNetworkRealTimeRequestHandler : public RmcNetworkHandler {
    RFX_DECLARE_HANDLER_CLASS(RmcNetworkRealTimeRequestHandler);

  public:
    RmcNetworkRealTimeRequestHandler(int slot_id, int channel_id);
    virtual ~RmcNetworkRealTimeRequestHandler();

  protected:
    virtual void onHandleRequest(const sp<RfxMclMessage>& msg);

  private:
    void resetVoiceRegStateCache(RIL_VOICE_REG_STATE_CACHE* voiceCache, RIL_CACHE_GROUP source);
    void resetDataRegStateCache(RIL_DATA_REG_STATE_CACHE* dataCache, RIL_CACHE_GROUP source);
    void updateVoiceRegStateCache(RIL_VOICE_REG_STATE_CACHE* voice, int source, int count);
    void updateDataRegStateCache(RIL_DATA_REG_STATE_CACHE* data, int source, int count);
    void combineVoiceRegState(const sp<RfxMclMessage>& msg);
    int convertToAndroidRegState(unsigned int uiRegState);
    void combineDataRegState(const sp<RfxMclMessage>& msg);
    void requestVoiceRegistrationState(const sp<RfxMclMessage>& msg);
    int requestVoiceRegistrationStateCdma(const sp<RfxMclMessage>& msg);
    void printVoiceCache(RIL_VOICE_REG_STATE_CACHE cache);
    void printDataCache();
    void requestDataRegistrationState(const sp<RfxMclMessage>& msg);
    void requestDataRegistrationStateGsm();
    void requestOperator(const sp<RfxMclMessage>& msg);
    void requestQueryNetworkSelectionMode(const sp<RfxMclMessage>& msg);
    void updateCaBandInfo();
    void resetCaCache(RIL_CA_CACHE* cacache);
    void updateDcStatus(int cell, int max);
    void updateServiceStateValue();
    void sendVoiceRegResponse(const sp<RfxMclMessage>& msg);
    void sendDataRegResponse(const sp<RfxMclMessage>& msg);
    int isCdmaVoiceInFemtocell();
    int isCdmaDataInFemtocell();
    int isInFemtocell(char* sector_id, char* subnet_mask, int network_id, int radio_technology);

  protected:
    int m_slot_id;
    int m_channel_id;
    int m_emergency_only;
    int m_ps_roaming_ind = 0;
    int m_cdma_emergency_only = 0;
    int m_ims_ecc_only = 0;
};

#endif
