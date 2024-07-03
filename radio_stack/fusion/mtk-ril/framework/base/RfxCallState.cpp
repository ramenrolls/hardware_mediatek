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

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "RfxCallState.h"

/*****************************************************************************
 * Class RfxController
 *****************************************************************************/

RfxCallState::RfxCallState() {
    mCallType = CALL_TYPE_UNKNOWN;
    mCallState = CALL_STATE_UNKNOWN;
    mCallCount = 0;
}

RfxCallState::RfxCallState(CallTypeEnum callType, CallStateEnum callState, int callCount) {
    mCallType = callType;
    mCallState = callState;
    mCallCount = callCount;
}

RfxCallState::~RfxCallState() {}

CallTypeEnum RfxCallState::getCallType() { return mCallType; }

CallStateEnum RfxCallState::getCallState() { return mCallState; }

int RfxCallState::getCallCount() { return mCallCount; }

void RfxCallState::setCallType(CallTypeEnum callType) { mCallType = callType; }

void RfxCallState::setCallState(CallStateEnum callState) { mCallState = callState; }

void RfxCallState::setCallCount(int count) { mCallCount = count; }

bool RfxCallState::hasConnectedC2kCall() {
    return ((mCallType == CALL_TYPE_C2K) && (mCallState == CALL_STATE_CONNECTED) &&
            (mCallCount > 0));
}

bool RfxCallState::hasConnectedGsmCall() {
    return ((mCallType == CALL_TYPE_GSM) && (mCallState == CALL_STATE_CONNECTED) &&
            (mCallCount > 0));
}

bool RfxCallState::hasConnectedImsCall() {
    return ((mCallType == CALL_TYPE_IMS) && (mCallState == CALL_STATE_CONNECTED) &&
            (mCallCount > 0));
}

String8 RfxCallState::toString() {
    return String8::format("call type=%d, call state=%d, call count=%d", mCallType, mCallState,
                           mCallCount);
}
