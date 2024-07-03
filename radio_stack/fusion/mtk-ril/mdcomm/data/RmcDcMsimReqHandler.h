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

#ifndef __RMC_DC_MSIM_REQ_HANDLER_H__
#define __RMC_DC_MSIM_REQ_HANDLER_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "RfxBaseHandler.h"

// Mapping to err cause: 4112, 4117 (multiple PS allow error)
#define RIL_E_OEM_MULTI_ALLOW_ERR RIL_E_OEM_ERROR_1

/*****************************************************************************
 * Class RmcDcMsimReqHandler
 *****************************************************************************/
class RmcDcMsimReqHandler : public RfxBaseHandler {
    RFX_DECLARE_HANDLER_CLASS(RmcDcMsimReqHandler);

  public:
    RmcDcMsimReqHandler(int slot_id, int channel_id);
    virtual ~RmcDcMsimReqHandler();

  private:
    void handleRequestAllowData(const sp<RfxMclMessage>& msg);
    void setDataAllowed(int allowed, const sp<RfxMclMessage>& msg);
    void handleDataConnectionAttachRequest(const sp<RfxMclMessage>& msg);
    void handleDataConnectionDetachRequest(const sp<RfxMclMessage>& msg);
    void updateDataAllowStatus(int slotId, int allow);
    void updatePreDataAllowStatus(int slotId, int allow);

  protected:
    virtual void onHandleRequest(const sp<RfxMclMessage>& msg);
};
#endif /* __RMC_MSIM_REQ_HANDLER_H__ */
