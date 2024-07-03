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
#include "RfxIdToStringUtils.h"
#include "RfxLog.h"

#define RFX_LOG_TAG "RfxIdToStr"

RfxIdToStringUtils* RfxIdToStringUtils::sSelf = NULL;

void RfxIdToStringUtils::init() {
    if (sSelf == NULL) {
        RFX_LOG_D(RFX_LOG_TAG, "init");
        sSelf = new RfxIdToStringUtils();
    }
}

void RfxIdToStringUtils::registerId(int id, char* str) {
    init();
    sSelf->registerInternal(sSelf->mIdList, id, str);
}

const char* RfxIdToStringUtils::idToString(int id) {
    init();
    Mutex::Autolock autoLock(sSelf->mMutex);
    const RfxIdMappingEntry& entry = sSelf->findIdEntry(sSelf->mIdList, id);
    return entry.mStr.string();
}

void RfxIdToStringUtils::registerInternal(SortedVector<RfxIdMappingEntry>& list, int id,
                                          char* str) {
    RfxIdMappingEntry entry(id, str);
    size_t old_size = list.size();
    list.add(entry);
    if (list.size() == old_size) {
        // if same RfxBaseData, don't trigger assert
        const RfxIdMappingEntry& dup_entry = findIdEntry(list, id);
        if (dup_entry != entry) {
            RFX_LOG_E(RFX_LOG_TAG, "duplicated register the same request: %d", id);
            RFX_ASSERT(0);
        }
    }
    RFX_LOG_D(RFX_LOG_TAG, "id = %d, string = %s", entry.mId, entry.mStr.string());
}

const RfxIdMappingEntry& RfxIdToStringUtils::findIdEntry(SortedVector<RfxIdMappingEntry>& list,
                                                         int id) {
    RfxIdMappingEntry query_entry(id, String8());
    ssize_t index = list.indexOf(query_entry);
    if (index >= 0) {
        return list.itemAt(index);
    }

    // add unknow id to list and query again. SortVector would copy entry.
    // We can avoid to return local object
    RfxIdMappingEntry unknownEntry =
            RfxIdMappingEntry(id, String8::format("UNKNOWN ID: %d", id).string());
    list.add(unknownEntry);
    index = list.indexOf(query_entry);
    return list.itemAt(index);
}
