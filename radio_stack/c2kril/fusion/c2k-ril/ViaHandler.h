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

#ifndef __VIA_HANDLER_H__
#define __VIA_HANDLER_H__

/*****************************************************************************
 * Include
 *****************************************************************************/

#include "RfxBaseHandler.h"
#include "ViaBaseHandler.h"

/*****************************************************************************
 * Class ViaHandler
 *****************************************************************************/

class ViaHandler : public ViaBaseHandler {
  public:
    ViaHandler();
    virtual ~ViaHandler();
    void sendCommandDemo(RfxBaseHandler* handler, char* str);
    void handleCdmaSubscription(RfxBaseHandler* handler, char** p_response, RIL_Errno* result);
    void requestSetPreferredVoicePrivacyMode(RfxBaseHandler* handler, int value, RIL_Errno* result);
    void requestQueryPreferredVoicePrivacyMode(RfxBaseHandler* handler, int* value,
                                               RIL_Errno* result);
    void handleCdmaPrlChanged(const sp<RfxMclMessage>& msg, RfxBaseHandler* handler, int slotId);
    void registerForViaUrc(RfxBaseHandler* handler);
    void handleViaUrc(const sp<RfxMclMessage>& msg, RfxBaseHandler* handler, int slotId);
    const char** getViaAllowedUrcForNw();
    int convertCdmaEvdoSig(int sig, int tag);
    int getCdmaLocationInfo(RfxBaseHandler* handler,
                            RIL_VOICE_REG_STATE_CACHE* voice_reg_state_cache);
    const char** getAgpsUrc();
    void requestAgpsConnind(RfxBaseHandler* handler, int connected, RIL_Errno* result);
    int getCdmaLocationInfo(RfxBaseHandler* handler, CDMA_CELL_LOCATION_INFO* cdma_cell_location);

  private:
    int mSystemId;
};

extern "C" ViaBaseHandler* create() { return new ViaHandler; }

extern "C" void destroy(ViaBaseHandler* handler) { delete handler; }

#endif /* __VIA_HANDLER_H__ */
