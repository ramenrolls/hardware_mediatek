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

#ifndef __SMS_PARSER_UTILS_H__
#define __SMS_PARSER_UTILS_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include <string>
using namespace std;

typedef unsigned char BYTE;

typedef enum {
    ENCODING_UNKNOWN = 0,
    ENCODING_7BIT,
    ENCODING_8BIT,
    ENCODING_16BIT,
    ENCODING_KSC5601
} SmsDataCodingScheme;

typedef enum { CLASS_UNKNOWN = 0, CLASS_0, CLASS_1, CLASS_2, CLASS_3 } MessageClass;

/*****************************************************************************
 * Class SmsMessage
 *****************************************************************************/
class PhoneNumberUtils {
  public:
    static string prependPlusToNumber(string number);
    static string numToString(int number);
    static BYTE* hexStringToBytes(string s);
    static string stringFromBytes(BYTE* data, int offset, int byteCount);

  private:
    static string internalCalledPartyBCDFragmentToString(BYTE* bytes, int offset, int length);
    static char bcdToChar(BYTE b);
    static int hexCharToInt(char c);

  private:
    static const int TOA_INTERNATIONAL;
    static const char REPLACEMENT_CHAR;
    static const char PAUSE;
    static const char WAIT;
    static const char WILD;
};

class GsmAlphabet {
  public:
    static string gsm7BitPackedToString(BYTE* pdu, int offset, int lengthSeptets);
    static string gsm7BitPackedToString(BYTE* pdu, int offset, int lengthSeptets,
                                        int numPaddingBits, int languageTable, int shiftTable);
    static string gsm8BitUnpackedToString(BYTE* data, int offset, int length);

  public:
    static const BYTE GSM_EXTENDED_ESCAPE;

  private:
    static const string* sLanguageTables;
    static const string* sLanguageShiftTables;
};
#endif /* __SMS_PARSER_UTILS_H__ */
