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

#ifndef __RMC_IMS_RTT_CONTROL_REQUEST_HANDLER_H__
#define __RMC_IMS_RTT_CONTROL_REQUEST_HANDLER_H__

#include "RfxBaseHandler.h"
#include "RfxIntsData.h"
#include "RfxVoidData.h"
#include "RfxStringData.h"
#include "RfxStringsData.h"
#include "RfxMessageId.h"

#define RFX_LOG_TAG "RmcImsRttCtlReqHdl"

#define MAX_MD_CFGTYPE_NUM 3

class RmcImsRttControlRequestHandler : public RfxBaseHandler {
    RFX_DECLARE_HANDLER_CLASS(RmcImsRttControlRequestHandler);

  public:
    RmcImsRttControlRequestHandler(int slot_id, int channel_id);
    virtual ~RmcImsRttControlRequestHandler();

  protected:
    virtual void onHandleRequest(const sp<RfxMclMessage>& msg);

  private:
    void sendRttModifyRequest(const sp<RfxMclMessage>& msg);
    void sendRttText(const sp<RfxMclMessage>& msg);
    void rttModifyRequestResponse(const sp<RfxMclMessage>& msg);
    void toggleRttAudioIndication(const sp<RfxMclMessage>& msg);
};

#endif
