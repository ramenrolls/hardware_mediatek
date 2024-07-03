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

#ifndef __RFX_HANDLER_MANAGER__H__
#define __RFX_HANDLER_MANAGER__H__

#include "utils/RefBase.h"
#include "utils/SortedVector.h"
#include "utils/String8.h"
#include <map>
#include "RfxBaseHandler.h"
#include "RfxMclMessage.h"
#include "RfxDefs.h"
#include "utils/Mutex.h"

using ::android::Mutex;
using ::android::RefBase;
using ::android::SortedVector;
using ::android::sp;
using ::android::String8;
using ::android::Vector;
using ::android::wp;
using namespace std;

// Predefined class
class RfxBaseHandler;
typedef RfxBaseHandler* (*RfxCreateHandlerFuncptr)(int slot_id, int channe_id);

#define RFX_HANDLER_CREATE(_ptr, _class_name, _param) \
    do {                                              \
        (_ptr) = new _class_name _param;              \
    } while (0)

#define RFX_HANDLER_DESTROY(_ptr)      \
    do {                               \
        if (_ptr != NULL) delete _ptr; \
    } while (0)

class RfxHandlerRegisterEntry {
  public:
    RfxHandlerRegisterEntry()
        : m_handler(NULL),
          m_channel_id(-1),
          m_slot_id(-1),
          m_id(-1),
          m_client_id(-1),
          m_raw_urc(String8()),
          mNeedAllMatch(false) {}

    RfxHandlerRegisterEntry(RfxBaseHandler* handler, int channel_id, int slot_id, int id,
                            const char* raw_urc, bool needAllMatch)
        : m_handler(handler),
          m_channel_id(channel_id),
          m_slot_id(slot_id),
          m_id(id),
          m_client_id(-1),
          m_raw_urc(raw_urc),
          mNeedAllMatch(needAllMatch) {}

    RfxHandlerRegisterEntry(RfxBaseHandler* handler, int channel_id, int slot_id, int id,
                            int client_id, const char* raw_urc, bool needAllMatch)
        : m_handler(handler),
          m_channel_id(channel_id),
          m_slot_id(slot_id),
          m_id(id),
          m_client_id(client_id),
          m_raw_urc(raw_urc),
          mNeedAllMatch(needAllMatch) {}

    RfxHandlerRegisterEntry(const RfxHandlerRegisterEntry& other)
        : m_handler(other.m_handler),
          m_channel_id(other.m_channel_id),
          m_slot_id(other.m_slot_id),
          m_id(other.m_id),
          m_client_id(other.m_client_id),
          m_raw_urc(other.m_raw_urc),
          mNeedAllMatch(other.mNeedAllMatch) {}

    bool operator<(const RfxHandlerRegisterEntry& other) const {
        // condition1
        return (mNeedAllMatch < other.mNeedAllMatch) ||
               // condition2
               (mNeedAllMatch == other.mNeedAllMatch &&
                m_raw_urc.size() > other.m_raw_urc.size()) ||
               // condition3
               (mNeedAllMatch == other.mNeedAllMatch &&
                m_raw_urc.size() == other.m_raw_urc.size() && m_slot_id < other.m_slot_id) ||
               // condition4
               (mNeedAllMatch == other.mNeedAllMatch &&
                m_raw_urc.size() == other.m_raw_urc.size() && m_slot_id == other.m_slot_id &&
                m_id < other.m_id) ||
               // condition5
               (mNeedAllMatch == other.mNeedAllMatch &&
                m_raw_urc.size() == other.m_raw_urc.size() && m_slot_id == other.m_slot_id &&
                m_id == other.m_id && m_raw_urc < other.m_raw_urc) ||
               // condition6
               (mNeedAllMatch == other.mNeedAllMatch &&
                m_raw_urc.size() == other.m_raw_urc.size() && m_slot_id == other.m_slot_id &&
                m_id == other.m_id && m_raw_urc == other.m_raw_urc &&
                m_client_id < other.m_client_id);
    }

