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

#ifndef __RFX_CALL_STATE_H
#define __RFX_CALL_STATE_H

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "Rfx.h"
#include <utils/String8.h>

using ::android::String8;

/* Define call type. */
typedef enum {
    CALL_TYPE_UNKNOWN = -1,
    CALL_TYPE_C2K = 0,
    CALL_TYPE_GSM = 1,
    CALL_TYPE_IMS = 2
} CallTypeEnum;

/* Define call state. */
typedef enum {
    CALL_STATE_UNKNOWN = -1,
    CALL_STATE_CONNECTED = 0,
    CALL_STATE_DISCONNECTED = 1
} CallStateEnum;

/*****************************************************************************
 * Class RfxController
 *****************************************************************************/
class RfxCallState {
  public:
    RfxCallState();

    RfxCallState(CallTypeEnum callType, CallStateEnum callState, int callCount);

    ~RfxCallState();

  public:
    CallTypeEnum getCallType();
    CallStateEnum getCallState();
    int getCallCount();
    void setCallType(CallTypeEnum callType);
    void setCallState(CallStateEnum callState);
    void setCallCount(int count);
    String8 toString();

  private:
    CallTypeEnum mCallType;
    CallStateEnum mCallState;
    int mCallCount;

  public:
    bool hasConnectedC2kCall();
    bool hasConnectedGsmCall();
    bool hasConnectedImsCall();

    bool equalTo(const RfxCallState& other) const {
        return (mCallType == other.mCallType) && (mCallState == other.mCallState) &&
               (mCallCount == other.mCallCount);
    }

    RfxCallState& operator=(const RfxCallState& other) {
        mCallType = other.mCallType;
        mCallState = other.mCallState;
        mCallCount = other.mCallCount;
        return *this;
    }

    bool operator==(const RfxCallState& other) { return equalTo(other); }

    bool operator!=(const RfxCallState& other) { return !equalTo(other); }
};

#endif /* __RFX_NW_CALL_STATE_H */
