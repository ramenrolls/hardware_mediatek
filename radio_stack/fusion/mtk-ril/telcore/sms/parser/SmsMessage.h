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

#ifndef __SMS_MESSAGE_H__
#define __SMS_MESSAGE_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "SmsParserUtils.h"

class PduParser;
class SmsHeader;
class SmsAddress;
class GsmSmsAddress;
/*****************************************************************************
 * Class SmsMessage
 *****************************************************************************/
class SmsMessage {
  public:
    SmsMessage();
    virtual ~SmsMessage();
    static SmsMessage* createFromPdu(BYTE* pdu, int length);
    bool isReplace();
    bool isWapush();
    bool isSmsForSUPL();
    bool isConcentratedSms();
    BYTE* getUserData();
    int getUserDataLength();
    int getRefNumber();
    int getSeqNumber();
    int getTotalSegnumber();

  private:
    void parsePdu(BYTE* pdu, int length);
    void parseSmsDeliver(PduParser* parser, int firstByte);
    void parseUserData(PduParser* parser, bool hasUserDataHeader);
    void parseWappushUserData();
    void parseMessageBody();
    void extractEmailAddressFromMessageBody();

  private:
    BYTE* mPdu;
    string mScAddress;
    int mMti;
    bool mReplyPathPresent;
    int mProtocolIdentifier;
    GsmSmsAddress* mOriginatingAddress;
    long mScTimeMillis;
    int mDataCodingScheme;
    bool mIsMwi;
    bool mMwiSense;
    bool mMwiDontStore;
    int mVoiceMailCount;
    BYTE* mUserData;
    int mUserDataLength;
    SmsHeader* mUserDataHeader;
    MessageClass mMessageClass;
    string mEmailFrom;
    string mEmailBody;
    string mMessageBody;
    bool mIsEmail;
    int mCur;
    int mEncodingType;
};

class WappushMessage {
  public:
    WappushMessage(BYTE* pdu, int length);
    virtual ~WappushMessage();
    void parsePdu();
    bool isWapushForSUPL();
    BYTE* getUserData();
    int getUserDataLength();

  private:
    BYTE* mPdu;
    int mTransactionId;
    int mPduType;
    int mPduLength;
    string mWapAppId;
    string mContentType;
    BYTE* mUserData;
    int mUserDataLength;
    static const string MIME_TYPE_SUPL;
    static const string XWAP_APP_ID_SUPL;
    static const string MIME_TYPE_SUPL_HEX;
    static const string XWAP_APP_ID_SUPL_HEX;
};
#endif /* __SMS_MESSAGE_H__ */