    bool operator>(const RfxHandlerRegisterEntry& other) const {
        // condition1
        return (mNeedAllMatch > other.mNeedAllMatch) ||
               // condition2
               (mNeedAllMatch == other.mNeedAllMatch &&
                m_raw_urc.size() < other.m_raw_urc.size()) ||
               // condition3
               (mNeedAllMatch == other.mNeedAllMatch &&
                m_raw_urc.size() == other.m_raw_urc.size() && m_slot_id > other.m_slot_id) ||
               // condition4
               (mNeedAllMatch == other.mNeedAllMatch &&
                m_raw_urc.size() == other.m_raw_urc.size() && m_slot_id == other.m_slot_id &&
                m_id > other.m_id) ||
               // condition5
               (mNeedAllMatch == other.mNeedAllMatch &&
                m_raw_urc.size() == other.m_raw_urc.size() && m_slot_id == other.m_slot_id &&
                m_id == other.m_id && m_raw_urc > other.m_raw_urc) ||
               // condition6
               (mNeedAllMatch == other.mNeedAllMatch &&
                m_raw_urc.size() == other.m_raw_urc.size() && m_slot_id == other.m_slot_id &&
                m_id == other.m_id && m_raw_urc == other.m_raw_urc &&
                m_client_id > other.m_client_id);
    }

    bool operator==(const RfxHandlerRegisterEntry& other) const {
        return (m_slot_id == other.m_slot_id) && (m_id == other.m_id) &&
               (mNeedAllMatch == other.mNeedAllMatch) &&
               (m_raw_urc.size() == other.m_raw_urc.size()) && (m_raw_urc == other.m_raw_urc) &&
               (m_client_id == other.m_client_id);
    }

  public:
    // wp<RfxBaseHandler> m_handler;
    RfxBaseHandler* m_handler;
    int m_channel_id;
    int m_slot_id;
    int m_id;

    // client id for user data transmitted by fragment protocol
    int m_client_id;
    String8 m_raw_urc;
    bool mNeedAllMatch;
};

class RfxHandlerManager {
  public:
    static RfxHandlerManager* init();

    static void registerHandler(RfxCreateHandlerFuncptr func_ptr, int c_id, int slot_category,
                                bool isOpReplaced, bool isOpHandler);

    static void registerToHandleRequest(RfxBaseHandler* handler, int channel_id, int slot_id,
                                        const int* request_id_list, int length);

    static void registerToHandleUrc(RfxBaseHandler* handler, int channel_id, int slot_id,
                                    const char** urc_prefix_list, int length,
                                    bool needAllMatch = false);

    static void registerToHandleEvent(RfxBaseHandler* handler, int channel_id, int slot_id,
                                      const int* event_id_list, int length);

    static void registerToHandleEvent(RfxBaseHandler* handler, int channel_id, int slot_id,
                                      int client_id, const int* event_id_list, int length);

    static void processMessage(const sp<RfxMclMessage>& msg);

    static void initHandler(int channel_id);

    static int findMsgChannel(int type, int slot_id, int id, int client_id, const char* urc);

  private:
    void registerInternal(Vector<RfxCreateHandlerFuncptr>& list, RfxCreateHandlerFuncptr func_ptr,
                          int c_id);

    void registerInternal(SortedVector<RfxHandlerRegisterEntry>& list, RfxBaseHandler* handler,
                          int channel_id, int slot_id, const int* id_list, int length);

    void registerInternal(SortedVector<RfxHandlerRegisterEntry>& list, RfxBaseHandler* handler,
                          int channel_id, int slot_id, int client_id, const int* id_list,
                          int length);

    void registerInternal(SortedVector<RfxHandlerRegisterEntry>& list, RfxBaseHandler* handler,
                          int channel_id, int slot_id, const char** urc_list, int length,
                          bool needAllMatch);

    RfxBaseHandler* findMsgHandler(SortedVector<RfxHandlerRegisterEntry>& list, int channelId,
                                   int slotId, int id, int client_id, const char* urc);
    RfxBaseHandler* findMsgHandlerInternal(SortedVector<RfxHandlerRegisterEntry>& list,
                                           int channelId, int slotId, int id, int client_id,
                                           const char* urc, bool needAllMatch);

    SortedVector<RfxHandlerRegisterEntry>* findListByType(int type);

    SortedVector<RfxHandlerRegisterEntry> findListByChannel(int type, int channel_id);

  private:
    static RfxHandlerManager* s_self;
    // handler & channel relationship
    Vector<RfxCreateHandlerFuncptr> m_slot_handler_list[RIL_PROXY_OFFSET];
    Vector<RfxCreateHandlerFuncptr> m_non_slot_handler_list[RIL_PROXY_OFFSET];

    // request & handle relationship
    // only register channel0-9, so remember to calculate offset according to slot
    SortedVector<RfxHandlerRegisterEntry> m_request_list[RIL_SUPPORT_CHANNELS];
    SortedVector<RfxHandlerRegisterEntry> m_urc_list[RIL_SUPPORT_CHANNELS];
    SortedVector<RfxHandlerRegisterEntry> m_event_list[RIL_SUPPORT_CHANNELS];
    mutable Mutex m_mutex[RIL_SUPPORT_CHANNELS];
};

#endif
