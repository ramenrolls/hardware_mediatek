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

#ifndef __PDU_PARSER_H__
#define __PDU_PARSER_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include <map>
#include "SmsParserUtils.h"

class SmsHeader;
class GsmSmsAddress;
class SmsMessage;
/*****************************************************************************
 * Class PduParser
 *****************************************************************************/

class PduParser {
  public:
    PduParser(BYTE* pdu, int length);
    virtual ~PduParser();
    string getSCAddress();
    int getByte();
    GsmSmsAddress* getAddress();
    int constructUserData(bool hasUserDataHeader, bool dataInSeptets);
    int getUserDataLength();
    BYTE* getUserData();
    SmsHeader* getUserDataHeader();
    long getSCTimestampMillis();

  private:
    BYTE* mPdu;
    int mCur;
    int mUserDataSeptetPadding;
    SmsHeader* mUserDataHeader;
    BYTE* mUserData;
    int mUserDataLength;
    int mPduLength;
};

class WappushPduParser {
  public:
    WappushPduParser(BYTE* pdu, int length);
    long getValue32();
    int getDecodedDataLength();
    bool decodeContentType(int startIndex);
    bool decodeConstrainedEncoding(int startIndex);
    bool decodeIntegerValue(int startIndex);
    bool decodeShortInteger(int startIndex);
    bool decodeLongInteger(int startIndex);
    bool decodeTokenText(int startIndex);
    void decodeHeaders(int startIndex, int headerLength);
    bool decodeHeaderFieldName(int startIndex);
    bool decodeTextString(int startIndex);
    void expandWellKnownHeadersName();
    string getValueString();
    bool seekXWapApplicationId(int startIndex, int endIndex);
    bool decodeXWapApplicationId(int startIndex);
    bool decodeUintvarInteger(int startIndex);
    bool decodeValueLength(int startIndex);
    bool decodeExtensionMedia(int startIndex);
    bool decodeHeaderFieldValues(int startIndex);
    static void initWapParaMap();

  public:
    static const string CONTENT_TYPE_B_PUSH_CO;
    static const int PARAMETER_ID_X_WAP_APPLICATION_ID;
    static const int PDU_TYPE_PUSH;
    static const int PDU_TYPE_CONFIRMED_PUSH;

  private:
    void expandWellKnownMimeType();
    bool readContentParameters(int startIndex, int leftToRead, int accumulator);
    bool decodeNoValue(int startIndex);
    string numToHexString(int number);
    string numToString(int number);

  private:
    BYTE* mWspData;
    int mDataLength;
    int mPduLength;
    long mUnsigned32bit;
    map<string, string> mContentParameters;
    map<string, string> mHeaders;
    string mStringValue;
    static const int WAP_PDU_LENGTH_QUOTE;
    static map<int, string> WELL_KNOWN_MIME_TYPES;
    static map<int, string> WELL_KNOWN_PARAMETERS;
    static map<int, string> WELL_KNOWN_HEADERS;
    static const int WAP_PDU_SHORT_LENGTH_MAX;
    static const int Q_VALUE;
};
#endif /* __PDU_PARSER_H__ */
