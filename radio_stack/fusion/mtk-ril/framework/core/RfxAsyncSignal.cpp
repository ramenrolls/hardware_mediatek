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

#include "RfxBasics.h"
#include "RfxAsyncSignal.h"

/*****************************************************************************
 * Define
 *****************************************************************************/

#define RFX_LOG_TAG "SlotQueueEntry"

#define RFX_ASYNC_SIGNAL_EMIT_LOOP_THRESHOLD 1000
#define RFX_ASYNC_SIGNAL_EMIT_BURST_THRESHOLD 3000

/*****************************************************************************
 * Class RfxAsyncSignalQueue
 *****************************************************************************/

RFX_IMPLEMENT_CLASS(RFX_ASYNC_SIGNAL_QUEUE_CLASS_NAME, RfxAsyncSignalQueue, RfxObject);
RFX_OBJ_IMPLEMENT_SINGLETON_CLASS(RfxAsyncSignalQueue);

void RfxAsyncSignalQueue::putSlot(RfxObject* obj, RfxAsyncSlotObjMemFuncPtrEx memFunc, void* data) {
    RFX_OBJ_ASSERT_VALID(obj);
    RFX_ASSERT(memFunc != NULL);

    // Cerate a new list entry
    SlotQueueEntry* entry = new SlotQueueEntry(obj, memFunc, data);

    // Add the slot to the tail of list
    if (m_list_tail != NULL) {
        m_list_tail->m_next = entry;
    }
    m_list_tail = entry;

    // Add to the head if the list is empty
    if (m_list_head == NULL) {
        m_list_head = entry;
    }
}

void RfxAsyncSignalQueue::clear() {
    SlotQueueEntry* i = m_list_head;
    while (i != NULL) {
        SlotQueueEntry* next = i->m_next;

        delete (i);

        i = next;
    }
    m_list_head = NULL;
}

void RfxAsyncSignalQueue::processEmit() {
    int loopCountDown = RFX_ASYNC_SIGNAL_EMIT_LOOP_THRESHOLD;
    do {
        // NOTE:
        //  Assert here if it have infinit loop with post emit.
        //  For example, emit a post in callback ltself
        RFX_ASSERT(loopCountDown != 0);
        loopCountDown--;

        int burstCountDown = RFX_ASYNC_SIGNAL_EMIT_BURST_THRESHOLD;
        while (1) {
            // NOTE:
            //  Assert here if there are burst post emits
            //  For example, emit so many post in one loop
            RFX_ASSERT(burstCountDown > 0);
            burstCountDown--;
            SlotQueueEntry* entry = getSlot();
            if (entry == NULL) {
                // No more entry in the queue
                break;
            }

            // The target is invalid, ignore it
            RfxObject* obj = entry->m_target_ptr.promote().get();
            if (obj == NULL) {
                delete (entry);
                continue;
            }

            RFX_OBJ_ASSERT_VALID(obj);

            RfxAsyncSlotObjMemFuncPtrEx callback = entry->m_callback;
            (obj->*callback)(entry->m_data);

            delete (entry);
        }  // while (1)
    } while (!isEmpty());

    // VFX_LOG(VFX_INFO3, VFX_MMI_CHECK_UPDATE_END);
}

RfxAsyncSignalQueue::SlotQueueEntry* RfxAsyncSignalQueue::getSlot() {
    SlotQueueEntry* first_entry = m_list_head;
    if (first_entry != NULL) {
        m_list_head = first_entry->m_next;

        if (m_list_head == NULL) {
            // If the queue becomes empty, pointer the last entry to NULL
            m_list_tail = NULL;
        }
    }

    return first_entry;
}
