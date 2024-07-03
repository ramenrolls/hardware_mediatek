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

#ifndef __RFX_ROOT_CONTROLLER_H__
#define __RFX_ROOT_CONTROLLER_H__

/*****************************************************************************
 * Include
 *****************************************************************************/

#include "utils/SortedVector.h"
#include "utils/RefBase.h"
#include "utils/List.h"
#include "RfxController.h"
#include "RfxStatusManager.h"
#include "RfxRilAdapter.h"
#include "RfxDefs.h"

using ::android::SortedVector;
using ::android::sp;
using ::android::Vector;
using ::android::wp;

extern int RFX_SLOT_COUNT;

/*****************************************************************************
 * Class RfxRegistryEntry
 *****************************************************************************/
class RfxRegistryEntry {
  public:
    RfxRegistryEntry()
        : id(-1),
          slot_id(RFX_SLOT_ID_UNKNOWN),
          controller(NULL),
          priority(RfxController::HANDLER_PRIORITY::DEFAULT),
          msg_token(-1) {}

    RfxRegistryEntry(int _id, int _slot_id, wp<RfxController>& _controller,
                     RfxController::HANDLER_PRIORITY _priority, int token)
        : id(_id),
          slot_id(_slot_id),
          controller(_controller),
          priority(_priority),
          msg_token(token) {}

    RfxRegistryEntry(const RfxRegistryEntry& other)
        : id(other.id),
          slot_id(other.slot_id),
          controller(other.controller),
          priority(other.priority),
          msg_token(other.msg_token) {}

    bool operator<(const RfxRegistryEntry& rhs) const {
        return (id < rhs.id) || (id == rhs.id && slot_id < rhs.slot_id) ||
               (id == rhs.id && slot_id == rhs.slot_id && priority < rhs.priority) ||
               (id == rhs.id && slot_id == rhs.slot_id && priority == rhs.priority &&
                msg_token < rhs.msg_token);
    }

    bool operator>(const RfxRegistryEntry& rhs) const {
        return (id > rhs.id) || (id == rhs.id && slot_id > rhs.slot_id) ||
               (id == rhs.id && slot_id == rhs.slot_id && priority > rhs.priority) ||
               (id == rhs.id && slot_id == rhs.slot_id && priority == rhs.priority &&
                msg_token > rhs.msg_token);
    }

    bool operator==(const RfxRegistryEntry& rhs) const {
        return id == rhs.id && slot_id == rhs.slot_id && priority == rhs.priority &&
               msg_token == rhs.msg_token;
    }

  public:
    int id;
    int slot_id;
    wp<RfxController> controller;
    RfxController::HANDLER_PRIORITY priority;
    int msg_token;
};

/*****************************************************************************
 * Class RfxSuspendedMsgEntry
 *****************************************************************************/
class RfxSuspendedMsgEntry {
  public:
    RfxSuspendedMsgEntry() : controller(NULL), message(NULL) {}

    RfxSuspendedMsgEntry(RfxController* handler, const sp<RfxMessage>& msg)
        : controller(handler), message(msg) {}

    RfxSuspendedMsgEntry(const RfxSuspendedMsgEntry& other)
        : controller(other.controller), message(other.message) {}

    RfxSuspendedMsgEntry& operator=(const RfxSuspendedMsgEntry& other) {
        controller = other.controller;
        message = other.message;
        return *this;
    }

  public:
    wp<RfxController> controller;
    sp<RfxMessage> message;
};

/*****************************************************************************
 * Class RfxRootController
 *****************************************************************************/

class RfxRootController : public RfxController {
    RFX_DECLARE_CLASS(RfxRootController);
    RFX_OBJ_DECLARE_SINGLETON_CLASS(RfxRootController);

    // Constructor / Destructor
  public:
    // Constructor
    RfxRootController() {
        for (int index = 0; index < MAX_RFX_SLOT_ID + 1; index++) {
            m_status_managers[index] = NULL;
            m_slot_root_controllers[index] = NULL;
        }
    }

    virtual ~RfxRootController() {}

    // Method
  public:
    void processSuspendedMessage();

    virtual bool processMessage(const sp<RfxMessage>& message);

    virtual bool processAtciRequest(const sp<RfxMessage>& message);

    virtual void clearMessages();
    // Overridable
  public:
    void registerToHandleRequest(RfxController* controller, int slot_id, const int* request_id_list,
                                 size_t length, HANDLER_PRIORITY priority = DEFAULT);

    void registerToHandleResponse(RfxController* controller, int slot_id,
                                  const int* response_id_list, size_t length, int token = -1);

    void registerToHandleUrc(RfxController* controller, int slot_id, const int* urc_id_list,
                             size_t length);

    void unregisterToHandleRequest(int slot_id, const int* request_id_list, size_t length,
                                   HANDLER_PRIORITY priority = DEFAULT);

    void unregisterToHandleUrc(int slot_id, const int* urc_id_list, size_t length);

    RfxStatusManager* getStatusManager(int slot_id) const;

    RfxController* getSlotRootController(int slot_id) const;

    void setSlotRootController(int slot_id, RfxController* slot_root);

    void suspendMessage(RfxController* controller, const sp<RfxMessage>& message);

    void clearRegistry(RfxController* controller);

  protected:
    virtual void onInit();

  private:
    SortedVector<RfxRegistryEntry> m_request_list;
    SortedVector<RfxRegistryEntry> m_response_list;
    SortedVector<RfxRegistryEntry> m_urc_list;
    Vector<RfxSuspendedMsgEntry> m_suspended_msg_list;
    // add one more status manage for no-slot controllers
    RfxStatusManager* m_status_managers[MAX_RFX_SLOT_ID + 1];
    // add one more for append non-slot controllers
    RfxController* m_slot_root_controllers[MAX_RFX_SLOT_ID + 1];

  private:
    void registerInternal(SortedVector<RfxRegistryEntry>& list, RfxController* controller,
                          int slot_id, const int* id_list, size_t length,
                          HANDLER_PRIORITY priority = DEFAULT, int token = -1);

    void unregisterInternal(SortedVector<RfxRegistryEntry>& list, int slot_id, const int* id_list,
                            size_t length, HANDLER_PRIORITY priority = DEFAULT, int token = -1);

    RfxController* findMsgHandler(int id, int slot_id, SortedVector<RfxRegistryEntry>& list,
                                  HANDLER_PRIORITY priority = DEFAULT, int token = -1);

    void clearRegistryInternal(SortedVector<RfxRegistryEntry>& list, RfxController* controller);

    void handleSendResponseAck(const sp<RfxMessage>& message);
};

#endif /* __RFX_ROOT_CONTROLLER_H__ */
