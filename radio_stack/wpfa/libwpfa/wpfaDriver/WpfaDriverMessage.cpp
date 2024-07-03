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

#include "WpfaDriverMessage.h"

using ::android::RefBase;
using ::android::sp;
using ::android::String8;

#define WPFA_D_LOG_TAG "WpfaDriverMessage"

WpfaDriverMessage::WpfaDriverMessage() : mMsgId(0), mTid(0), mType(0), mParams(0), mData(NULL) {}

WpfaDriverMessage::~WpfaDriverMessage() {
    if (mData != NULL) {
        delete (mData);
    }
}

sp<WpfaDriverMessage> WpfaDriverMessage::obtainMessage(uint16_t msgId, uint16_t tId, uint16_t type,
                                                       uint16_t params,
                                                       const WpfaDriverBaseData& data) {
    sp<WpfaDriverMessage> msg = new WpfaDriverMessage();
    msg->mMsgId = msgId;
    msg->mTid = tId;
    msg->mType = type;
    msg->mParams = params;
    msg->mData = copyData(msgId, &data);
    return msg;
}

sp<WpfaDriverMessage> WpfaDriverMessage::obtainMessage(uint16_t msgId, uint16_t tId, uint16_t type,
                                                       uint16_t params) {
    sp<WpfaDriverMessage> msg = new WpfaDriverMessage();
    msg->mMsgId = msgId;
    msg->mTid = tId;
    msg->mType = type;
    msg->mParams = params;
    msg->mData = NULL;
    return msg;
}

int WpfaDriverMessage::getCcciMsgBodySize(uint16_t msgId) {
    switch (msgId) {
        case MSG_M2A_REG_DL_FILTER:
            return CCCI_BODY_REG_FILTER_SIZE;
            break;

        case MSG_M2A_DEREG_DL_FILTER:
            return CCCI_BODY_DEREG_FILTER_SIZE;
            break;

        case MSG_A2M_REG_REPLY:
            return CCCI_BODY_REG_ACCEPT_SIZE;
            break;

        case MSG_A2M_DEREG_REPLY:
            return CCCI_BODY_DEREG_ACCEPT_SIZE;
            break;

        case MSG_A2M_WPFA_VERSION:
        case MSG_M2A_WPFA_VERSION:
            return CCCI_BODY_FILTER_VER_SIZE;
            break;

        default:
            return 0;
    }
}

WpfaDriverBaseData* WpfaDriverMessage::copyData(uint16_t msgId, const WpfaDriverBaseData* data) {
    switch (msgId) {
        case MSG_M2A_REG_DL_FILTER:
            return WpfaDriverRegFilterData::copyDataByObj(data);
            break;

        case MSG_M2A_DEREG_DL_FILTER:
            return WpfaDriverDeRegFilterData::copyDataByObj(data);
            break;

        case MSG_A2M_REG_REPLY:
        case MSG_A2M_DEREG_REPLY:
            return WpfaDriverAcceptData::copyDataByObj(data);
            break;

        case MSG_A2M_WPFA_VERSION:
        case MSG_M2A_WPFA_VERSION:
            return WpfaDriverVersionData::copyDataByObj(data);
            break;

        case MSG_M2A_UL_IP_PKT:
            return WpfaDriverULIpPkt::copyDataByObj(data);
            break;

        default:
            return NULL;
    }
}

String8 WpfaDriverMessage::toString() const {
    return String8::format("MsgId=%d, Tid=%d, Type=%d, Params=%d", mMsgId, mTid, mType, mParams);
}
