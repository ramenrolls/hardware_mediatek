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

#ifndef __RFX_IMS_CALL_INFO_H
#define __RFX_IMS_CALL_INFO_H

/*****************************************************************************
 * Include
 *****************************************************************************/
#include <vector>

#include "Rfx.h"
#include "utils/String8.h"
#include "utils/Vector.h"

using ::android::String8;
using ::android::Vector;

/*****************************************************************************
 * Class RfxImsCallInfo
 *****************************************************************************/
class RfxImsCallInfo {
  public:
    enum {
        STATE_ESTABLISHING,
        STATE_DIALING,
        STATE_ALERTING,
        STATE_ESTABLISHED,
        STATE_HELD,
        STATE_INCOMING,
        STATE_WAITING,
        STATE_TERMINATING,
        STATE_TERMINATED,
    };

    enum {
        IMS_VOICE = 20,
        IMS_VIDEO = 21,
        IMS_VOICE_CONF = 22,
        IMS_VIDEO_CONF = 23,
        IMS_VOICE_CONF_PARTS = 24,
        IMS_VIDEO_CONF_PARTS = 25,
    };

    RfxImsCallInfo();

    RfxImsCallInfo(int callId, int callState, String8 number, bool isConference,
                   bool isConferenceHost, bool isEcc, bool isVideo, bool isMT);
    explicit RfxImsCallInfo(RfxImsCallInfo* call);

    ~RfxImsCallInfo();

  public:
    int getCallId();
    int getCallState();
    String8 getNumber();
    bool isConference();
    bool isConferenceHost();
    bool isEcc();
    bool isVideo();
    bool isMT();
    String8 getName();
    Vector<RfxImsCallInfo*> getParticipantList();
    void setCallId(int callId);
    void setCallState(int callState);
    void setNumber(String8 number);
    void setIsConference(bool isConference);
    void setIsConferenceHost(bool isConferenceHost);
    void setIsEcc(bool isEcc);
    void setIsVideo(bool isVideo);
    void setIsMT(bool isMT);
    void setName(String8 name);
    String8 toString();
    RIL_CallState getRilCallState();
    bool updateByCallMode(int callMode);
    void addParticipant(RfxImsCallInfo* call);
    void removeParticipant(int index);
    void removeParticipant(RfxImsCallInfo* call);
    int getCallMode();

  private:
    int mCallId;
    int mCallState;
    String8 mNumber;
    bool mIsConference;
    bool mIsConferenceHost;
    bool mIsEcc;
    bool mIsVideo;
    bool mIsMT;
    String8 mName;
    Vector<RfxImsCallInfo*> mConferenceParticipant;

  public:
    bool equalTo(const RfxImsCallInfo& other) const {
        return (mCallId == other.mCallId) && (mCallState == other.mCallState) &&
               (mNumber == other.mNumber) && (mIsConference == other.mIsConference) &&
               (mIsConferenceHost == other.mIsConferenceHost) && (mIsEcc == other.mIsEcc) &&
               (mIsVideo == other.mIsVideo) && (mIsMT == other.mIsMT) && (mName == other.mName);
    }

    RfxImsCallInfo& operator=(const RfxImsCallInfo& other) {
        mCallId = other.mCallId;
        mCallState = other.mCallState;
        mNumber = other.mNumber;
        mIsConference = other.mIsConference;
        mIsConferenceHost = other.mIsConferenceHost;
        mIsEcc = other.mIsEcc;
        mIsVideo = other.mIsVideo;
        mIsMT = other.mIsMT;
        mName = other.mName;
        return *this;
    }

    bool operator==(const RfxImsCallInfo& other) { return equalTo(other); }

    bool operator!=(const RfxImsCallInfo& other) { return !equalTo(other); }
};

#endif /* __RFX_IMS_CALL_INFO_H */
