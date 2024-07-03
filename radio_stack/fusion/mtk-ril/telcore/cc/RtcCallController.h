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

#ifndef __RFX_CALL_CONTROLLER_H__
#define __RFX_CALL_CONTROLLER_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include <map>
#include <vector>
#include <string>

#include "RfxImsCallInfo.h"
#include "ims/RtcImsConferenceCallMessageHandler.h"
#include "RfxPreciseCallState.h"
#include "RfxTimer.h"
#include "RtcRedialController.h"
#include "utils/String16.h"
#include "utils/String8.h"
#include "utils/Vector.h"

using ::android::String16;
using ::android::String8;
using ::android::Vector;

using std::string;

#define MAX_ADDRESS_LEN 40

/*****************************************************************************
 * Class RfxController
 *****************************************************************************/

class RfxStringsData;
class RfxImsCallInfo;

class RtcCallController : public RfxController {
    // Required: declare this class
    RFX_DECLARE_CLASS(RtcCallController);

  public:
    RtcCallController();
    virtual ~RtcCallController();
    void onParticipantsUpdate(vector<sp<ConferenceCallUser>> users, bool autoTerminate);
    void onMergeStart();
    void retrieveMergeFail(int activeCallIdBeforeMerge);

    void notifyCallInfoUpdate(const sp<RfxMessage>& message);

    // Override
  protected:
    virtual void onInit();
    virtual void onDeinit();
    virtual bool onHandleRequest(const sp<RfxMessage>& message);
    virtual bool onHandleUrc(const sp<RfxMessage>& message);
    virtual bool onHandleResponse(const sp<RfxMessage>& message);
    virtual bool onPreviewMessage(const sp<RfxMessage>& message);
    virtual bool onCheckIfResumeMessage(const sp<RfxMessage>& message);
    virtual bool onCheckIfRejectMessage(const sp<RfxMessage>& message, bool isModemPowerOff,
                                        int radioState);
    virtual void createRedialController();

    void responseDialFailed(const sp<RfxMessage>& message);
    // error handling
    bool rejectDualDialForDSDS();
    // handle Ims call
    void handleImsDialRequest(const sp<RfxMessage>& message);
    void handleImsDialFailResponse(const sp<RfxMessage>& message);
    void imsCallEstablishFailed(int slotId);
    void responseToRilJAndUpdateIsImsCallExist(const sp<RfxMessage>& msg);
    void updateIsImsCallExistToStatusManager(int slotId);

    RtcRedialController* mRedialCtrl;

  private:
    void onCsPhoneChanged(RfxStatusKeyEnum key, RfxVariant oldValue, RfxVariant newValue);

    void onServiceStateChanged(RfxStatusKeyEnum key, RfxVariant oldValue, RfxVariant newValue);

    void onRadioStateChanged(RfxStatusKeyEnum key, RfxVariant old_value, RfxVariant value);

    void onCardTypeChanged(RfxStatusKeyEnum key, RfxVariant oldValue, RfxVariant newValue);
    bool isCsCallExistAndNoEccExist();
    bool isCsActiveCallExistAndNoEccExist();

    bool handleCsCallInfoUpdate(const sp<RfxMessage>& msg);

    bool isInChina();

    // handle Ims call
    bool hasImsCall(int slotId);
    bool handleIncomingCall(int slotId, RfxStringsData* data);
    bool handleImsCallInfoUpdate(const sp<RfxMessage>& msg);
    void handleSrvcc(int slotId, const sp<RfxMessage>& msg);
    bool isDuringSrvcc(int slotId);
    void addImsCall(int slotId, RfxImsCallInfo* call);
    void removeImsCall(int slotId, RfxImsCallInfo* call);
    void assignImsCallId(int slotId, int callId);
    bool updateNumberFromEcpiNumOrPau(RfxImsCallInfo* targetCall, string ecpiNumber, string pau);
    bool updateNumberFromPau(RfxImsCallInfo* targetCall, string pau);
    bool updateDisplayNameFromPau(RfxImsCallInfo* targetCall, string pau);
    string getFieldValueFromPau(string pau, string field);
    void clearAllImsCalls(int slotId);
    void releaseEstablishingCall(int slotId);
    bool waitCallRingForMT(const sp<RfxMessage>& msg);
    void handleCallRing(int slotId);
    bool handleImsCallModeChange(const sp<RfxMessage>& msg);
    void clearCallRingCache(int slotId);
    void generateImsConference(int slotId, int callId);
    void handleImsStkCall(int slotId, char** params);

