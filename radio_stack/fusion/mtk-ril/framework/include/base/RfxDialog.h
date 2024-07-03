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

#ifndef RFX_DIALOG_H_
#define RFX_DIALOG_H_

#include <stdio.h>
#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <telephony/mtk_ril.h>
#include "utils/RefBase.h"

using namespace std;
using ::android::RefBase;
using ::android::sp;

/**
 * Represents mediaAttributes element in Dialog Event Package.
 */
class RfxMediaAttribute : public virtual RefBase {
  public:
    RfxMediaAttribute();
    virtual ~RfxMediaAttribute();

  public:
    string mediaType;
    string mediaDirection;
    bool port0;
};

/**
 * Represents dialog element in Dialog Event Package.
 */
class RfxDialog : public virtual RefBase {
  public:
    RfxDialog();
    virtual ~RfxDialog();

  public:
    static const string AUDIO;
    static const string VIDEO;
    static const string SIP_RENDERING;
    static const string NO;

    static const string CONFIRMED;
    static const string EARLY;
    static const string TRYING;
    static const string PROCEEDING;
    static const string TERMINATED;

    static const string INACTIVE;
    static const string SENDRECV;
    static const string SENDONLY;
    static const string RECVONLY;

    typedef enum {
        CALL_STATE_CONFIRMED = 1,
        CALL_STATE_TERMINATED = 2,
        CALL_STATE_TRYING = 3,
        CALL_STATE_PROCEEDING = 4,
        CALL_STATE_EARLY = 5,
    } DialogCallState;

    // call type is corresponding to value in ImsCallProfile
    typedef enum {
        CALL_TYPE_VOICE = 2,
        CALL_TYPE_VT = 4,
        CALL_TYPE_VT_TX = 5,
        CALL_TYPE_VT_RX = 6,
        CALL_TYPE_VT_NODIR = 7,
    } DialogCallType;

  public:
    int dialogId;
    bool exclusive;
    string state;
    bool initiator;

    // local start
    string identity;
    string targetUri;
    vector<sp<RfxMediaAttribute>> mediaAttributes;

    // Param start
    string pname;
    string pval;
    // Param end
    // local end

    // remote start
    string remoteIdentity;
    string remoteTargetUri;

    // Param start
    string remotePname;
    string remotePval;
    // Param end
    // remote end
  public:
    int getDialogId();
    string getAddress();
    string getRemoteAddress();
    bool isMt();
    bool isCallHeld();
    bool isPullable();
    int getCallState();
    int getCallType();

  private:
    bool isCallHeld(vector<sp<RfxMediaAttribute>> mediaAttributes);
    bool isVideoCallInBackground();
};

#endif /* DIALOG_H_ */
