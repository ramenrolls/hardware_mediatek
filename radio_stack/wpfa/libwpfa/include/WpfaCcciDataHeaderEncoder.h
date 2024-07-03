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

#ifndef WPFA_DRIVER_CCCI_DATA_ENCODER_H
#define WPFA_DRIVER_CCCI_DATA_ENCODER_H

#include "String8.h"
#include "RefBase.h"
#include "WpfaDriverMessage.h"

#include <stdint.h>
#include <mtk_log.h>

using ::android::String8;

class WpfaCcciDataHeader {
  public:
    WpfaCcciDataHeader() : mMsgId(0), mTid(0), mMsgType(0), mParam16bit(0) {}

    WpfaCcciDataHeader(uint16_t msgId, uint16_t tId, uint16_t msgType, uint16_t para)
        : mMsgId(msgId), mTid(tId), mMsgType(msgType), mParam16bit(para) {}

    uint16_t getMsgId() const { return mMsgId; }

    uint16_t getTid() const { return mTid; }

    uint16_t getMsgType() const { return mMsgType; }

    uint16_t getParams() const { return mParam16bit; }

    String8 toString() const;

  private:
    uint16_t mMsgId;
    uint16_t mTid;
    uint16_t mMsgType;
    uint16_t mParam16bit;
};

class WpfaCcciDataHeaderEncoder {
  public:
    static const size_t HEADER_SIZE = 8;

  private:
    WpfaCcciDataHeaderEncoder();

  public:
    static void init();
    static WpfaCcciDataHeader decodeHeader(ccci_msg_hdr_t* header);
    static void encodeHeader(WpfaCcciDataHeader data, ccci_msg_hdr_t* header);

  private:
    static WpfaCcciDataHeaderEncoder* sSelf;
};
#endif
