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

#include "RfxDataCloneManager.h"
#include "RfxIdToStringUtils.h"

#define RFX_LOG_TAG "RfxCloneMgr"

RfxDataCloneManager* RfxDataCloneManager::s_self = NULL;

void RfxDataCloneManager::init() {
    if (s_self == NULL) {
        RFX_LOG_D(RFX_LOG_TAG, "init");
        s_self = new RfxDataCloneManager();
    }
}

void RfxDataCloneManager::registerRequestId(RfxCopyDataByDataFuncptr copyByData,
                                            RfxCopyDataByObjFuncptr copyByObj, int id) {
    init();
    RFX_LOG_D(RFX_LOG_TAG, "registerRequestId: id: %s(%d)", RFX_ID_TO_STR(id), id);
    s_self->registerInternal(s_self->m_request_list, copyByData, copyByObj, id);
}

void RfxDataCloneManager::registerResponseId(RfxCopyDataByDataFuncptr copyByData,
                                             RfxCopyDataByObjFuncptr copyByObj, int id) {
    init();
    RFX_LOG_D(RFX_LOG_TAG, "registerResponseId: id: %s(%d)", RFX_ID_TO_STR(id), id);
    s_self->registerInternal(s_self->m_response_list, copyByData, copyByObj, id);
}

void RfxDataCloneManager::registerUrcId(RfxCopyDataByDataFuncptr copyByData,
                                        RfxCopyDataByObjFuncptr copyByObj, int id) {
    init();
    RFX_LOG_D(RFX_LOG_TAG, "registerUrcId: id: %s(%d)", RFX_ID_TO_STR(id), id);
    s_self->registerInternal(s_self->m_urc_list, copyByData, copyByObj, id);
}

void RfxDataCloneManager::registerEventId(RfxCopyDataByDataFuncptr copyByData,
                                          RfxCopyDataByObjFuncptr copyByObj, int id) {
    init();
    RFX_LOG_D(RFX_LOG_TAG, "registerEventId: id: %s(%d)", RFX_ID_TO_STR(id), id);
    s_self->registerInternal(s_self->m_event_list, copyByData, copyByObj, id);
}

RfxBaseData* RfxDataCloneManager::copyData(int id, void* data, int length, int type) {
    SortedVector<RfxDataCloneEntry> entry = s_self->findDataCloneEntryList(type);
    RfxCopyDataByDataFuncptr ptr = s_self->findCopyDataByDataFuncptr(entry, id);
    if (ptr != NULL) {
        RFX_LOG_D(RFX_LOG_TAG, "copyData id = %d, ptr = %p", id, ptr);
        return ptr(data, length);
    }

    if (id != INVALID_ID) {
        RFX_LOG_D(RFX_LOG_TAG, "did not have corresponding RfxBaseData");
    }
    return NULL;
}

RfxBaseData* RfxDataCloneManager::copyData(int id, const RfxBaseData* data, int type) {
    SortedVector<RfxDataCloneEntry> entry = s_self->findDataCloneEntryList(type);
    RfxCopyDataByObjFuncptr ptr = s_self->findCopyDataByObjFuncptr(entry, id);
    if (ptr != NULL) {
        RFX_LOG_D(RFX_LOG_TAG, "copyData id = %d, ptr = %p", id, ptr);
        return ptr(data);
    }
    if (id != INVALID_ID) {
        RFX_LOG_D(RFX_LOG_TAG, "did not have corresponding RfxBaseData");
    }
    return NULL;
}

void RfxDataCloneManager::registerInternal(SortedVector<RfxDataCloneEntry>& list,
                                           RfxCopyDataByDataFuncptr copyByData,
                                           RfxCopyDataByObjFuncptr copyByObj, int id) {
    RfxCopyDataByDataFuncptr dataFuncptr = copyByData;
    RfxCopyDataByObjFuncptr objFuncptr = copyByObj;

    RfxDataCloneEntry entry(dataFuncptr, objFuncptr, id);
    size_t old_size = list.size();
    list.add(entry);
    if (list.size() == old_size) {
        // if same RfxBaseData, don't trigger assert
        const RfxDataCloneEntry& dup_entry = findDataCloneEntry(list, copyByData, copyByObj, id);
        if (dup_entry != entry) {
            RFX_LOG_E(RFX_LOG_TAG, "duplicated register the same request: %d", id);
            RFX_ASSERT(0);
        }
    }
    RFX_LOG_D(RFX_LOG_TAG, "id = %d, copyByData = %p, copyByObj = %p", id, copyByData, copyByObj);
}

const RfxDataCloneEntry& RfxDataCloneManager::findDataCloneEntry(
        SortedVector<RfxDataCloneEntry>& list, RfxCopyDataByDataFuncptr copyByData,
        RfxCopyDataByObjFuncptr copyByObj, int id) {
    RfxDataCloneEntry query_entry(copyByData, copyByObj, id);
    ssize_t index = list.indexOf(query_entry);
    if (index >= 0) {
        // for debug
        const RfxDataCloneEntry& result = list.itemAt(index);
        // RFX_LOG_D(RFX_LOG_TAG, "findDataCloneEntry, find entry for index = %zd, id = %d,\
// dataPtr = %p, objPtr = %p", index, result.m_id, result.m_copyByData, result.m_copyByObj);
        return list.itemAt(index);
    }

    if (id != INVALID_ID) {
        RFX_LOG_D(RFX_LOG_TAG, "findDataCloneEntry, doesn't find any entry for id = %d", id);
    }
    static RfxDataCloneEntry sDummyEntry = RfxDataCloneEntry();
    return sDummyEntry;
}

RfxCopyDataByDataFuncptr RfxDataCloneManager::findCopyDataByDataFuncptr(
        SortedVector<RfxDataCloneEntry>& entry, int id) {
    const RfxDataCloneEntry& result = findDataCloneEntry(entry, NULL, NULL, id);
    if (/*result != NULL &&*/ result != RfxDataCloneEntry()) {
        // RFX_LOG_D(RFX_LOG_TAG, "findCopyDataByObjFuncptr, result dataPtr = %p, objPtr = %p, id =
        // %d",
        //         result.m_copyByData, result.m_copyByObj, result.m_id);
        return result.m_copyByData;
    }
    RFX_LOG_D(RFX_LOG_TAG, "findCopyDataByObjFuncptr, result is NULL");
    return NULL;
}

RfxCopyDataByObjFuncptr RfxDataCloneManager::findCopyDataByObjFuncptr(
        SortedVector<RfxDataCloneEntry>& entry, int id) {
    const RfxDataCloneEntry& result = findDataCloneEntry(entry, NULL, NULL, id);
    if (/*result != NULL &&*/ result != RfxDataCloneEntry()) {
        // RFX_LOG_D(RFX_LOG_TAG, "findCopyDataByObjFuncptr, result dataPtr = %p, objPtr = %p, id =
        // %d",
        //         result.m_copyByData, result.m_copyByObj, result.m_id);
        return result.m_copyByObj;
    }
    RFX_LOG_D(RFX_LOG_TAG, "findCopyDataByObjFuncptr, result is NULL");
    return NULL;
}

SortedVector<RfxDataCloneEntry> RfxDataCloneManager::findDataCloneEntryList(int type) {
    switch (type) {
        case REQUEST:
            return m_request_list;
        case RESPONSE:
            return m_response_list;
        case URC:
            return m_urc_list;
        case EVENT:
            return m_event_list;
        default:
            RFX_LOG_E(RFX_LOG_TAG, "findDataCloneEntry: should not be here");
            RFX_ASSERT(0);
            break;
    }
}
