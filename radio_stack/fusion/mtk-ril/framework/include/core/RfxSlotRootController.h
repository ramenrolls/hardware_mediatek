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

#ifndef __RFX_SLOT_ROOT_CONTROLLER_H__
#define __RFX_SLOT_ROOT_CONTROLLER_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "utils/Vector.h"
#include "RfxController.h"

using ::android::Vector;
/*****************************************************************************
 * Class RfxSlotRootController
 *****************************************************************************/

class RfxSlotRootController : public RfxController {
    RFX_DECLARE_CLASS(RfxSlotRootController);

    // Constructor / Destructor
  public:
    RfxSlotRootController() : m_cs_requests(NULL), m_ps_requests(NULL) {}

    explicit RfxSlotRootController(int slot_id);

    virtual ~RfxSlotRootController();

    void regReqToCsRild(const int* request_id_list, size_t length);

    void regReqToPsRild(const int* request_id_list, size_t length);

    void unregReqToCsRild(const int* request_id_list, size_t length) {
        unregisterRequestInternal(request_id_list, length, m_cs_requests);
    }

    void unregReqToPsRild(const int* request_id_list, size_t length) {
        unregisterRequestInternal(request_id_list, length, m_ps_requests);
    }

  protected:
    virtual bool onHandleRequest(const sp<RfxMessage>& message);

    virtual bool onHandleResponse(const sp<RfxMessage>& message);

  private:
    void registerRequestInternal(const int* request_id_list, size_t length, Vector<int>* list);

    void unregisterRequestInternal(const int* request_id_list, size_t length, Vector<int>* list);

  private:
    Vector<int>* m_cs_requests;

    Vector<int>* m_ps_requests;
};

#endif /* __RFX_SLOT_ROOT_CONTROLLER_H__ */
