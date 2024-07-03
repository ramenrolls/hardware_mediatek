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

#ifndef __RTC_SMS_UTIL_H__
#define __RTC_SMS_UTIL_H__
/*****************************************************************************
 * Include
 *****************************************************************************/
#include <utils/Looper.h>
#include <utils/RefBase.h>
#include <utils/Parcel.h>
#include "RfxBasics.h"
#include "RfxMainThread.h"

/*****************************************************************************
 * Class Declaraion
 *****************************************************************************/

/*****************************************************************************
 * Namespace Declaration
 *****************************************************************************/
using ::android::Looper;
using ::android::Message;
using ::android::MessageHandler;
using ::android::Parcel;
using ::android::RefBase;
using ::android::sp;

/*****************************************************************************
 * Defines
 *****************************************************************************/
#define SMS_TAG "sms-ril"

/*****************************************************************************
 * Class RtcSmsParsingMessage
 *****************************************************************************/
class RtcSmsParsingMessage : public virtual RefBase {
  private:
    RtcSmsParsingMessage() : m_id(-1), m_parcel(NULL) {}
    RtcSmsParsingMessage(const RtcSmsParsingMessage& o);
    RtcSmsParsingMessage& operator=(const RtcSmsParsingMessage& o);
    virtual ~RtcSmsParsingMessage();

  public:
    int32_t getId() const { return m_id; }

    Parcel* getParcel() const { return m_parcel; }

    static sp<RtcSmsParsingMessage> obtainMessage(int32_t id, Parcel* parcel);

  private:
    int32_t m_id;
    Parcel* m_parcel;
};

/*****************************************************************************
 * Class RtcSmsHandler
 *****************************************************************************/
/*
 * Base handler to handle SMS message
 */
class RtcSmsHandler : public RfxMainHandler {
  public:
    explicit RtcSmsHandler(const sp<RtcSmsParsingMessage>& msg) : m_msg(msg) {}
    // Destructor
    virtual ~RtcSmsHandler() {}
    // Send SMS message
    void sendMessage(sp<Looper> looper);

  protected:
    // SMS message referance
    sp<RtcSmsParsingMessage> m_msg;

    // dummy message that makes handler happy
    Message m_dummyMsg;
};

/*****************************************************************************
 * Class RtcSmsParsingThreadHandler
 *****************************************************************************/
/*
 * Handler that is used to send message to SMS parsing thread
 */
class RtcSmsParsingThreadHandler : public RtcSmsHandler {
  public:
    // Constructor
    explicit RtcSmsParsingThreadHandler(
            const sp<RtcSmsParsingMessage>& msg)  // [IN] the SMS message
        : RtcSmsHandler(msg) {}

    // Destructor
    virtual ~RtcSmsParsingThreadHandler() {}

    // Override
  public:
    // Override handleMessage, don't the watch dog in RfxMainHandler
    virtual void handleMessage(const Message& message);
    virtual void onHandleMessage(const Message& message) { RFX_UNUSED(message); }
};

#endif /* __RTC_SMS_UTIL_H__ */
