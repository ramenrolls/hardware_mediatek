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

#ifndef __RTC_SMS_HEADER_H__
#define __RTC_SMS_HEADER_H__

/*****************************************************************************
 * Class ConcatRef
 *****************************************************************************/
class ConcatRef {
  public:
    ConcatRef() : refNumber(-1), seqNumber(-1), msgCount(-1), is3Gpp2(false), isEightBits(false) {}

  public:
    int refNumber;
    int seqNumber;
    int msgCount;
    bool is3Gpp2;
    bool isEightBits;
};

/*****************************************************************************
 * Class RtcSmsUserDataHeader
 *****************************************************************************/
class RtcSmsUserDataHeader {
    // Constructor/Destructor
  public:
    // Construct user data header with UDH array.
    RtcSmsUserDataHeader(unsigned char* header,  // [IN] point to first elemement of header array
                         int len,                // [IN] the length of header array
                         bool is3Gpp2  // [IN] the format of the SMS, true is 3GPP2, false is 3GPP
    );

    // Destructor
    virtual ~RtcSmsUserDataHeader() {}

    // External Method
  public:
    // Check if the SMS is concatenated SMS.
    //
    // RETURNS: true if the SMS is concatenated SMS
    bool isConcatSms() const { return mConcatSms; }

    // Get the reference number of the concatenated SMS.
    //
    // RETURNS: the reference number
    int getRefNumber() const { return mConcatRef.refNumber; }

    // Get the message count of the concatenated SMS.
    //
    // RETURNS: the message count
    int getMsgCount() const { return mConcatRef.msgCount; }

    // Get the sequency number of the concatenated SMS.
    int getSeqNumber() const { return mConcatRef.seqNumber; }

    // Check the SMS format is 3GPP2 or not.
    //
    // RETURNS: true if 3GPP2 format SMS
    bool is3Gpp2() { return mConcatRef.is3Gpp2; }

    // Error happens or not when parsing the SMS header.
    //
    // RETURNS: true if error happen
    bool isError() { return mError; }

  private:
    // Set Error when parsing SMS header error.
    //
    // RETURNS: NONE
    void setError(bool error = true  // [IN] true for error
    ) {
        mError = error;
    }

    // Implement
  private:
    ConcatRef mConcatRef;
    bool mConcatSms;
    bool mError;
};

#endif /* __RTC_SMS_HEADER_H__ */
