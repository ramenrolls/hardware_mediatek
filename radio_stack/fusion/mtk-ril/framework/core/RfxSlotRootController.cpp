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
#include "RfxSlotRootController.h"
#include "RfxRilAdapter.h"
#include "RfxLog.h"

/*****************************************************************************
 * Class RfxSlotRootController
 *****************************************************************************/
RFX_IMPLEMENT_CLASS("RfxSlotRootController", RfxSlotRootController, RfxController);

RfxSlotRootController::RfxSlotRootController(int slot_id)
    : m_cs_requests(NULL), m_ps_requests(NULL) {
    m_slot_id = slot_id;
}

RfxSlotRootController::~RfxSlotRootController() {
    if (m_cs_requests) {
        delete (m_cs_requests);
    }

    if (m_ps_requests) {
        delete (m_ps_requests);
    }
}

void RfxSlotRootController::regReqToCsRild(const int* request_id_list, size_t length) {
    if (m_cs_requests == NULL) {
        m_cs_requests = new Vector<int>();
    }
    registerRequestInternal(request_id_list, length, m_cs_requests);
}

void RfxSlotRootController::regReqToPsRild(const int* request_id_list, size_t length) {
    if (m_ps_requests == NULL) {
        m_ps_requests = new Vector<int>();
    }
    registerRequestInternal(request_id_list, length, m_ps_requests);
}

void RfxSlotRootController::registerRequestInternal(const int* request_id_list, size_t length,
                                                    Vector<int>* list) {
    if (list == NULL) {
        RFX_ASSERT(0);
    }

    for (size_t i = 0; i < length; i++) {
        list->add(request_id_list[i]);
    }

    registerToHandleRequest(request_id_list, length);
}

void RfxSlotRootController::unregisterRequestInternal(const int* request_id_list, size_t length,
                                                      Vector<int>* list) {
    if (!list) {
        return;
    }
    size_t i = 0;
    while (i < list->size()) {
        int id = list->itemAt(i);
        bool found = false;
        for (size_t j = 0; j < length; j++) {
            if (id == request_id_list[j]) {
                list->removeAt(i);
                found = true;
                break;
            }
        }
        if (!found) {
            i++;
        }
    }

    unregisterToHandleRequest(request_id_list, length);
}

bool RfxSlotRootController::onHandleRequest(const sp<RfxMessage>& message) {
    int id = message->getId();

    if (m_cs_requests) {
        size_t size = m_cs_requests->size();
        for (size_t i = 0; i < size; i++) {
            if (m_cs_requests->itemAt(i) == id) {
                int cs_type = getStatusManager()->getIntValue(RFX_STATUS_KEY_VOICE_TYPE,
                                                              RADIO_TECH_GROUP_GSM);
                sp<RfxMessage> req_msg;
                if (cs_type == RADIO_TECH_GROUP_C2K) {
                    req_msg = RfxMessage::obtainRequest(RADIO_TECH_GROUP_C2K, id, message, true);
                } else if (cs_type == RADIO_TECH_GROUP_GSM) {
                    req_msg = RfxMessage::obtainRequest(RADIO_TECH_GROUP_GSM, id, message, true);
                } else {
                    RFX_ASSERT(0);
                }
                requestToMcl(req_msg);
                return true;
            }
        }
    }

    if (m_ps_requests) {
        size_t size = m_ps_requests->size();
        for (size_t i = 0; i < size; i++) {
            if (m_ps_requests->itemAt(i) == id) {
                int ps_type = getStatusManager()->getIntValue(RFX_STATUS_KEY_DATA_TYPE,
                                                              RADIO_TECH_GROUP_GSM);
                sp<RfxMessage> req_msg;
                if (ps_type == RADIO_TECH_GROUP_C2K) {
                    req_msg = RfxMessage::obtainRequest(RADIO_TECH_GROUP_C2K, id, message, true);
                } else if (ps_type == RADIO_TECH_GROUP_GSM) {
                    req_msg = RfxMessage::obtainRequest(RADIO_TECH_GROUP_GSM, id, message, true);
                } else {
                    RFX_ASSERT(0);
                }
                requestToMcl(req_msg);
                return true;
            }
        }
    }
    RFX_ASSERT(0);  // the request not registered
    return false;
}

bool RfxSlotRootController::onHandleResponse(const sp<RfxMessage>& message) {
    int id = message->getId();
    if (m_cs_requests) {
        size_t size = m_cs_requests->size();
        for (size_t i = 0; i < size; i++) {
            if (m_cs_requests->itemAt(i) == id) {
                responseToRilj(message);
                return true;
            }
        }
    }
    if (m_ps_requests) {
        size_t size = m_ps_requests->size();
        for (size_t i = 0; i < size; i++) {
            if (m_ps_requests->itemAt(i) == id) {
                responseToRilj(message);
                return true;
            }
        }
    }
    RFX_ASSERT(0);  // the response not registered
    return false;
}
