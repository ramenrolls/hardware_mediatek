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

#ifndef __SMS_HEADER_H__
#define __SMS_HEADER_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include <list>
#include "SmsParserUtils.h"
#include "SmsHeader.h"

typedef struct {
    int destPort;
    int origPort;
    bool areEightBits;
} PortAddrs;

typedef struct {
    int refNumber;
    int seqNumber;
    int msgCount;
    bool isEightBits;
} ConcatRef;

typedef struct {
    int msgIndType;
    int msgCount;
} SpecialSmsMsg;

typedef struct {
    int id;
    BYTE* data;
} MiscElt;

/*****************************************************************************
 * Class SmsHeader
 *****************************************************************************/
class SmsHeader {
  public:
    SmsHeader();
    virtual ~SmsHeader();
    static SmsHeader* fromByteArray(BYTE* data, int headerLength);

  public:
    ConcatRef mConcatRef;
    PortAddrs mPortAddrs;
    int mLanguageShiftTable;
    int mLanguageTable;
    list<SpecialSmsMsg> mSpecialSmsMsgList;
    list<MiscElt> mMiscEltList;
    static const int PORT_WAP_PUSH;
    static const int PORT_SUPL;

  private:
    static const int ELT_ID_CONCATENATED_8_BIT_REFERENCE = 0x00;
    static const int ELT_ID_CONCATENATED_16_BIT_REFERENCE = 0x08;
    static const int ELT_ID_APPLICATION_PORT_ADDRESSING_8_BIT = 0x04;
    static const int ELT_ID_APPLICATION_PORT_ADDRESSING_16_BIT = 0x05;
    static const int ELT_ID_NATIONAL_LANGUAGE_SINGLE_SHIFT = 0x24;
    static const int ELT_ID_NATIONAL_LANGUAGE_LOCKING_SHIFT = 0x25;
    static const int ELT_ID_SPECIAL_SMS_MESSAGE_INDICATION = 0x01;
};

#endif /* __SMS_HEADER_H__ */
