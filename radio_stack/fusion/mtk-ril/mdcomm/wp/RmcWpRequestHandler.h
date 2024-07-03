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

#ifndef __RMC_WP_REQUEST_HANDLER_H__
#define __RMC_WP_REQUEST_HANDLER_H__

#include "RfxBaseHandler.h"
#include "RfxIntsData.h"
#include "RfxStringData.h"
#include "RfxMessageId.h"
#include "RfxVoidData.h"
#include <libmtkrilutils.h>

typedef enum {
    WORLD_PHONE_POLYCY_OP01A = 1,
    WORLD_PHONE_POLYCY_OP01B = 2,
    WORLD_PHONE_POLYCY_OM = 3,
    WORLD_PHONE_POLYCY_OP02 = 4,
} WorldPhonePolicyType;

class RmcWpRequestHandler : public RfxBaseHandler {
    RFX_DECLARE_HANDLER_CLASS(RmcWpRequestHandler);

  public:
    RmcWpRequestHandler(int slot_id, int channel_id);
    virtual ~RmcWpRequestHandler();

  public:
    static int isWorldModeSwitching();
    void setActiveModemType();
    int getActiveModemType();
    int queryActiveMode();

  protected:
    virtual void onHandleRequest(const sp<RfxMclMessage>& msg);

  private:
    void requestResumingWorldMode(const sp<RfxMclMessage>& msg);
    void onWorldModeStateChanged(int state);
    void handleWorldModeChanged(const sp<RfxMclMessage>& msg);
    WorldPhonePolicyType getWorldPhonePolicy();
    void setWorldPhonePolicy();
    int isWorldPhoneSupport();
    SVLTE_PROJ_TYPE getSvlteProjectType();
    int isSvlteLcgSupport();
    void worldPhoneInitialize(int slot_id);

  private:
    static int bWorldModeSwitching;
    static int ecsraUrcParams[5];
    static RIL_RadioState radioStateBackup[MAX_SIM_COUNT];
};

#endif /*__RMC_WP_REQUEST_HANDLER_H__*/
