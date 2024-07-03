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
#include "SmsParserUtils.h"
#include "GsmSmsAddress.h"
#include "SmsAddress.h"
#include "RfxLog.h"

#define RFX_LOG_TAG "GsmSmsAddress"
/*****************************************************************************
 * Class GsmSmsAddress
 *****************************************************************************/
const int GsmSmsAddress::OFFSET_ADDRESS_LENGTH = 0;
const int GsmSmsAddress::OFFSET_TOA = 1;
const int GsmSmsAddress::OFFSET_ADDRESS_VALUE = 2;

GsmSmsAddress::GsmSmsAddress(BYTE* data, int length) {
    mOrigBytes = new BYTE[length];
    memcpy(mOrigBytes, data, length);

    // addressLength is the count of semi-octets, not bytes
    int addressLength = mOrigBytes[OFFSET_ADDRESS_LENGTH] & 0xff;
    int toa = mOrigBytes[OFFSET_TOA] & 0xff;
    mTon = 0x7 & (toa >> 4);
    // TOA must have its high bit set
    if ((toa & 0x80) != 0x80) {
        RFX_LOG_D(RFX_LOG_TAG, "TOA must have its high bit set");
    }
    if (isAlphanumeric()) {
        // An alphanumeric address
        int countSeptets = addressLength * 4 / 7;
    } else {
        // TS 23.040 9.1.2.5 says
        // that "the MS shall interpret reserved values as 'Unknown'
        // but shall store them exactly as received"
        BYTE lastByte = mOrigBytes[length - 1];

        if ((addressLength & 1) == 1) {
            // Make sure the final unused BCD digit is 0xf
            mOrigBytes[length - 1] |= 0xf0;
        }

        // And restore origBytes
        mOrigBytes[length - 1] = lastByte;
    }
}

GsmSmsAddress::~GsmSmsAddress() {
    if (mOrigBytes != NULL) {
        delete mOrigBytes;
        mOrigBytes = NULL;
    }
}