    // forceRelease
    bool hasPendingHangupRequest(bool isForegnd);
    bool hasPendingHangupRequest(int hangupCallId);

    // call state cache
    void handleCdmaFlashRequest(const sp<RfxMessage>& message);
    void handleCdmaCallWait();
    void handleGetCurrentCallsResponse(const sp<RfxMessage>& message);
    Vector<RfxPreciseCallState*>* parsePreciseCallState(RIL_Call** pp_calls, int count);
    void updateDisconnecting(Vector<RfxPreciseCallState*>* origList, int hangupCallId);
    void updateDisconnecting(Vector<RfxPreciseCallState*>* origList, bool isForegnd);
    void updateDisconnected(Vector<RfxPreciseCallState*>* oldList,
                            Vector<RfxPreciseCallState*>* newList);
    void freePreciseCallStateList(Vector<RfxPreciseCallState*>* list);
    void updatePreciseCallStateList(RfxPreciseCallState* preciseCallState,
                                    Vector<RfxPreciseCallState*>* list);

    // error handling
    RIL_CALL_INFO_TYPE shouldRejectIncomingCall(int callMode);
    void approveIncoming(int callId, int seqNo, bool approve);

    bool canHandleCsDialRequest(const sp<RfxMessage>& message);

    bool canHandleHangupForegndOrBackgndRequest(const sp<RfxMessage>& message);

    void unsetEccModeLocally();
    /* ALPS03346578: Emergency dial can be handled after receiving response of
        RFX_MSG_REQUEST_SET_ECC_MODE */
    bool canHandleEmergencyDialRequest(const sp<RfxMessage>& message);
    RfxImsCallInfo* getCallByCallId(int slotId, int callId);
    void updateCallCount();
    int getValidImsCallCount();

    int getPendingMTCallId();

    bool handlePendingMTCallMode(const sp<RfxMessage>& msg);
    void handleAsyncCallControlResult(const sp<RfxMessage>& message);
    void handleAsyncCallControlResponse(const sp<RfxMessage>& message);
    void handleAsyncImsCallControlRequest(const sp<RfxMessage>& message);
    bool shouldDoAsyncImsCallControl();

    void handleImsEventPackageIndication(const sp<RfxMessage>& msg);
    void hangupIncomingCall();
    bool handleSetCallIndicationRequest(const sp<RfxMessage>& msg);
    bool reportFakeImsRingingCall(const sp<RfxMessage>& msg, int seqNo);
    void maybeSendNoEcbm(bool isForce);

    void handleAutoAnswer(const sp<RfxMessage>& msg);
    void onAutoAnswerTimer();
    void handleSpeechCodecInfo(const sp<RfxMessage>& msg);
    void handleImsGetCurrentCalls(const sp<RfxMessage>& msg);
    void handleImsHangupWaitingOrBackground(const sp<RfxMessage>& msg);
    void handleImsHangupFgResumeBg(const sp<RfxMessage>& msg);
    void imsCallToRilCall(int& index, RfxImsCallInfo* call, RIL_Call* p_calls);
    void checkIfResumeBg(int slotId);
    void handleImsSwitchFgBg(const sp<RfxMessage>& msg);
    bool canHandleRequestDuringComboOperations(const sp<RfxMessage>& message);
    Vector<RfxImsCallInfo*> getWaitingCall(Vector<RfxImsCallInfo*> calls);
    Vector<RfxImsCallInfo*> getHoldingCall(Vector<RfxImsCallInfo*> calls);
    Vector<RfxImsCallInfo*> getActiveCall(Vector<RfxImsCallInfo*> calls);
    RfxImsCallInfo* getImsCallByCallId(Vector<RfxImsCallInfo*> calls, int callId);
    Vector<RfxImsCallInfo*> getConferenceHostCall(Vector<RfxImsCallInfo*> calls);
    void dumpImsCallList(Vector<RfxImsCallInfo*> calls);
    void handleEconfResult(const sp<RfxMessage>& msg);
    bool canHandleImsGetCurrentCallsRequest(const sp<RfxMessage>& message);
    bool allowImsIncomingOnAnotherSlot();
    void hangupIncomingCallOnAllSlot();
    bool handleClirSpecial(const sp<RfxMessage>& msg);
    bool canHandleAnswerRequest(const sp<RfxMessage>& message);
    bool onHandleRequestForImsFakeMode(const sp<RfxMessage>& message);
    bool onHandleResponseForImsFakeMode(const sp<RfxMessage>& message);
    void resetImsFakeMode();
    void onAutoApproveTimer();

