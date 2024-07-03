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
#include "SuplMsgDispatcher.h"
#include <list>
#include "SmsMessage.h"
#include "ConcentratedSms.h"
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>
#include <unistd.h>
#include <cutils/jstring.h>
#include "RfxLog.h"

#define RFX_LOG_TAG "SuplMsgDispatcher"

RFX_IMPLEMENT_CLASS("SuplMsgDispatcher", SuplMsgDispatcher, RfxObject);

int SuplMsgDispatcher::sSuplExistenceState = SUPL_EXISTENCE_UNKNOWN;
int SuplMsgDispatcher::sSuplExistenceDetectionCount = 0;
/*****************************************************************************
 * Class SuplMsgDispatcher
 *****************************************************************************/
void SuplMsgDispatcher::onDeinit() {
    list<ConcentratedSms*>::iterator iter;
    for (iter = mConcSmsList.begin(); iter != mConcSmsList.end(); iter++) {
        ConcentratedSms* concSms = *iter;
        RFX_OBJ_CLOSE(concSms);
    }
    mConcSmsList.clear();
}

void SuplMsgDispatcher::dispatchSuplMsg(string content) {
    int length = content.length();
    RFX_LOG_D(RFX_LOG_TAG, "dispatchSuplMsg is content length: %d", length);
    BYTE* pdu = PhoneNumberUtils::hexStringToBytes(content);
    if (pdu == NULL) {
        return;
    }
    SmsMessage* msg = SmsMessage::createFromPdu(pdu, length / 2);
    RFX_LOG_D(RFX_LOG_TAG, "dispatchSuplMsg begin!");
    if (msg->isConcentratedSms()) {
        RFX_LOG_D(RFX_LOG_TAG, "dispatchSuplMsg is concentrated message!");
        if (msg->isWapush()) {
            // save
            RFX_LOG_D(RFX_LOG_TAG, "dispatchSuplMsg is wap push message!");
            ConcentratedSms* concSms = NULL;
            concSms = findConcSms(msg->getRefNumber());
            if (concSms != NULL) {
                RFX_LOG_D(RFX_LOG_TAG, "dispatchSuplMsg find existing wap push message!");
                concSms->addSegment(msg);
            } else {
                RFX_OBJ_CREATE_EX(concSms, ConcentratedSms, this, (msg));
                mConcSmsList.push_back(concSms);
                concSms->mTimeOutSignal.connect(this, &SuplMsgDispatcher::onConcSmsTimeout);
            }
            bool handled = notifyConcMsg2Mnl(concSms);
            if (handled) {
                mConcSmsList.remove(concSms);
                RFX_OBJ_CLOSE(concSms);
            }
        } else {
            RFX_LOG_D(RFX_LOG_TAG, "dispatchSuplMsg notify sms to mnl!");
            notifySms2Mnl(msg);
            delete msg;
        }
    } else {
        RFX_LOG_D(RFX_LOG_TAG, "dispatchSuplMsg not concentrated sms!");
        if (!notifySms2Mnl(msg)) {
            RFX_LOG_D(RFX_LOG_TAG, "dispatchSuplMsg not sms, check wap push!");
            if (msg->isWapush()) {
                RFX_LOG_D(RFX_LOG_TAG, "dispatchSuplMsg is wap push message!");
                WappushMessage* wapMsg =
                        new WappushMessage(msg->getUserData(), msg->getUserDataLength());
                wapMsg->parsePdu();
                notifyWappush2Mnl(wapMsg);
                delete wapMsg;
            }
        }
        // To do:
        // free memory
        delete msg;
    }
    return;
}

void SuplMsgDispatcher::onConcSmsTimeout(int ref) {
    RFX_LOG_D(RFX_LOG_TAG, "onConcSmsTimeout ref: %d", ref);
    ConcentratedSms* concSms = findConcSms(ref);
    if (concSms != NULL) {
        notifyConcMsg2Mnl(concSms);
        mConcSmsList.remove(concSms);
        RFX_OBJ_CLOSE(concSms);
    }
}

ConcentratedSms* SuplMsgDispatcher::findConcSms(int ref) {
    RFX_LOG_D(RFX_LOG_TAG, "findConcSms ref: %d", ref);
    list<ConcentratedSms*>::iterator iter;
    for (iter = mConcSmsList.begin(); iter != mConcSmsList.end(); iter++) {
        ConcentratedSms* concSms = *iter;
        RFX_LOG_D(RFX_LOG_TAG, "findConcSms getRefNumber: %d", concSms->getRefNumber());
        if (concSms->getRefNumber() == ref) {
            return concSms;
        }
    }
    return NULL;
}

bool SuplMsgDispatcher::notifySms2Mnl(SmsMessage* msg) {
    RFX_LOG_D(RFX_LOG_TAG, "notifySms2Mnl is sms for supl: %d", msg->isSmsForSUPL());
    if (msg->isSmsForSUPL()) {
        BYTE* data = msg->getUserData();
        int length = msg->getUserDataLength();
        hal2mnl_ni_message((char*)data, length);
        return true;
    }
    return false;
}

