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
#include "RtcEmbmsSessionInfo.h"

#define RFX_LOG_TAG "RTC_EMBMS_INFO"

/*****************************************************************************
 * Class RpDataConnectionInfo
 *****************************************************************************/

RtcEmbmsSessionInfo::RtcEmbmsSessionInfo()
    : mTransId(-1),
      mTmgiLen(0),
      mTmgi{0},
      mSaiCount(0),
      mSais{0},
      mFreqCount(0),
      mFreqs{0},
      mOriginalRequest(NULL) {}

RtcEmbmsSessionInfo::~RtcEmbmsSessionInfo() {}

void RtcEmbmsSessionInfo::copy(RtcEmbmsSessionInfo& src) {
    this->mTransId = src.mTransId;
    this->mTmgiLen = src.mTmgiLen;
    memcpy(this->mTmgi, src.mTmgi, sizeof(this->mTmgi[0]) * (EMBMS_MAX_LEN_TMGI + 1));
    this->mSaiCount = src.mSaiCount;
    memcpy(this->mSais, src.mSais, sizeof(this->mSais[0]) * EMBMS_MAX_NUM_SAI);
    this->mFreqCount = src.mFreqCount;
    memcpy(this->mFreqs, src.mFreqs, sizeof(this->mFreqs[0]) * EMBMS_MAX_NUM_FREQ);
    this->mOriginalRequest = src.mOriginalRequest;
}

void RtcEmbmsSessionInfo::dump() {
    RFX_LOG_D(RFX_LOG_TAG, "TransId:[%d],Tmgi:[%s],mSaiCount:[%d],mFreqCount:[%d]", mTransId, mTmgi,
              mSaiCount, mFreqCount);
}
