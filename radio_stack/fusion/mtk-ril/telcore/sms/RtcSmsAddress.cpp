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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "RtcSmsAddress.h"

/*****************************************************************************
 * RtcSmsAddress
 *****************************************************************************/
RtcSmsAddress::RtcSmsAddress() : mTon(TON_UNKNOWN), mError(false) {}

/*****************************************************************************
 * RtcGsmSmsAddress
 *****************************************************************************/
RtcGsmSmsAddress::RtcGsmSmsAddress(unsigned char* data, int offset, int length) {
    if ((data != NULL) && (length <= MAX_BUFFER_SIZE)) {
        memcpy(mOrigBytes, data + offset, length);
        int addressLength = mOrigBytes[OFFSET_ADDRESS_LENGTH];
        int toa = mOrigBytes[OFFSET_TOA] & 0xff;
        setTon(0x7 & (toa >> 4));
        if ((toa & 0x80) != 0x80) {
            setError();
            return;
        }
        if (isAlphanumeric()) {
            // Not support
            setError();
            return;
        } else {
            char lastByte = mOrigBytes[length - 1];

            if ((addressLength & 1) == 1) {
                // Make sure the final unused BCD digit is 0xf
                mOrigBytes[length - 1] |= 0xf0;
            }
            bool ret = calledPartyBCDFragmentToString(mOrigBytes, OFFSET_ADDRESS_VALUE,
                                                      length - OFFSET_ADDRESS_VALUE, getAddr());
            if (!ret) {
                setError();
            }
            // And restore origBytes
            mOrigBytes[length - 1] = lastByte;
        }
    } else {
        setError();
    }
}

bool RtcGsmSmsAddress::bcd2Char(unsigned char bcd, String8& addr) {
    if (bcd < 0xa) {
        addr.appendFormat("%d", bcd);
        return true;
    } else {
        return false;
    }
}

bool RtcGsmSmsAddress::calledPartyBCDFragmentToString(unsigned char* bcd, int offset, int length,
                                                      String8& addr) {
    for (int i = offset; i < length + offset; i++) {
        if (!bcd2Char((bcd[i] & 0xf), addr)) {
            return false;
        }
        unsigned char b = (bcd[i] >> 4) & 0xf;
        if (b == 0xf && i + 1 == length + offset) {
            // ignore final 0xf
            break;
        }
        if (!bcd2Char(b, addr)) {
            return false;
        }
    }
    return true;
}

/*****************************************************************************
 * RtcCdmaSmsAddress
 *****************************************************************************/
RtcCdmaSmsAddress::RtcCdmaSmsAddress(RIL_CDMA_SMS_Address& address) {
    if (address.digit_mode == RIL_CDMA_SMS_DIGIT_MODE_4_BIT) {
        CdmaSmsAddress* pAddr;
        RFX_OBJ_CREATE_EX(pAddr, CdmaSmsAddress, NULL, (address, true));
        RFX_ASSERT(pAddr != NULL);
        pAddr->getAddress(getAddr());
        RFX_OBJ_CLOSE(pAddr);
    } else {
        setError();
    }
}
