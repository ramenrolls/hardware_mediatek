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
#include "RtcConcatSms.h"
#include "RtcSmsMessage.h"

/*****************************************************************************
 * Class RtcConCatSmsRoot
 *****************************************************************************/
RFX_IMPLEMENT_CLASS("RtcConCatSmsRoot", RtcConCatSmsRoot, RfxObject);

RtcConCatSmsSender* RtcConCatSmsRoot::getSmsSender(const String8& address) {
    if (address.isEmpty()) {
        return NULL;
    }
    RfxObject* obj = getFirstChildObj();
    RtcConCatSmsSender* sender = NULL;
    while (obj != NULL) {
        sender = (RtcConCatSmsSender*)obj;
        if (sender->isSameAs(address)) {
            return sender;
        }
        obj = obj->getNextObj();
    }
    RFX_OBJ_CREATE_EX(sender, RtcConCatSmsSender, this, (address));
    return sender;
}

void RtcConCatSmsRoot::cleanUpObj() {
    RfxObject* obj = getFirstChildObj();
    RtcConCatSmsSender* sender = NULL;
    Vector<RtcConCatSmsSender*> cleanObj;
    while (obj != NULL) {
        sender = (RtcConCatSmsSender*)obj;
        sender->cleanUpObj();
        if (sender->getChildCount() == 0) {
            cleanObj.push(sender);
        }
        obj = obj->getNextObj();
    }
    Vector<RtcConCatSmsSender*>::iterator it;
    for (it = cleanObj.begin(); it != cleanObj.end(); it++) {
        RFX_OBJ_CLOSE(*it);
    }
}

/*****************************************************************************
 * RtcConCatSmsSender
 *****************************************************************************/
RFX_IMPLEMENT_CLASS("RtcConCatSmsSender", RtcConCatSmsSender, RfxObject);

RtcConCatSmsGroup* RtcConCatSmsSender::getSmsGroup(int refNumber, int messageCount) {
    if (refNumber < 0 || messageCount < 0) {
        return NULL;
    }
    RfxObject* obj = getFirstChildObj();
    RtcConCatSmsGroup* group = NULL;
    while (obj != NULL) {
        group = (RtcConCatSmsGroup*)obj;
        if (group->isSameAs(refNumber, messageCount)) {
            return group;
        }
        obj = obj->getNextObj();
    }
    RFX_OBJ_CREATE_EX(group, RtcConCatSmsGroup, this, (refNumber, messageCount));
    return group;
}

void RtcConCatSmsSender::cleanUpObj() {
    RfxObject* obj = getFirstChildObj();
    RtcConCatSmsGroup* group = NULL;
    Vector<RtcConCatSmsGroup*> cleanObj;
    while (obj != NULL) {
        group = (RtcConCatSmsGroup*)obj;
        if (group->allPartsReady() || group->isExpire()) {
            cleanObj.push(group);
        }
        obj = obj->getNextObj();
    }
    Vector<RtcConCatSmsGroup*>::iterator it;
    for (it = cleanObj.begin(); it != cleanObj.end(); it++) {
        RFX_OBJ_CLOSE(*it);
    }
}

/*****************************************************************************
 * RtcConCatSmsGroup
 *****************************************************************************/
RFX_IMPLEMENT_CLASS("RtcConCatSmsGroup", RtcConCatSmsGroup, RfxObject);

RtcConCatSmsPart* RtcConCatSmsGroup::getSmsPart(int seqNumber) {
    if (seqNumber <= 0 || seqNumber > mMessageCount) {
        return NULL;
    }
    RfxObject* obj = getFirstChildObj();
    RtcConCatSmsPart* part = NULL;
    while (obj != NULL) {
        part = (RtcConCatSmsPart*)obj;
        if (part->isSameAs(seqNumber)) {
            return part;
        }
        obj = obj->getNextObj();
    }
    RFX_OBJ_CREATE_EX(part, RtcConCatSmsPart, this, (seqNumber));
    return part;
}

bool RtcConCatSmsGroup::isExpire() {
    nsecs_t now = systemTime(SYSTEM_TIME_MONOTONIC);
    if ((now - mTimeStamp) > seconds_to_nanoseconds(60 * 3)) {
        return true;
    }
    return false;
}

/*****************************************************************************
 * RtcConCatSmsPart
 *****************************************************************************/
RFX_IMPLEMENT_CLASS("RtcConCatSmsPart", RtcConCatSmsPart, RfxObject);

RtcConCatSmsPart::~RtcConCatSmsPart() {
    if (mConvertedMessage != NULL) {
        delete mConvertedMessage;
    }
    if (mMessage != NULL) {
        delete mMessage;
    }
}

const RtcSmsMessage& RtcConCatSmsPart::getMessage() const {
    if (mIs3Gpp) {
        return *((RtcGsmSmsMessage*)mMessage);
    } else {
        return *((RtcCdmaSmsMessage*)mMessage);
    }
}

RtcSmsMessage* RtcConCatSmsPart::getConvertedMessage() {
    if (mConvertedMessage != NULL) {
        return mConvertedMessage;
    }
    RtcConCatSmsGroup* group = (RtcConCatSmsGroup*)getParent();
    if (group != NULL) {
        RtcConCatSmsPart* part = (RtcConCatSmsPart*)group->getFirstChildObj();
        if ((part != NULL) && (part != this) && (!part->isSameFormatAs(this))) {
            if (part->isFormat3Gpp()) {
                mConvertedMessage = new RtcGsmSmsMessage(
                        (const RtcGsmSmsMessage&)part->getMessage(), getMessage().getEncodingType(),
                        getMessage().getNumField(), getMessage().getUserDataPayload());
            } else {
                mConvertedMessage = new RtcCdmaSmsMessage(
                        (const RtcCdmaSmsMessage&)part->getMessage(),
                        getMessage().getEncodingType(), getMessage().getNumField(),
                        getMessage().getUserDataPayload());
            }
        }
    }
    return mConvertedMessage;
}
