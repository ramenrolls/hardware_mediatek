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

#ifndef __RMC_IMS_PROVISIONING_REQUEST_HANDLER_H__
#define __RMC_IMS_PROVISIONING_REQUEST_HANDLER_H__

#include "RfxBaseHandler.h"
#include "RfxVoidData.h"
#include "RfxStringData.h"
#include "RfxStringsData.h"
#include "RfxMessageId.h"

#define RFX_LOG_TAG "RmcImsProvisioningRequestHandler"

class RmcImsProvisioningRequestHandler : public RfxBaseHandler {
    RFX_DECLARE_HANDLER_CLASS(RmcImsProvisioningRequestHandler);

  public:
    RmcImsProvisioningRequestHandler(int slot_id, int channel_id);
    virtual ~RmcImsProvisioningRequestHandler();

  protected:
    virtual void onHandleRequest(const sp<RfxMclMessage>& msg);

  private:
    void requestGetProvisionValue(const sp<RfxMclMessage>& msg);
    void requestSetProvisionValue(const sp<RfxMclMessage>& msg);
};

#endif
