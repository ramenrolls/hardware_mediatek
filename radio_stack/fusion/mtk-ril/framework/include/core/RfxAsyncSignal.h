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

#ifndef __RFX_ASYNC_SIGNAL_H__
#define __RFX_ASYNC_SIGNAL_H__

/*****************************************************************************
 * Include
 *****************************************************************************/

#include "RfxLog.h"
#include "RfxObject.h"
#include "utils/RefBase.h"

using ::android::RefBase;
using ::android::sp;
using ::android::wp;

/*****************************************************************************
 * Define
 *****************************************************************************/

#define RFX_ASYNC_SIGNAL_QUEUE_CLASS_NAME "AsyncSignalQueue"

/*****************************************************************************
 * Class RfxAsyncSignalQueue
 *****************************************************************************/

// Event callback
typedef void (RfxObject::*RfxAsyncSlotObjMemFuncPtrEx)(void* data);
// RfxAsyncSignal0 callbak
typedef void (RfxObject::*RfxAsyncSlotObjMemFuncPtr)();

class RfxAsyncSignalQueue : public RfxObject {
    RFX_DECLARE_CLASS(RfxAsyncSignalQueue);
    RFX_OBJ_DECLARE_SINGLETON_CLASS(RfxAsyncSignalQueue);

    // Constructor / Destructor
  public:
    // Default constructor
    RfxAsyncSignalQueue() : m_list_head(NULL), m_list_tail(NULL) {}

    // Destructor
    virtual ~RfxAsyncSignalQueue() { clear(); }

    // Methods
  public:
    // Test if the slot list is empty
    bool isEmpty() const { return m_list_head == NULL; }

    // Add a slot into the tail of slot queue
    void putSlot(RfxObject* obj, RfxAsyncSlotObjMemFuncPtrEx memFunc, void* data);

    // Add a slot into the tail of slot queue (without event)
    void putSlot0(RfxObject* obj, RfxAsyncSlotObjMemFuncPtr memFunc) {
        putSlot(obj, (RfxAsyncSlotObjMemFuncPtrEx)memFunc, NULL);
    }

    // Clear all slots in the slot list
    void clear();

    // Framewrok methods
  public:
    void processEmit();

    // Implementation
  private:
    class SlotQueueEntry {
      public:
        SlotQueueEntry(RfxObject* obj, RfxAsyncSlotObjMemFuncPtrEx memFunc, void* data)
            : m_target_ptr(obj), m_callback(memFunc), m_data(data), m_next(NULL) {}

      public:
        wp<RfxObject> m_target_ptr;
        RfxAsyncSlotObjMemFuncPtrEx m_callback;
        void* m_data;
        SlotQueueEntry* m_next;
    };

    SlotQueueEntry* m_list_head;  // Pointer to the first slot entry of queue
    SlotQueueEntry* m_list_tail;  // Pointer to the last slot entry of queue

    // Get the first slot entry of the queue, and remove it from queue.
    // RETURNS: The first of slot entry of the queue.
    //          Return NULL if the queue is empty.
    SlotQueueEntry* getSlot();
};

/*****************************************************************************
 * Global Functions
 *****************************************************************************/

// A helper invoke post without type safe.
// This function is for internal use by framework.
template <class _MemFunc>
inline void rfxPostInvoke(RfxObject* obj, _MemFunc func, void* data = NULL) {
    RfxAsyncSignalQueue* queue = RFX_OBJ_GET_INSTANCE(RfxAsyncSignalQueue);
    queue->putSlot(obj, static_cast<RfxAsyncSlotObjMemFuncPtrEx>(func), data);
}

#endif /* __RFX_ASYNC_SIGNAL_H__ */
