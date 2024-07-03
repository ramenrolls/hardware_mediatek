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

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "PduParser.h"
#include "ConcentratedSms.h"
#include "SmsMessage.h"
#include "RfxLog.h"

#define RFX_LOG_TAG "ConcentratedSms"

/*****************************************************************************
 * Class SmsHeader
 *****************************************************************************/
RFX_IMPLEMENT_CLASS("ConcentratedSms", ConcentratedSms, RfxObject);

ConcentratedSms::ConcentratedSms(SmsMessage* msg)
    : mWapPushUserData(NULL), mWappushMsg(NULL), mTimeoutHandle(NULL) {
    mTotalSeg = msg->getTotalSegnumber();
    mRefNumber = msg->getRefNumber();
    mIsWapPush = msg->isWapush();
    mMsgList.push_back(msg);
}

ConcentratedSms::ConcentratedSms()
    : mTotalSeg(0),
      mRefNumber(-1),
      mIsWapPush(false),
      mWapPushUserData(NULL),
      mWappushMsg(NULL),
      mTimeoutHandle(NULL) {}

void ConcentratedSms::onInit() {
    RFX_LOG_D(RFX_LOG_TAG, "ConcentratedSms init start timer!");
    mTimeoutHandle = RfxTimer::start(RfxCallback0(this, &ConcentratedSms::onTimeout), ms2ns(60000));
}

void ConcentratedSms::onDeinit() {
    if (mWappushMsg != NULL) {
        delete mWappushMsg;
        mWappushMsg = NULL;
    }
    if (mTimeoutHandle != NULL) {
        RfxTimer::stop(mTimeoutHandle);
        mTimeoutHandle = NULL;
    }
    list<SmsMessage*>::iterator iter;
    for (iter = mMsgList.begin(); iter != mMsgList.end(); iter++) {
        SmsMessage* temp = *iter;
        if (temp != NULL) {
            delete temp;
        }
    }
    mMsgList.clear();
}
int ConcentratedSms::getRefNumber() { return mRefNumber; }

void ConcentratedSms::addSegment(SmsMessage* msg) {
    list<SmsMessage*>::iterator iter;
    for (iter = mMsgList.begin(); iter != mMsgList.end(); iter++) {
        SmsMessage* sms = *iter;
        if ((sms->getRefNumber() == mRefNumber) && (sms->getSeqNumber() == msg->getSeqNumber())) {
            // duplicated SMS, delete it.
            delete msg;
            return;
        }
    }
    mMsgList.push_back(msg);
    mMsgList.sort(ConcentratedSms::compareSms);
    return;
}

bool ConcentratedSms::isAllSegmentsReceived() { return (int)mMsgList.size() == mTotalSeg; }

void ConcentratedSms::parseWappushPdu() {
    int pduLength = 0;
    for (list<SmsMessage*>::iterator iter = mMsgList.begin(); iter != mMsgList.end(); iter++) {
        SmsMessage* sms = *iter;
        pduLength += sms->getUserDataLength();
    }
    BYTE* wapPdu = new BYTE[pduLength];
    int offset = 0;
    for (list<SmsMessage*>::iterator iter = mMsgList.begin(); iter != mMsgList.end(); iter++) {
        SmsMessage* sms = *iter;
        memcpy(wapPdu + offset, sms->getUserData(), sms->getUserDataLength());
        offset += sms->getUserDataLength();
    }
    mWappushMsg = new WappushMessage(wapPdu, pduLength);
    mWappushMsg->parsePdu();
    delete[] wapPdu;
}

BYTE* ConcentratedSms::getWappushMsgUserData() {
    if (mWappushMsg != NULL) {
        return mWappushMsg->getUserData();
    }
    return NULL;
}

int ConcentratedSms::getWappushMsgUserDataLength() {
    if (mWappushMsg != NULL) {
        return mWappushMsg->getUserDataLength();
    }
    return 0;
}

bool ConcentratedSms::isWappush() { return mIsWapPush; }

bool ConcentratedSms::isWapushForSUPL() {
    if (mWappushMsg != NULL) {
        return mWappushMsg->isWapushForSUPL();
    }
    return false;
}

bool ConcentratedSms::compareSms(SmsMessage* msg1, SmsMessage* msg2) {
    if (msg1->getSeqNumber() < msg2->getSeqNumber()) {
        return true;
    }
    return false;
}

void ConcentratedSms::onTimeout() {
    if (mTimeoutHandle != NULL) {
        RfxTimer::stop(mTimeoutHandle);
        mTimeoutHandle = NULL;
    }
    mTimeOutSignal.postEmit(mRefNumber);
}
