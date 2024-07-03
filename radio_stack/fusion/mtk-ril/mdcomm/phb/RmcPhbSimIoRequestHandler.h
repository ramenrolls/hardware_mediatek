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

#ifndef __RMC_PHB_SIMIO_REQUEST_HANDLER_H__
#define __RMC_PHB_SIMIO_REQUEST_HANDLER_H__

#include "RmcSimBaseHandler.h"
#include "RfxIntsData.h"
#include "RfxStringData.h"
#include "RfxMessageId.h"
#include "RfxVoidData.h"

class RmcPhbSimIoRequestHandler : public RmcSimBaseHandler {
    RFX_DECLARE_HANDLER_CLASS(RmcPhbSimIoRequestHandler);

  public:
    RmcPhbSimIoRequestHandler(int slot_id, int channel_id);
    virtual ~RmcPhbSimIoRequestHandler();

  protected:
    virtual void onHandleRequest(const sp<RfxMclMessage>& msg);

  private:
    void requestHandlePhbSimIo(const sp<RfxMclMessage>& msg);
    void makePhbSimRspFromUsimFcp(unsigned char** simResponse);

  private:
    int mIsEngLoad = -1;
};

#endif /*__RMC_PHB_SIMIO_REQUEST_HANDLER_H__*/
