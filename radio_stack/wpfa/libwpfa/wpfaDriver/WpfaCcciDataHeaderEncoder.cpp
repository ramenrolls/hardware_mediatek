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

#include "WpfaCcciDataHeaderEncoder.h"

#define WPFA_D_LOG_TAG "WpfaCcciDataHeaderEncoder"

String8 WpfaCcciDataHeader::toString() const {
    return String8::format("mMsgId: %d, mTid: %d, mMsgType: %d, mParam16bit: %d", mMsgId, mTid,
                           mMsgType, mParam16bit);
}

WpfaCcciDataHeaderEncoder* WpfaCcciDataHeaderEncoder::sSelf = NULL;

WpfaCcciDataHeaderEncoder::WpfaCcciDataHeaderEncoder() {}

void WpfaCcciDataHeaderEncoder::init() { sSelf = new WpfaCcciDataHeaderEncoder(); }

void WpfaCcciDataHeaderEncoder::encodeHeader(WpfaCcciDataHeader data, ccci_msg_hdr_t* header) {
    header->msg_id = data.getMsgId();
    header->t_id = data.getTid();
    header->msg_type = data.getMsgType();
    header->param_16bit = data.getParams();
    mtkLogD(WPFA_D_LOG_TAG, " encodeHeader msg_id: %d, t_id:%d, msg_type:%d, mPara: %d",
            header->msg_id, header->t_id, header->msg_type, header->param_16bit);
}

WpfaCcciDataHeader WpfaCcciDataHeaderEncoder::decodeHeader(ccci_msg_hdr_t* header) {
    uint16_t msgId = header->msg_id;
    uint16_t mTid = header->t_id;
    uint16_t mMsgType = header->msg_type;
    uint16_t mPara = header->param_16bit;

    mtkLogD(WPFA_D_LOG_TAG, " decodeHeader msg_id: %d, t_id:%d, msg_type:%d, mPara: %d", msgId,
            mTid, mMsgType, mPara);
    WpfaCcciDataHeader data(msgId, mTid, mMsgType, mPara);
    return data;
}
