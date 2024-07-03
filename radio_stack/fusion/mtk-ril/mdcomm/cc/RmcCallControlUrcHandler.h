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

#ifndef __RMC_CALL_CONTROL_URC_HANDLER_H__
#define __RMC_CALL_CONTROL_URC_HANDLER_H__

#include "RmcCallControlBaseHandler.h"

class RmcCallControlUrcHandler : public RmcCallControlBaseHandler {
    RFX_DECLARE_HANDLER_CLASS(RmcCallControlUrcHandler);

  public:
    RmcCallControlUrcHandler(int slot_id, int channel_id);
    virtual ~RmcCallControlUrcHandler();

    static int getSpeechCodec();
    static void resetSpeechCodec();

  protected:
    virtual void onHandleUrc(const sp<RfxMclMessage>& msg);

    virtual void onHandleTimer();

  private:
    void handleCallProgressIndicationMessage(const sp<RfxMclMessage>& msg);
    void handleRingMessage(const sp<RfxMclMessage>& msg);
    void handleEspeechMessage(const sp<RfxMclMessage>& msg);
    void handleIncomingCallIndicationMessage(const sp<RfxMclMessage>& msg);
    void handleCipherIndicationMessage(const sp<RfxMclMessage>& msg);
    char* convertUcs2String(char* ucs2str);
    void handleCnapMessage(const sp<RfxMclMessage>& msg);
    void handleSpeechCodecInfo(const sp<RfxMclMessage>& msg);
    void handleCrssNotification(const sp<RfxMclMessage>& msg, int code);
    void handleSuppSvcNotification(const sp<RfxMclMessage>& msg, int notiType);
    void sendRingbackToneNotification(int isStart);
    void handleECPI(char** data);
    bool shoudNotifyCallInfo(int msgType);
    bool shouldNotifyCallStateChanged(int msgType);

    /// C2K specific start
    void handleNoCarrierMessage();
    void handleCdmaCallingPartyNumberInfoMessage(const sp<RfxMclMessage>& msg);
    void handleRedirectingNumberInfoMessage(const sp<RfxMclMessage>& msg);
    void handleLineControlInfoMessage(const sp<RfxMclMessage>& msg);
    void handleExtendedDisplayInfoMessage(const sp<RfxMclMessage>& msg);
    void handleDisplayAndSignalsInfoMessage(const sp<RfxMclMessage>& msg);
    void handleCallControlStatusMessage(const sp<RfxMclMessage>& msg);
    void handleCdmaCallWaitingMessage(const sp<RfxMclMessage>& msg);
    void handleWpsTrafficChannelAssigned();
    /// @}

    char atLog[MAX_AT_RESPONSE] = {0};

    enum {
        STOP_RING_BACK_TONE = 0,
        START_RING_BACK_TONE,
    };

    enum {
        CRSS_CALL_WAITING,
        CRSS_CALLED_LINE_ID_PREST,
        CRSS_CALLING_LINE_ID_PREST,
        CRSS_CONNECTED_LINE_ID_PREST
    };

    enum { SUPP_SVC_CSSI, SUPP_SVC_CSSU };

    typedef enum {
        VOICE = 0,
        VT = 10,
        VOLTE = 20,
        VILTE = 21,
        VOLTE_CONF = 22,
    } Call_Mode;

    /// C2K specific start
    enum {
        IDLE,
        CONNECTED,
        DIALING,
        INCOMING,
        WAITING,
    };

    int mCallState = IDLE;
    const int DISPLAY_TAG_CALLING_PARTY_NAME = 0x8D;
    const int DISPLAY_TAG_ORIG_CALLED_NAME = 0x8F;
    /// @}

    int mIsRingBackTonePlaying = 0;

    /**
     *   MtkSpeechCodecTypes
     *     NONE(0),
     *     QCELP13K(0x0001),
     *     EVRC(0x0002),
     *     EVRC_B(0x0003),
     *     EVRC_WB(0x0004),
     *     EVRC_NW(0x0005),
     *     AMR_NB(0x0006),
     *     AMR_WB(0x0007),
     *     GSM_EFR(0x0008),
     *     GSM_FR(0x0009),
     *     GSM_HR(0x000A);
     */
    static int mSpeechCodec;
};
#endif
