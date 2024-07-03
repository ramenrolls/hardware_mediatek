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

#ifndef __CONCENTRATED_SMS_H__
#define __CONCENTRATED_SMS_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include <list>
#include "SmsParserUtils.h"
#include "RfxTimer.h"

class SmsMessage;
class WappushMessage;
/*****************************************************************************
 * Class SuplMsgDispatcher
 *****************************************************************************/
class ConcentratedSms : public RfxObject {
    // Required: declare this class
    RFX_DECLARE_CLASS(ConcentratedSms);

  public:
    ConcentratedSms();
    explicit ConcentratedSms(SmsMessage* msg);
    void addSegment(SmsMessage* msg);
    int getRefNumber();
    bool isAllSegmentsReceived();
    void parseWappushPdu();
    BYTE* getWappushMsgUserData();
    int getWappushMsgUserDataLength();
    bool isWappush();
    bool isWapushForSUPL();

  private:
    static bool compareSms(SmsMessage* msg1, SmsMessage* msg2);
    void onTimeout();

    // Override
  protected:
    virtual void onInit();
    virtual void onDeinit();

  public:
    RfxSignal1<int> mTimeOutSignal;

  private:
    int mTotalSeg;
    int mRefNumber;
    bool mIsWapPush;
    BYTE* mWapPushUserData;
    WappushMessage* mWappushMsg;
    list<SmsMessage*> mMsgList;
    TimerHandle mTimeoutHandle;
};
#endif /* __CONCENTRATED_SMS_H__ */