bool SuplMsgDispatcher::notifyWappush2Mnl(WappushMessage* wapMsg) {
    RFX_LOG_D(RFX_LOG_TAG, "notifyWappush2Mnl is wap push for supl: %d", wapMsg->isWapushForSUPL());
    if (wapMsg->isWapushForSUPL()) {
        BYTE* data = wapMsg->getUserData();
        int length = wapMsg->getUserDataLength();
        hal2mnl_ni_message((char*)data, length);
        return true;
    }
    return false;
}

bool SuplMsgDispatcher::notifyConcMsg2Mnl(ConcentratedSms* msg) {
    RFX_LOG_D(RFX_LOG_TAG, "notifyConcMsg2Mnl isAllSegmentsReceived: %d",
              msg->isAllSegmentsReceived());
    if (msg->isAllSegmentsReceived()) {
        if (msg->isWappush()) {
            RFX_LOG_D(RFX_LOG_TAG, "notifyConcMsg2Mnl isWappush: %d", msg->isWappush());
            msg->parseWappushPdu();
            if (msg->isWapushForSUPL()) {
                RFX_LOG_D(RFX_LOG_TAG, "notifyConcMsg2Mnl isWapushForSUPL: %d",
                          msg->isWapushForSUPL());
                // notify AGPS
                BYTE* userData = msg->getWappushMsgUserData();
                int length = msg->getWappushMsgUserDataLength();
                hal2mnl_ni_message((char*)userData, length);
                return true;
            }
        } else {
            // to do:
        }
    }
    return false;
}

void SuplMsgDispatcher::hal2mnl_ni_message(char* msg, int len) {
    // LOGD("hal2mnl_ni_message  len=%d", len);
    char buff[HAL_MNL_BUFF_SIZE] = {0};
    int offset = 0;
    put_int(buff, &offset, HAL_MNL_INTERFACE_VERSION);
    put_int(buff, &offset, HAL2MNL_NI_MESSAGE);
    put_binary(buff, &offset, msg, len);
    int ret = safe_sendto(MTK_HAL2MNL, buff, offset);
    if (SUPL_EXISTENCE_UNKNOWN == sSuplExistenceState) {
        if (-1 == ret) {
            if (++sSuplExistenceDetectionCount > SUPL_EXISTENCE_MAX_DETECTION) {
                sSuplExistenceState = SUPL_EXISTENCE_NO;
                RFX_LOG_D(RFX_LOG_TAG, "ril2mnl_ni_message SuplExistence: no");
            }
        } else {
            sSuplExistenceState = SUPL_EXISTENCE_YES;
            RFX_LOG_D(RFX_LOG_TAG, "ril2mnl_ni_message SuplExistence: yes");
        }
    }
}

void SuplMsgDispatcher::put_byte(char* buff, int* offset, const char input) {
    *((char*)&buff[*offset]) = input;
    *offset += 1;
}

void SuplMsgDispatcher::put_short(char* buff, int* offset, const short input) {
    put_byte(buff, offset, input & 0xff);
    put_byte(buff, offset, (input >> 8) & 0xff);
}

void SuplMsgDispatcher::put_int(char* buff, int* offset, const int input) {
    put_short(buff, offset, input & 0xffff);
    put_short(buff, offset, (input >> 16) & 0xffff);
}

void SuplMsgDispatcher::put_binary(char* buff, int* offset, const char* input, const int len) {
    put_int(buff, offset, len);
    if (len > 0) {
        memcpy(&buff[*offset], input, len);
        *offset += len;
    }
}

int SuplMsgDispatcher::safe_sendto(const char* path, const char* buff, int len) {
    int ret = 0;
    RFX_LOG_D(RFX_LOG_TAG, "safe_sendto path: %s, len: %d", path, len);
    struct sockaddr_un addr;
    int retry = 10;
    int fd = socket(PF_LOCAL, SOCK_DGRAM, 0);
    if (fd < 0) {
        RFX_LOG_E(RFX_LOG_TAG, "safe_sendto faled reason[%s]:%d", strerror(errno), errno);
        return -1;
    }

    memset(&addr, 0, sizeof(addr));
    addr.sun_path[0] = 0;
    memcpy(addr.sun_path + 1, path, strlen(path));
    addr.sun_family = AF_UNIX;

    while ((ret = sendto(fd, buff, len, 0, (const struct sockaddr*)&addr, sizeof(addr))) == -1) {
        RFX_LOG_E(RFX_LOG_TAG, "safe_sendto faled reason[%s]:%d", strerror(errno), errno);
        if (errno == EINTR) continue;
        if (errno == EAGAIN) {
            if (retry-- > 0) {
                usleep(100 * 1000);
                continue;
            }
        }
        break;
    }
    close(fd);

    return ret;
}

bool SuplMsgDispatcher::doesSuplExist(void) { return (SUPL_EXISTENCE_NO != sSuplExistenceState); }
