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

#ifndef RFX_RIL_ADAPTER_H_
#define RFX_RIL_ADAPTER_H_

#include "RfxObject.h"
#include "RfxMessage.h"
#include "RfxVariant.h"
#include "RfxStatusDefs.h"
#include "RfxDefs.h"
#include "RfxLog.h"

class RfxRilAdapter : public RfxObject {
    RFX_DECLARE_CLASS(RfxRilAdapter);
    RFX_OBJ_DECLARE_SINGLETON_CLASS(RfxRilAdapter);

  private:
    RfxRilAdapter();

    virtual ~RfxRilAdapter();

  public:
    void requestToMcl(const sp<RfxMessage>& message);
    void requestToMclWithDelay(const sp<RfxMessage>& message, nsecs_t nsec);
    void requestAckToRilj(const sp<RfxMessage>& message);
    bool responseToRilj(const sp<RfxMessage>& message);
    bool responseToBT(const sp<RfxMessage>& message);
};

#endif /* RFX_RIL_ADAPTER_H_ */
