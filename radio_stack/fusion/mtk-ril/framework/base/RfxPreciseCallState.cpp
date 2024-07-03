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
#include "RfxPreciseCallState.h"
#include "RfxLog.h"

/*****************************************************************************
 * Class RfxPreciseCallState
 *****************************************************************************/

RfxPreciseCallState::RfxPreciseCallState()
    : mSlot(-1),
      mCallId(-1),
      mCallType(CALL_TYPE_NONE),
      mCallRat(CALL_RAT_NONE),
      mCallDir(CALL_DIR_NONE),
      mCallNumber(""),
      mOrigState(ORIG_NONE) {}

RfxPreciseCallState::~RfxPreciseCallState() {}

OrigState RfxPreciseCallState::RILStateToOrigState(RIL_CallState state) {
    switch (state) {
        case RIL_CALL_ACTIVE:
            return ORIG_ACTIVE;
        case RIL_CALL_HOLDING:
            return ORIG_HOLDING;
        case RIL_CALL_DIALING:
            return ORIG_DIALING;
        case RIL_CALL_ALERTING:
            return ORIG_ALERTING;
        case RIL_CALL_INCOMING:
            return ORIG_INCOMING;
        case RIL_CALL_WAITING:
            return ORIG_WAITING;
        default:
            return ORIG_NONE;
    }
}

bool RfxPreciseCallState::isValid() {
    return (mSlot > -1 && mCallId > 0 && mCallType != CALL_TYPE_NONE && mCallRat != CALL_RAT_NONE &&
            mCallDir != CALL_DIR_NONE && mOrigState != ORIG_NONE);
}

void RfxPreciseCallState::dump() {
    RFX_LOG_D("PreciseCallState", "slot:%d, id:%d, type:%d, rat:%d, dir:%d, num:%s, orig:%d", mSlot,
              mCallId, mCallType, mCallRat, mCallDir, String8(mCallNumber).string(), mOrigState);
}
