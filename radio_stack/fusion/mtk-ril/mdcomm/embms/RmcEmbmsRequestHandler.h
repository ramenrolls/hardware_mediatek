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

#ifndef __RMC_EMBMS_REQUEST_HANDLER_H__
#define __RMC_EMBMS_REQUEST_HANDLER_H__

#include "RfxBaseHandler.h"
#include "RfxStringData.h"
#include "RfxMessageId.h"
#include <telephony/mtk_ril.h>

#include "RfxMisc.h"
#include <sys/socket.h>
#include <sys/stat.h>
#include "RfxAtLine.h"
#include "RfxIntsData.h"
#include "RfxStringData.h"
#include "RfxVoidData.h"
#include "RmcEmbmsUtils.h"
#include "embms/RfxEmbmsGetCoverageRespData.h"
#include "embms/RfxEmbmsLocalEnableRespData.h"
#include "embms/RfxEmbmsDisableRespData.h"
#include "embms/RfxEmbmsLocalStartSessionReqData.h"
#include "embms/RfxEmbmsLocalStartSessionRespData.h"
#include "embms/RfxEmbmsLocalStopSessionReqData.h"
#include "embms/RfxEmbmsGetTimeRespData.h"
#include "embms/RfxEmbmsLocalSessionNotifyData.h"
#include "embms/RfxEmbmsLocalSaiNotifyData.h"
#include "embms/RfxEmbmsLocalOosNotifyData.h"

#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "RmcEmbmsReq"

class RmcEmbmsRequestHandler : public RfxBaseHandler {
    RFX_DECLARE_HANDLER_CLASS(RmcEmbmsRequestHandler);

  public:
    RmcEmbmsRequestHandler(int slot_id, int channel_id);
    virtual ~RmcEmbmsRequestHandler();

  protected:
    virtual void onHandleRequest(const sp<RfxMclMessage>& msg);
    virtual void onHandleEvent(const sp<RfxMclMessage>& msg);

  private:
    void requestEmbmsAt(const sp<RfxMclMessage>& msg);
    void requestLocalEmbmsEnable(const sp<RfxMclMessage>& msg);
    void requestLocalEmbmsDisable(const sp<RfxMclMessage>& msg);
    void requestLocalEmbmsStartSession(const sp<RfxMclMessage>& msg);
    void requestLocalEmbmsStopSession(const sp<RfxMclMessage>& msg);
    void requestLocalEmbmsGetNetworkTime(const sp<RfxMclMessage>& msg);
    void requestLocalEmbmsSetCoverageStatus(const sp<RfxMclMessage>& msg);
    void requestEmbmsGetCoverageStatus(const sp<RfxMclMessage>& msg);
    void requestLocalTriggerCellInfoNotify(const sp<RfxMclMessage>& msg);
    void requestAtGetSaiList_old(const sp<RfxMclMessage>& msg);
    void requestAtGetSaiList(const sp<RfxMclMessage>& msg);
    void requestAtNetworkInfo(const sp<RfxMclMessage>& msg);
    void requestAtBssiSignalLevel(const sp<RfxMclMessage>& msg);
    void requestAtModemStatus(const sp<RfxMclMessage>& msg);
    void requestAtEnableUrcEvents(const sp<RfxMclMessage>& msg);
    void requestAtAvailService(const sp<RfxMclMessage>& msg);
    void requestAtDeviceInfo(const sp<RfxMclMessage>& msg);
    void requestAtSetPreference(const sp<RfxMclMessage>& msg);
    void requestAtNetworkRegSupport(const sp<RfxMclMessage>& msg);
    void requestAtNetworkRegStatus(const sp<RfxMclMessage>& msg);
    void postEpsNetworkUpdate(const sp<RfxMclMessage>& msg);
    void postHvolteUpdate(const sp<RfxMclMessage>& msg);
    void postSaiListUpdate(const sp<RfxMclMessage>& msg);
    void postSessionListUpdate(const sp<RfxMclMessage>& msg);

    int isTmgiEmpty(char* tmgi);
    bool parseSIB16Time(sp<RfxAtResponse> p_response, RIL_EMBMS_GetTimeResp* time_response);
    bool parseNitzTime(sp<RfxAtResponse> p_response, RIL_EMBMS_GetTimeResp* time_response);
    int convertLteRegState(int status, int eAct);
    bool parseCellId(RfxAtLine* p_response, int* status, unsigned int* cell_id);
    bool parseSailist(sp<RfxAtResponse> p_response, RIL_EMBMS_LocalSaiNotify* p_embms_sailist);
    bool isRjilSupport();
    bool isAtCmdEnableSupport();
    void ril_data_setflags(int s, struct ifreq* ifr, int set, int clr);
    void ril_embms_ioctl_init();
    void setNwIntfDown(const char* pzIfName);
    void configureEmbmsNetworkInterface(int interfaceId, int isUp);
    int getSignalStrength(RfxAtLine* line, int* rssi);
    void dummyOkResponse(const sp<RfxMclMessage>& msg);
    void forceEnableMdEmbms(const sp<RfxMclMessage>& msg);

  private:
    static int embms_sock_fd;
    static int embms_sock6_fd;
    static const char* CCMNI_IFNAME_CCMNI;
    RIL_EMBMS_LocalSessionNotify g_active_session;
    RIL_EMBMS_LocalSessionNotify g_available_session;
};

#endif