    void updateRQ(bool RQ);

    void onSendDelayCallInfoTimer();
    void onAutoFakeAcceptTimer();

    CallRat mCallRat;
    CallRat mImsCallRat;
    char* mEccNumber;
    char mEccNumberBuffer[MAX_ADDRESS_LEN + 1];
    int mEccState;
    enum {
        STATE_DIALING,
        STATE_ALERTING,
        STATE_ACTIVE,
        STATE_DISCONNECTED,
    };

    Vector<RfxPreciseCallState*>* mPreciseCallStateList;
    bool mUseLocalCallFailCause;
    int mDialLastError;
    bool mEspeechOnInDialingEcc;
    int mCsCallCount;

    // key = slot, value = call
    std::map<int, Vector<RfxImsCallInfo*>> mImsCalls;
    std::map<int, RfxImsCallInfo*> mEstablishingCall;
    std::map<int, sp<RfxMessage>> mPendingCallInfoForMT;
    std::map<int, bool> mCallRingIndicated;
    std::map<int, int> mSrvccState;
    sp<RfxMessage> mPendingCallControlMessage;
    int mPendingIncomingCallId;
    Vector<RfxImsCallInfo*> mToBeMergedCalls;
    int mAutoAnswerImsIncoming;
    int mAutoAnswerCallId;

    bool mWaitForSwitchCallResp;

    /* ALPS03346578: Emergency dial can be handled after receiving response of
        RFX_MSG_REQUEST_SET_ECC_MODE */
    bool mWaitForSetEccModeResponse;
    int mPendingSrvccCallCount;
    bool mNeedResumeOrAnswerCall;
    int mExpectedToResumeCallId;
    int mSpeechCodec;
    // For combo operations.
    int mCallIdToBeHeld;
    int mCallIdToBeHangup;
    int mCallIdExpectToResumeOrAnswer;
    int mCallIdResuming;
    int mCallIdAnswering;
    sp<RfxMessage> mPendingImsStkCallInfoMessage;
    sp<RfxMessage> mDelaySendCallInfoMessage;
    Vector<int> mCallIdsDisconnecting;

    bool handleDialAsEmergencyDial(const sp<RfxMessage>& message);
    void handleRedialEcc(const sp<RfxMessage>& urc);
    void handleEmergencyBearerSupportNotify(const sp<RfxMessage>& message);
    sp<RfxMessage> mOriginalDialMessage;
    sp<RfxMessage> mImsDialMessage;
    sp<RfxMessage> mHangupWaitingOrBgMessage;
    sp<RfxMessage> mHangupFgResumeBgMessage;
    sp<RfxMessage> mSwitchFgBgMessage;
    int mSeqNo;
    sp<RfxMessage> mOriginalAnswerMessage;
    sp<RfxMessage> mOriginalHangupMessage;
    bool mImsFakeMode;

    TimerHandle mAutoAnswerTimerHandle;
    TimerHandle mAutoApproveTimerHandle;
    static const int AUTO_ANSWER_NONE = 0;
    static const int AUTO_ANSWER_EM = 1;
    static const int AUTO_ANSWER_FAKE = 2;

    TimerHandle mSendImsCallInfoTimerHandle;

    const char* PAU_NUMBER_FIELD = "<tel:";
    const char* PAU_NAME_FIELD = "<name:";
    const char* PAU_SIP_NUMBER_FIELD = "<sip:";
    const char* PAU_END_FLAG_FIELD = ">";

    const int SRVCC_STATE_NONE = -1;
    const int SRVCC_STATE_STARTED = 0;
    const int SRVCC_STATE_COMPLETED = 1;
    const int SRVCC_STATE_FAILED = 2;
    const int SRVCC_STATE_CANCELED = 3;

    // Convert CHLD=0 to CHLD=1x for hanging up ringing call
    sp<RfxMessage> mOriginalHangupRingingMessage;
    int findRingingConnection();

    bool mRQ;

    TimerHandle mAutoFakeAcceptTimerHandle;
};

#endif /* __RFX_CALL_CONTROLLER_H__ */
