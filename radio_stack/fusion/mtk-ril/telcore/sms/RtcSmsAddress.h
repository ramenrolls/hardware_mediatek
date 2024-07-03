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

#ifndef __RTC_SMS_ADDRESS_H__
#define __RTC_SMS_ADDRESS_H__
/*****************************************************************************
 * Include
 *****************************************************************************/
#include "sms/RmcCdmaSmsConverter.h"

/*****************************************************************************
 * RtcSmsAddress
 *****************************************************************************/
class RtcSmsAddress {
    // Constructor / Destrutor
  public:
    // Constructor
    RtcSmsAddress();

    // External Method
  public:
    // Get the address of the SMS.
    //
    // RETURNS: the SMS address
    const String8& getAddressString() const { return mAddress; }

    // Check if error happens when parsing SMS address
    //
    // RETURNS: true if error happens
    bool isError() { return mError; }

    // Constants
  public:
    static const int TON_UNKNOWN = 0;
    static const int TON_INTERNATIONAL = 1;
    static const int TON_NATIONAL = 2;
    static const int TON_NETWORK = 3;
    static const int TON_SUBSCRIBER = 4;
    static const int TON_ALPHANUMERIC = 5;
    static const int TON_ABBREVIATED = 6;

    // Method for sub class
  protected:
    // Set parsing error.
    //
    // RETURNS: None
    void setError(bool error = true  // [IN] true for error, false for no error
    ) {
        mError = error;
    }

    // Set the type of number(TON).
    //
    // RETRURNS: None
    void setTon(int ton) { mTon = ton; }

    // Get the type of number(TON).
    //
    // RETURNS: the type of number
    const int& getTon() const { return mTon; }

    // Get the address for address modification.
    //
    // RETURNS: the reference of the address
    String8& getAddr() { return mAddress; }

    // Check the number type if alphanumeric.
    //
    // RETURNS: true if alphanumeric number
    bool isAlphanumeric() { return mTon == TON_ALPHANUMERIC; }

    // Implement
  private:
    int mTon;
    String8 mAddress;
    bool mError;
};

/*****************************************************************************
 * RtcGsmSmsAddress
 *****************************************************************************/
class RtcGsmSmsAddress : public RtcSmsAddress {
    // Constructor / Destructor
  public:
    // Constructor
    RtcGsmSmsAddress(unsigned char* data, int offset, int length);

    // Implement
  private:
    bool bcd2Char(unsigned char bcd, String8& addr);
    bool calledPartyBCDFragmentToString(unsigned char* bcd, int offset, int length, String8& addr);

    static const int MAX_BUFFER_SIZE = 40;
    static const int OFFSET_ADDRESS_LENGTH = 0;
    static const int OFFSET_TOA = 1;
    static const int OFFSET_ADDRESS_VALUE = 2;
    unsigned char mOrigBytes[MAX_BUFFER_SIZE];
};

/*****************************************************************************
 * RtcCdmaSmsAddress
 *****************************************************************************/
class RtcCdmaSmsAddress : public RtcSmsAddress {
    // Constructor / Destructor
  public:
    RtcCdmaSmsAddress(RIL_CDMA_SMS_Address& addr);
};
#endif /* __RTC_SMS_ADDRESS_H__ */
