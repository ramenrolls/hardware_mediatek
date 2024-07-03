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

#ifndef __RTC_CONCAT_SMS_H__
#define __RTC_CONCAT_SMS_H__
/*****************************************************************************
 * Include
 *****************************************************************************/
#include "RfxObject.h"
#include "RtcSmsAddress.h"
#include "RtcSmsHeader.h"
#include "Timers.h"

/*****************************************************************************
 * Class Declaration
 *****************************************************************************/
class RtcConCatSmsSender;
class RtcConCatSmsGroup;
class RtcConCatSmsPart;
class RtcSmsMessage;

/*****************************************************************************
 * RtcConCatSmsRoot
 *****************************************************************************/
class RtcConCatSmsRoot : public RfxObject {
    RFX_DECLARE_CLASS(RtcConCatSmsRoot);
    // External Method
  public:
    // Get the SMS sender object.
    //
    // RETURNS: SMS sender object
    RtcConCatSmsSender* getSmsSender(const String8& address  // [IN] the SMS address string
    );

    // Close the object that should be closed.
    //
    // RETURNS: None
    void cleanUpObj();
};

/*****************************************************************************
 * RtcConCatSmsSender
 *****************************************************************************/
class RtcConCatSmsSender : public RfxObject {
    RFX_DECLARE_CLASS(RtcConCatSmsSender);
    // Constructor / Destructor
  public:
    // Default Constructor
    RtcConCatSmsSender() {}

    // Construct with a SMS address
    RtcConCatSmsSender(const String8& address  // [IN] the SMS address
                       )
        : mAddress(address) {}

    // Destructor
    virtual ~RtcConCatSmsSender() {}

    // External Method
  public:
    // Get the SMS address.
    //
    // RETURNS: SMS address string.
    const String8& getAddress() const { return mAddress; }

    // Check if the input address is the same address
    //
    // RETURNS: true if address same
    bool isSameAs(const String8& address  // [IN] the address to compare with
    ) {
        return mAddress == address;
    }

    // Get the SMS group object.
    //
    // RETURNS: the SMS group object
    RtcConCatSmsGroup* getSmsGroup(
            int refNumber,    // [IN] the reference number of this concatenated SMS.
            int messageCount  // [IN] the message count
    );

    // Close the object that should be closed.
    //
    // RETURNS: None
    void cleanUpObj();

    // Implement
  private:
    String8 mAddress;
};

/*****************************************************************************
 * RtcConCatSmsGroup
 *****************************************************************************/
class RtcConCatSmsGroup : public RfxObject {
    RFX_DECLARE_CLASS(RtcConCatSmsGroup);
    // Constructor / Destructor
  public:
    // Default constructor
    RtcConCatSmsGroup() : RfxObject(), mTimeStamp(0), mRefNumber(-1), mMessageCount(-1) {}

    // Construct SMS group with reference number and message count.
    RtcConCatSmsGroup(int refNumber,    // [IN] the reference number of this concatenated SMS.
                      int messageCount  // [IN] the message count
                      )
        : RfxObject(), mRefNumber(refNumber), mMessageCount(messageCount) {
        mTimeStamp = systemTime(SYSTEM_TIME_MONOTONIC);
    }

    // Destructor
    virtual ~RtcConCatSmsGroup() {}

    // External Method
  public:
    // Check if same concatenated SMS group
    //
    // RETURNS: true if belong to same concatenated SMS group
    bool isSameAs(int refNumber,    // [IN] the reference number of this concatenated SMS.
                  int messageCount  // [IN] the message count
    ) {
        return (refNumber == mRefNumber) && (mMessageCount == messageCount);
    }

    // Get the SMS part object
    //
    // RETURNS: SMS part object
    RtcConCatSmsPart* getSmsPart(int seqNumber  // [IN] the sequence number of this concatenated SMS
    );

    // Check if all parts are received of this concatenated SMS
    //
    // RETURNS: true if all parts received
    bool allPartsReady() { return mMessageCount == getChildCount(); }

    // Get the reference number of this concatenated SMS.
    //
    // RETURNS: reference number
    int getRefNumber() const { return mRefNumber; }

    // Get the message count of this concatenated SMS.
    //
    // RETURNS: message count
    int getMsgCount() const { return mMessageCount; }

    // Update the time stamp of this SMS group.
    //
    // RETURNS: None
    void updateTimeStamp() { mTimeStamp = systemTime(SYSTEM_TIME_MONOTONIC); }

    // Check if this SMS group is expired
    //
    // RETURNS: true if expired
    bool isExpire();

    // Implement
  private:
    nsecs_t mTimeStamp;
    int mRefNumber;
    int mMessageCount;
};

/*****************************************************************************
 * RtcConCatSmsPart
 *****************************************************************************/
class RtcConCatSmsPart : public RfxObject {
    RFX_DECLARE_CLASS(RtcConCatSmsPart);
    // Constructor / Destructor
  public:
    // Default constructor
    RtcConCatSmsPart() : mSeqNumber(-1), mIs3Gpp(true), mMessage(NULL), mConvertedMessage(NULL) {}

    // Construct a SMM part with sequency number
    RtcConCatSmsPart(int seqNumber  // [IN] the sequency number of this concatenated SMS
                     )
        : mSeqNumber(seqNumber), mIs3Gpp(true), mMessage(NULL), mConvertedMessage(NULL) {}

    // Destructor
    virtual ~RtcConCatSmsPart();

    // External Method
  public:
    // Check if same SMS part.
    //
    // RETURNS: None
    bool isSameAs(int seqNumber  // [IN] the sequency number of this concatenated SMS
    ) {
        return seqNumber == mSeqNumber;
    }

    // Set the SMS format
    //
    // RETURNS: None
    void setFormat3Gpp(bool is3Gpp  // [IN] true if 3GPP format
    ) {
        mIs3Gpp = is3Gpp;
    }

    // Check SMS format.
    //
    // RETURNS: true if 3GPP format
    bool isFormat3Gpp() const { return mIs3Gpp; }

    // Check if two SMS parts have same SMS format.
    //
    // RETURNS: true if having same format
    bool isSameFormatAs(RtcConCatSmsPart* other  // [IN] another SMS part to compare with
    ) {
        return mIs3Gpp == other->isFormat3Gpp();
    }

    // Get the sequency number of this concatenated SMS.
    //
    // RETURNS: sequency number
    int getSeqNumber() const { return mSeqNumber; }

    // Set the message object of this SMS part
    //
    // RETURNS: None
    void setMessage(RtcSmsMessage* message  // [IN] the message object
    ) {
        mMessage = message;
    }

    // Get the reference of message object.
    //
    // RETURNS: message object
    const RtcSmsMessage& getMessage() const;

    // Get the format converted message.
    //
    // RETURNS; the converted message object
    RtcSmsMessage* getConvertedMessage();

    // Implement
  private:
    int mSeqNumber;
    bool mIs3Gpp;
    RtcSmsMessage* mMessage;
    RtcSmsMessage* mConvertedMessage;
};

#endif /* __RTC_CONCAT_SMS_H__ */
