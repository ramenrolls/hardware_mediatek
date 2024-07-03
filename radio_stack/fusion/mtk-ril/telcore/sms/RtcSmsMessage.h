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

#ifndef __RTC_SMS_MESSAGE_H__
#define __RTC_SMS_MESSAGE_H__
/*****************************************************************************
 * Include
 *****************************************************************************/
#include "RtcSmsHeader.h"
#include "sms/RmcCdmaSmsConverter.h"

/*****************************************************************************
 * Class Declaration
 *****************************************************************************/
class RtcSmsAddress;
class RtcGsmSmsAddress;

/*****************************************************************************
 * External Functions
 *****************************************************************************/
extern unsigned char* convertHexStringToBytes(unsigned char* hexString, size_t dataLen);

/*****************************************************************************
 * RtcSmsMessage
 *****************************************************************************/
class RtcSmsMessage {
    // Constructor / Destructor
  public:
    // Constructor
    RtcSmsMessage();

    // Destructor
    virtual ~RtcSmsMessage();

    // External Method
  public:
    // Check if the SMS is concatenated SMS.
    //
    // RETURNS: true if the SMS is concatenated SMS
    bool isConcatSms() {
        return (mUserDataHeader != NULL) && (!mUserDataHeader->isError()) &&
               (mUserDataHeader->isConcatSms());
    }

    // Get user data header pointer.
    //
    // RETURNS: User data header pointer
    RtcSmsUserDataHeader* getUserDataHeader() { return mUserDataHeader; }

    // Get the pointer of RtcSmsAddress.
    //
    // RETURNS: pointer of RtcSmsAddress
    RtcSmsAddress* getSmsAddress() { return mSmsAddress; }

    // Check if error happen or not when encoding or decoding.
    //
    // RETURNS: true for error
    bool isError() { return mError; }

    // Get the message encoding type.
    //
    // RETURNS: the encoding type
    int getEncodingType() const { return mEncodingType; }

    // Get the number of field.
    //
    // RETURNS: field number
    int getNumField() const { return mNumField; }

    // Get the user data payload.
    //
    // RETURNS: user data payload
    const Vector<unsigned char>& getUserDataPayload() const { return mUserDataPayload; }

    // Constants
  public:
    static const int ENCODING_UNKNOWN = 0;
    static const int ENCODING_7BIT = 1;
    static const int ENCODING_8BIT = 2;
    static const int ENCODING_16BIT = 3;

    // Method for sub class
  protected:
    void setError(bool error = true) { mError = error; }

    void setEncodingType(int encodingType) { mEncodingType = encodingType; }

    void setNumberField(int numField) { mNumField = numField; }

    void createSmsUserDataHeader(unsigned char* header, int len, bool is3Gpp2);

    void setSmsAddress(RtcSmsAddress* addr) { mSmsAddress = addr; }

    Vector<unsigned char>& getUDP() { return mUserDataPayload; }

    void setUDP(Vector<unsigned char>& udp) { mUserDataPayload = udp; }

    // Implement
  private:
    RtcSmsAddress* mSmsAddress;
    RtcSmsUserDataHeader* mUserDataHeader;
    int mEncodingType;
    int mNumField;
    bool mError;
    Vector<unsigned char> mUserDataPayload;
};

/*****************************************************************************
 * GsmSmsMessage
 *****************************************************************************/
class RtcGsmSmsMessage : public RtcSmsMessage {
    // Constructor/Destructor
  public:
    // Construct a message with a hex PDU array.
    RtcGsmSmsMessage(unsigned char* hexString,  // [IN] The Hex Pdu
                     size_t dataLen             // [IN] The data length
    );

    // Construct a message with a base RtcGsmSmsMessage
    // and another Cdma SMS' user data payload.
    RtcGsmSmsMessage(const RtcGsmSmsMessage& message,  // [IN] the base RtcGsmSmsMessage
                     int encodingType,  // [IN] the encoding type of the CDMA SMS message
                     int numFields,     // [IN] the field number of the CDMA SMS message
                     const Vector<unsigned char>&
                             userDataPayload  // [IN] user data payload of CDMA SMS message
    );

    // Destructor
    virtual ~RtcGsmSmsMessage();

    // External Method
  public:
    int getLengthExceptUserData() const { return mLenExceptUserData; }

    const String8& getHexPdu() const { return mHexPdu; }

    // Implement
  private:
    void parse();
    void parseSmsDeliver(int firstByte);
    int parseDataCodingScheme(int dataCodingScheme);
    int getByte();

    // Implement
  private:
    unsigned char* mPdu;
    String8 mHexPdu;
    int mCur;
    int mLenExceptUserData;
    int mDataLen;
};

/*****************************************************************************
 * RtcCdmaSmsMessage
 *****************************************************************************/
class RtcCdmaSmsMessage : public RtcSmsMessage {
    // Constructor / Destructor
  public:
    // Construct with RIL message.
    RtcCdmaSmsMessage(RIL_CDMA_SMS_Message* pMessage  // [IN] the RIL message
    );

    // Construct with one Cdma message and another GSM message's user data payload.
    RtcCdmaSmsMessage(
            const RtcCdmaSmsMessage& message,             // [IN] the base CDMA message
            int encodingType,                             // [IN] GSM message's encoding type
            int numFields,                                // [IN] GSM message's field number
            const Vector<unsigned char>& userDataPayload  // [IN] GSM message's user data payload
    );

    // Destructor
    virtual ~RtcCdmaSmsMessage() {}

    // External Method
  public:
    // Get the reference of RIL message.
    //
    // RETURNS: the reference of message
    const RIL_CDMA_SMS_Message& getMessage() const { return mMsg; }

    // Constants
  private:
    static const int TELESERVICE_WMT = 0x1002;
    static const int TELESERVICE_WEMT = 0x1005;

    // Implement
  private:
    RIL_CDMA_SMS_Message mMsg;
};
#endif /* __RTC_SMS_MESSAGE_H__ */
