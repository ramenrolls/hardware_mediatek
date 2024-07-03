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

#ifndef __RMC_OP_CALL_CONTROL_COMMON_REQUEST_HANDLER_H__
#define __RMC_OP_CALL_CONTROL_COMMON_REQUEST_HANDLER_H__

#include <telephony/mtk_rilop.h>

#include "cc/RmcCallControlCommonRequestHandler.h"

class RmcOpCallControlCommonRequestHandler : public RmcCallControlCommonRequestHandler {
    RFX_DECLARE_HANDLER_CLASS(RmcOpCallControlCommonRequestHandler);

  public:
    RmcOpCallControlCommonRequestHandler(int slotId, int channelId);
    virtual ~RmcOpCallControlCommonRequestHandler();

  protected:
    virtual void onHandleRequest(const sp<RfxMclMessage>& msg);
    virtual void onHandleEvent(const sp<RfxMclMessage>& msg);

  private:
    void requestOp08GetCurrentCalls(const sp<RfxMclMessage>& msg);
    int callFromCLCCSLineOp08(RfxAtLine* pLine, RIL_Call* p_call);
    int callFromCLCCLineOp08(RfxAtLine* pLine, RIL_Call* p_call);
    void setIncomingVirtualLine(const sp<RfxMclMessage>& msg);
    void setTrn(const sp<RfxMclMessage>& msg);

    void handleVirtualLineTimeout(const sp<RfxMclMessage>& msg);

    static char cachedVirtualFrom[MAX_INCOMING_LINE_LENGTH];
    static char cachedVirtualTo[MAX_INCOMING_LINE_LENGTH];
    static int cachedVirtualToken;
    static int mtCallCount;
};

#endif
