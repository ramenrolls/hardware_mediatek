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

#include "RfxFragmentEncoder.h"
#include "RfxRilUtils.h"

#define RFX_LOG_TAG "RfxFragEnc"

String8 RfxFragmentData::toString() const {
    return String8::format(
            "mVersion: %d, mClientId: %d, mConfig: %d, \
            mLength: %zu",
            mVersion, mClientId, mConfig, mLength);
}

RfxFragmentEncoder* RfxFragmentEncoder::sSelf = NULL;

RfxFragmentEncoder::RfxFragmentEncoder() {}

void RfxFragmentEncoder::init() { sSelf = new RfxFragmentEncoder(); }

unsigned char* RfxFragmentEncoder::encodeHeader(RfxFragmentData data) {
    unsigned char* header;
    header = (unsigned char*)calloc(HEADER_SIZE + 1, sizeof(char));
    if (header == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "OOM");
        return NULL;
    }
    char log[100] = {0};
    char* tmpLog = NULL;

    int tmpClientId;
    // start flag
    header[0] = FRAME_START_FLAG;
    header[1] = FRAME_START_FLAG;

    // version
    header[2] = VERSION;

    // config
    header[3] = (char)data.getClientId();
    header[4] = (char)data.getConfig();

    // data length
    RFX_LOG_D(RFX_LOG_TAG, " encodeHeader len: %zu", data.getDataLength());
    header[5] = data.getDataLength() & 0x00FF;
    header[6] = (char)((data.getDataLength() & 0xFF00) >> 8);

    // end flag
    header[7] = FRAME_END_FLAG;
    header[8] = '\0';

    strncpy(log, "header:", 7);
    for (size_t i = 0; i < HEADER_SIZE; i++) {
        asprintf(&tmpLog, "%02X", header[i]);
        strncat(log, tmpLog, strlen(tmpLog));
        free(tmpLog);
        tmpLog = NULL;
    }

    RFX_LOG_D(RFX_LOG_TAG, "%s", log);
    return header;
}

RfxFragmentData RfxFragmentEncoder::decodeHeader(unsigned char* header) {
    unsigned char* local_readp = header;

    size_t startCount = 0;
    while (startCount < FRAME_START_FLAG_SIZE) {
        if (*(local_readp) == FRAME_START_FLAG) {
            RFX_LOG_D(RFX_LOG_TAG, "find start flag");
            local_readp++;
            startCount++;
        } else {
            RFX_LOG_D(RFX_LOG_TAG, "ERROR: can not find start flag");
            // RFX_ASSERT(0);
            return RfxFragmentData();
        }
    }

    unsigned char version = *(local_readp++);
    unsigned char clientId = *(local_readp++);
    unsigned char config = *(local_readp++);
    unsigned char length = *(local_readp++);
    unsigned char lengthExt = *(local_readp++);
    size_t dataLength = (size_t)length + ((size_t)lengthExt << 8);
    unsigned char endFlag = *(local_readp++);
    if (endFlag == FRAME_END_FLAG) {
        RFX_LOG_D(RFX_LOG_TAG, "find end flag");
    } else {
        RFX_LOG_E(RFX_LOG_TAG, "ERROR: can not find end flag");
        // RFX_ASSERT(0);
        return RfxFragmentData();
    }

    RFX_LOG_D(RFX_LOG_TAG, " decodeHeader version: %d, clientId:%d, config:%d, length: %zu",
              version, clientId, config, dataLength);
    RfxFragmentData data((int)version, clientId, config, dataLength);
    return data;
}
