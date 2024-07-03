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
#include "SmsHeader.h"
#include "RfxLog.h"

#define RFX_LOG_TAG "SmsHeader"
/*****************************************************************************
 * Class SmsHeader
 *****************************************************************************/
const int SmsHeader::PORT_WAP_PUSH = 2948;
const int SmsHeader::PORT_SUPL = 7275;

SmsHeader::SmsHeader() : mLanguageShiftTable(0), mLanguageTable(0) {
    mPortAddrs.destPort = -1;
    mPortAddrs.origPort = -1;
    mPortAddrs.areEightBits = false;
    mConcatRef.refNumber = -1;
    mConcatRef.seqNumber = -1;
    mConcatRef.msgCount = 0;
    mConcatRef.isEightBits = false;
}

SmsHeader::~SmsHeader() {
    for (list<MiscElt>::iterator iter = mMiscEltList.begin(); iter != mMiscEltList.end(); iter++) {
        MiscElt temp = *iter;
        if (temp.data != NULL) {
            delete temp.data;
            temp.data = NULL;
        }
    }
}

SmsHeader* SmsHeader::fromByteArray(BYTE* data, int headerLength) {
    SmsHeader* smsHeader = new SmsHeader();
    int curr = 0;
    while (curr < headerLength) {
        /**
         * NOTE: as defined in the spec, ConcatRef and PortAddr
         * fields should not reoccur, but if they do the last
         * occurrence is to be used.  Also, for ConcatRef
         * elements, if the count is zero, sequence is zero, or
         * sequence is larger than count, the entire element is to
         * be ignored.
         */
        int id = data[curr++];
        int length = data[curr++];
        RFX_LOG_D(RFX_LOG_TAG, "fromByteArray header element length: %d, id: %d", id, length);
        ConcatRef concatRef;
        PortAddrs portAddrs;
        int temp1 = 0;
        int temp2 = 0;
        int temp3 = 0;
        int temp4 = 0;
        switch (id) {
            case ELT_ID_CONCATENATED_8_BIT_REFERENCE: {
                concatRef.refNumber = data[curr++];
                concatRef.msgCount = data[curr++];
                concatRef.seqNumber = data[curr++];
                concatRef.isEightBits = true;
                if (concatRef.msgCount != 0 && concatRef.seqNumber != 0 &&
                    concatRef.seqNumber <= concatRef.msgCount) {
                    smsHeader->mConcatRef = concatRef;
                }
                RFX_LOG_D(RFX_LOG_TAG,
                          "fromByteArray concatRef.refNumber: %d,"
                          "concatRef.msgCount: %d,"
                          "concatRef.seqNumber: %d",
                          concatRef.refNumber, concatRef.msgCount, concatRef.seqNumber);
                break;
            }
            case ELT_ID_CONCATENATED_16_BIT_REFERENCE: {
                temp1 = data[curr++] << 8;
                temp2 = data[curr++];
                concatRef.refNumber = temp1 | temp2;
                concatRef.msgCount = data[curr++];
                concatRef.seqNumber = data[curr++];
                concatRef.isEightBits = false;
                if (concatRef.msgCount != 0 && concatRef.seqNumber != 0 &&
                    concatRef.seqNumber <= concatRef.msgCount) {
                    smsHeader->mConcatRef = concatRef;
                }
                RFX_LOG_D(RFX_LOG_TAG,
                          "fromByteArray concatRef.refNumber: %d,"
                          "concatRef.msgCount: %d,"
                          "concatRef.seqNumber: %d",
                          concatRef.refNumber, concatRef.msgCount, concatRef.seqNumber);
                break;
            }
            case ELT_ID_APPLICATION_PORT_ADDRESSING_8_BIT: {
                portAddrs.destPort = data[curr++];
                portAddrs.origPort = data[curr++];
                portAddrs.areEightBits = true;
                smsHeader->mPortAddrs = portAddrs;
                RFX_LOG_D(RFX_LOG_TAG,
                          "fromByteArray portAddrs.destPort: %d,"
                          "portAddrs.origPort: %d",
                          portAddrs.destPort, portAddrs.origPort);
                break;
            }
            case ELT_ID_APPLICATION_PORT_ADDRESSING_16_BIT: {
                temp1 = data[curr++] << 8;
                temp2 = data[curr++];
                portAddrs.destPort = temp1 | temp2;
                temp3 = data[curr++] << 8;
                temp4 = data[curr++];
                portAddrs.origPort = temp3 | temp4;
                portAddrs.areEightBits = false;
                smsHeader->mPortAddrs = portAddrs;
                RFX_LOG_D(RFX_LOG_TAG,
                          "fromByteArray portAddrs.destPort: %d,"
                          "portAddrs.origPort: %d",
                          portAddrs.destPort, portAddrs.origPort);
                break;
            }
            case ELT_ID_NATIONAL_LANGUAGE_SINGLE_SHIFT: {
                smsHeader->mLanguageShiftTable = data[curr++];
                RFX_LOG_D(RFX_LOG_TAG, "fromByteArray mLanguageShiftTable: %d",
                          smsHeader->mLanguageShiftTable);
                break;
            }
            case ELT_ID_NATIONAL_LANGUAGE_LOCKING_SHIFT: {
                smsHeader->mLanguageTable = data[curr++];
                RFX_LOG_D(RFX_LOG_TAG, "fromByteArray mLanguageShiftTable: %d",
                          smsHeader->mLanguageTable);
                break;
            }
            case ELT_ID_SPECIAL_SMS_MESSAGE_INDICATION: {
                SpecialSmsMsg specialSmsMsg;
                ;
                specialSmsMsg.msgIndType = data[curr++];
                specialSmsMsg.msgCount = data[curr++];
                smsHeader->mSpecialSmsMsgList.push_back(specialSmsMsg);
                RFX_LOG_D(RFX_LOG_TAG,
                          "fromByteArray specialSmsMsg.msgIndType: %d,"
                          "specialSmsMsg.msgCount: %d",
                          specialSmsMsg.msgIndType, specialSmsMsg.msgCount);
                break;
            }
            default: {
                MiscElt miscElt;
                miscElt.id = id;
                miscElt.data = new BYTE[length];
                memcpy(miscElt.data, data + curr, length);
                data += length;
                smsHeader->mMiscEltList.push_back(miscElt);
                RFX_LOG_D(RFX_LOG_TAG, "fromByteArray miscElt.id: %d", miscElt.id);
                break;
            }
        }
    }
    return smsHeader;
}
