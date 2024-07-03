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

#ifndef __RFX_ID_TO_STRING_UTILS__
#define __RFX_ID_TO_STRING_UTILS__

#include "utils/String8.h"
#include "utils/SortedVector.h"
#include "utils/Mutex.h"

using ::android::Mutex;
using ::android::SortedVector;
using ::android::String8;
using ::android::Vector;

#define RFX_ID_TO_STR(id) RfxIdToStringUtils::idToString(id)

class RfxIdMappingEntry {
  public:
    RfxIdMappingEntry() : mId(-1), mStr(String8()) {}

    RfxIdMappingEntry(int id, const char* str) : mId(id), mStr(str) {}

    RfxIdMappingEntry(const RfxIdMappingEntry& other) : mId(other.mId), mStr(other.mStr) {}

    bool operator<(const RfxIdMappingEntry& other) const { return (mId < other.mId); }

    bool operator>(const RfxIdMappingEntry& other) const { return (mId > other.mId); }

    bool operator==(const RfxIdMappingEntry& other) const { return (mId == other.mId); }

    bool operator!=(const RfxIdMappingEntry& other) const { return (mId != other.mId); }

  public:
    int mId;
    String8 mStr;
};

class RfxIdToStringUtils {
  public:
    RfxIdToStringUtils() {}
    virtual ~RfxIdToStringUtils() {}

    static void init();
    static void registerId(int id, char* str);
    static const char* idToString(int id);

  private:
    void registerInternal(SortedVector<RfxIdMappingEntry>& list, int id, char* str);
    const RfxIdMappingEntry& findIdEntry(SortedVector<RfxIdMappingEntry>& list, int id);

  private:
    static RfxIdToStringUtils* sSelf;
    SortedVector<RfxIdMappingEntry> mIdList;
    Mutex mMutex;
};

#endif
