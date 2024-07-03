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
#include "RtcSmsHeader.h"

/*****************************************************************************
 * Constants
 *****************************************************************************/
static const int ELT_ID_CONCATENATED_8_BIT_REFERENCE = 0x00;
static const int HEADER_ID_AND_HEADER_LEN_SIZE = 2;
static const int BYTE_BIT_MASK = 0xff;

/*****************************************************************************
 * Class RtcSmsUserDataHeader
 *****************************************************************************/
RtcSmsUserDataHeader::RtcSmsUserDataHeader(unsigned char* header, int len, bool is3Gpp2)
    : mConcatSms(false), mError(false) {
    int offset = 0;
    int headerId = header[offset++] & BYTE_BIT_MASK;
    int headerLen = header[offset++] & BYTE_BIT_MASK;
    if ((headerLen <= len + HEADER_ID_AND_HEADER_LEN_SIZE) &&
        (headerId == ELT_ID_CONCATENATED_8_BIT_REFERENCE)) {
        mConcatRef.refNumber = header[offset++] & BYTE_BIT_MASK;
        mConcatRef.msgCount = header[offset++] & BYTE_BIT_MASK;
        mConcatRef.seqNumber = header[offset++] & BYTE_BIT_MASK;
        mConcatRef.isEightBits = true;
        mConcatRef.is3Gpp2 = is3Gpp2;
        if (mConcatRef.msgCount != 0 && mConcatRef.seqNumber != 0 &&
            mConcatRef.seqNumber <= mConcatRef.msgCount) {
            mConcatSms = true;
        }
    } else {
        setError();
    }
    if (offset != len) {
        // Only support normal concatenated sms
        setError();
    }
}
