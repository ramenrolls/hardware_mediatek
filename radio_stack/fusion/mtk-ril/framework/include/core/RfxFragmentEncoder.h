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

#ifndef __RFX_FRAGMENT_ENCODER_H__
#define __RFX_FRAGMENT_ENCODER_H__

#include "RfxLog.h"
#include "rfx_properties.h"
#include "RfxDefs.h"
#include <telephony/mtk_ril.h>
#include "utils/String8.h"

using ::android::String8;

class RfxFragmentData {
  public:
    RfxFragmentData() : mVersion(1), mClientId(-1), mConfig(0), mLength(0) {}

    RfxFragmentData(int version, int clientId, int config, size_t length)
        : mVersion(version), mClientId(clientId), mConfig(config), mLength(length) {}

    int getClientId() const { return mClientId; }

    int getVersion() const { return mVersion; }

    int getConfig() const { return mConfig; }

    size_t getDataLength() { return mLength; }

    String8 toString() const;

  private:
    int mVersion;
    int mClientId;
    int mConfig;
    size_t mLength;
};

class RfxFragmentEncoder {
  public:
    static const unsigned char VERSION = 1;
    static const size_t HEADER_SIZE = 8;
    static const size_t MAX_FRAME_SIZE = 2048;

  private:
    static const unsigned char FRAME_START_FLAG = 0x7F;
    static const unsigned char FRAME_END_FLAG = 0x7F;
    static const size_t FRAME_START_FLAG_SIZE = 2;
    static const size_t FRAME_CLIENT_ID_SIZE = 5;

  private:
    RfxFragmentEncoder();

  public:
    static void init();
    static RfxFragmentData decodeHeader(unsigned char* header);
    static unsigned char* encodeHeader(RfxFragmentData data);

  private:
    static RfxFragmentEncoder* sSelf;
};
#endif
