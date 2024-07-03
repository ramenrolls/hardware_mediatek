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

#ifndef __RFX_DATA_CLONE_MANAGER__H__
#define __RFX_DATA_CLONE_MANAGER__H__

#include "utils/SortedVector.h"
#include "utils/RefBase.h"
#include "RfxBaseData.h"
#include "RfxLog.h"
#include "RfxDefs.h"

using ::android::SortedVector;
using ::android::sp;
using ::android::Vector;
using ::android::wp;

/*****************************************************************************
 * Class RfxDataCloneEntry
 *****************************************************************************/

class RfxDataCloneEntry {
  public:
    RfxDataCloneEntry() : m_copyByData(NULL), m_copyByObj(NULL), m_id(-1) {}

    RfxDataCloneEntry(RfxCopyDataByDataFuncptr copyByData, RfxCopyDataByObjFuncptr copyByObj,
                      int id)
        : m_copyByData(copyByData), m_copyByObj(copyByObj), m_id(id) {}

    RfxDataCloneEntry(const RfxDataCloneEntry& entry)
        : m_copyByData(entry.m_copyByData), m_copyByObj(entry.m_copyByObj), m_id(entry.m_id) {}

    bool operator<(const RfxDataCloneEntry& entry) const { return (m_id < entry.m_id); }

    bool operator>(const RfxDataCloneEntry& entry) const { return (m_id > entry.m_id); }

    bool operator==(const RfxDataCloneEntry& entry) const { return (m_id == entry.m_id); }
    bool operator!=(const RfxDataCloneEntry& entry) const { return (m_id != entry.m_id); }

  public:
    RfxCopyDataByDataFuncptr m_copyByData;
    RfxCopyDataByObjFuncptr m_copyByObj;
    int m_id;
};

/*****************************************************************************
 * Class RfxDataCloneManager
 *****************************************************************************/

class RfxDataCloneManager {
  public:
    RfxDataCloneManager() {}
    virtual ~RfxDataCloneManager() {}

    static void init();
    // register API
    static void registerRequestId(RfxCopyDataByDataFuncptr copyByData,
                                  RfxCopyDataByObjFuncptr copyByObj, int id);
    static void registerResponseId(RfxCopyDataByDataFuncptr copyByData,
                                   RfxCopyDataByObjFuncptr copyByObj, int id);
    static void registerUrcId(RfxCopyDataByDataFuncptr copyByData,
                              RfxCopyDataByObjFuncptr copyByObj, int id);
    static void registerEventId(RfxCopyDataByDataFuncptr copyByData,
                                RfxCopyDataByObjFuncptr copyByObj, int id);

    // copy API
    static RfxBaseData* copyData(int id, void* data, int length, int type);
    static RfxBaseData* copyData(int id, const RfxBaseData* data, int type);

  private:
    void registerInternal(SortedVector<RfxDataCloneEntry>& list,
                          RfxCopyDataByDataFuncptr copyByData, RfxCopyDataByObjFuncptr copyByObj,
                          int id);
    const RfxDataCloneEntry& findDataCloneEntry(SortedVector<RfxDataCloneEntry>& list,
                                                RfxCopyDataByDataFuncptr copyByData,
                                                RfxCopyDataByObjFuncptr copyByObj, int id);
    RfxCopyDataByDataFuncptr findCopyDataByDataFuncptr(SortedVector<RfxDataCloneEntry>& entry,
                                                       int id);
    RfxCopyDataByObjFuncptr findCopyDataByObjFuncptr(SortedVector<RfxDataCloneEntry>& entry,
                                                     int id);
    SortedVector<RfxDataCloneEntry> findDataCloneEntryList(int type);

  private:
    static RfxDataCloneManager* s_self;
    SortedVector<RfxDataCloneEntry> m_request_list;
    SortedVector<RfxDataCloneEntry> m_response_list;
    SortedVector<RfxDataCloneEntry> m_urc_list;
    SortedVector<RfxDataCloneEntry> m_event_list;
};

#endif
