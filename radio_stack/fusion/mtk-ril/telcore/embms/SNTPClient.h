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

#ifndef SNTP_CLIENT_H_

#define SNTP_CLIENT_H_

#include <utils/Errors.h>

namespace android {

static uint64_t OFFSET_1900_TO_1970 = ((365LL * 70LL) + 17LL) * 24LL * 60LL * 60LL;

// Implementation of the SNTP (Simple Network Time Protocol)
struct SNTPClient {
    SNTPClient();

    status_t requestTime(const char* host);

    // given a time obtained from ALooper::GetNowUs()
    // return the number of us elapsed since Jan 1 1970 00:00:00 (UTC).
    uint64_t mTimeReferenceNTP;
    int64_t mTimeReferenceUs;
    int64_t mRoundTripTimeNTP;

  private:
    enum {
        kNTPPort = 123,
        kNTPPacketSize = 48,
        kNTPModeClient = 3,
        kNTPVersion = 3,
        kNTPTransmitTimeOffset = 40,
        kNTPOriginateTimeOffset = 24,
        kNTPReceiveTimeOffset = 32,
    };

    static void writeTimeStamp(uint8_t* buffer, int offset, uint64_t time);
    static int64_t readTimeStamp(uint8_t* buffer, int offset);
};

uint64_t read32(uint8_t* buffer, int offset);

}  // namespace android

#endif  // SNTP_CLIENT_H_
