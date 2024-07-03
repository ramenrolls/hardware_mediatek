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

#ifndef __VIA_BASE_HANDLER_H__
#define __VIA_BASE_HANDLER_H__

/*****************************************************************************
 * Include
 *****************************************************************************/

#include "RfxBaseHandler.h"
#include "nw/RmcNetworkHandler.h"

/*****************************************************************************
 * Class ViaBaseHandler
 *****************************************************************************/

typedef enum {
    SIGNAL_CDMA_DBM = 0,
    SIGNAL_CDMA_ECIO = 1,
    SIGNAL_EVDO_DBM = 2,
    SIGNAL_EVDO_ECIO = 3
} C2K_SIGNAL;

class ViaBaseHandler {
  public:
    ViaBaseHandler() {}
    virtual ~ViaBaseHandler() {}

    virtual void sendCommandDemo(RfxBaseHandler* handler, char* str) = 0;
    virtual void handleCdmaSubscription(RfxBaseHandler* handler, char** p_response,
                                        RIL_Errno* result) = 0;
    virtual void requestSetPreferredVoicePrivacyMode(RfxBaseHandler* handler, int value,
                                                     RIL_Errno* result) = 0;
    virtual void requestQueryPreferredVoicePrivacyMode(RfxBaseHandler* handler, int* value,
                                                       RIL_Errno* result) = 0;
    virtual void handleCdmaPrlChanged(const sp<RfxMclMessage>& msg, RfxBaseHandler* handler,
                                      int slotId);
    virtual void registerForViaUrc(RfxBaseHandler* handler);
    virtual void handleViaUrc(const sp<RfxMclMessage>& msg, RfxBaseHandler* handler, int slotId);
    virtual const char** getViaAllowedUrcForNw() = 0;
    virtual int convertCdmaEvdoSig(int sig, int tag);
    virtual int getCdmaLocationInfo(RfxBaseHandler* handler,
                                    RIL_VOICE_REG_STATE_CACHE* voice_reg_state_cache);
    virtual const char** getAgpsUrc() = 0;
    virtual void requestAgpsConnind(RfxBaseHandler* handler, int connected, RIL_Errno* result) = 0;
    virtual int getCdmaLocationInfo(RfxBaseHandler* handler,
                                    CDMA_CELL_LOCATION_INFO* cdma_cell_location);
};

typedef ViaBaseHandler* create_t();
typedef void destroy_t(ViaBaseHandler*);

#endif /* __VIA_BASE_HANDLER_H__ */
