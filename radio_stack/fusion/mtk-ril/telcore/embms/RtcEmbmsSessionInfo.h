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

#ifndef __RTC_EMBMS_SESSION_INFO_H__
#define __RTC_EMBMS_SESSION_INFO_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include <utils/String16.h>

#include "RfxStatusDefs.h"
#include "RfxController.h"
#include <telephony/mtk_ril.h>

using ::android::String16;

/*****************************************************************************
 * Class RtcEmbmsSessionInfo
 *****************************************************************************/

class RtcEmbmsSessionInfo {
  public:
    RtcEmbmsSessionInfo();
    virtual ~RtcEmbmsSessionInfo();
    void dump();
    void copy(RtcEmbmsSessionInfo& src);

  public:
    int mTransId; /* Transaction ID for eMBMS middleware usage */
    int mTmgiLen; /* The length of TMGI */
    char mTmgi[EMBMS_MAX_LEN_TMGI + 1];
    int mSaiCount;
    int mSais[EMBMS_MAX_NUM_SAI];
    int mFreqCount;
    int mFreqs[EMBMS_MAX_NUM_FREQ];
    sp<RfxMessage> mOriginalRequest; /* The reqeust message */
};

#endif /* __RTC_EMBMS_SESSION_INFO_H__ */
