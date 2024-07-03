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

#ifndef __RFX_PRECISE_CALL_STATE_H
#define __RFX_PRECISE_CALL_STATE_H

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "Rfx.h"
#include "utils/String16.h"
#include "utils/String8.h"

using ::android::String16;
using ::android::String8;

typedef enum {
    CALL_TYPE_NONE = -1,
    CALL_TYPE_EMERGENCY = 0,
    CALL_TYPE_VOICE = 1,
    CALL_TYPE_VIDEO = 2,
} CallType;

typedef enum {
    CALL_RAT_NONE = -1,
    CALL_RAT_NO_SERIVCE = 0,
    CALL_RAT_GSM = 1,
    CALL_RAT_UMTS = 2,
    CALL_RAT_LTE = 3,
    CALL_RAT_CDMA = 4,
    CALL_RAT_WIFI = 5,
    CALL_RAT_NR = 6,
} CallRat;

typedef enum {
    CALL_DIR_NONE = -1,
    CALL_DIR_MO = 0,
    CALL_DIR_MT = 1,
} CallDirection;

typedef enum {
    ORIG_NONE = -1,
    ORIG_ACTIVE = 0,
    ORIG_HOLDING = 1,
    ORIG_DIALING = 2,  /* MO call only */
    ORIG_ALERTING = 3, /* MO call only */
    ORIG_INCOMING = 4, /* MT call only */
    ORIG_WAITING = 5,  /* MT call only */
    ORIG_DISCONNECTING = 6,
    ORIG_FOREGND_DISCONNECTING = 7,
    ORIG_BACKGND_DISCONNECTING = 8,
    ORIG_DISCONNECTED = 9,
} OrigState;

/*****************************************************************************
 * Class RfxPreciseCallState
 *****************************************************************************/

class RfxPreciseCallState {
  public:
    RfxPreciseCallState();
    virtual ~RfxPreciseCallState();
    static OrigState RILStateToOrigState(RIL_CallState state);
    bool isValid();
    void dump();

  public:
    int mSlot;
    int mCallId;
    CallType mCallType;
    CallRat mCallRat;
    CallDirection mCallDir;
    String16 mCallNumber;
    OrigState mOrigState;
};

#endif /* __RFX_PRECISE_CALL_STATE_H */
