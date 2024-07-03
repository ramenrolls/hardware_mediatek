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

#ifndef __RMC_VT_URC_HANDLER_H__
#define __RMC_VT_URC_HANDLER_H__

#include "RfxBaseHandler.h"
#include "RfxRawData.h"
#include "RmcVtMsgParser.h"

typedef struct {
    int anbrq_config;
    int ebi;
    int is_ul;
    int bitrate;
    int bearer_id;
    int pdu_session_id;
    int ext_param;
} RIL_EANBR;

typedef struct {
    int sim_slot_id;
    int irat_status;
    int is_successful;
} RIL_EIRAT;

class RmcVtUrcHandler : public RfxBaseHandler {
    RFX_DECLARE_HANDLER_CLASS(RmcVtUrcHandler);

  public:
    RmcVtUrcHandler(int slot_id, int channel_id);
    virtual ~RmcVtUrcHandler();

  protected:
    virtual void onHandleUrc(const sp<RfxMclMessage>& msg);

  private:
    void handleEANBR(const sp<RfxMclMessage>& msg);
    void handleEIRAT(const sp<RfxMclMessage>& msg);
};

#endif
