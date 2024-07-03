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

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "RmcOpDcImsReqHandler.h"
#include "RmcDcUtility.h"

#define RFX_LOG_TAG "RmcDcOpImsReqHandler"

/*****************************************************************************
 * Class RmcOpDcReqHandler
 *****************************************************************************/

RmcOpDcImsReqHandler::RmcOpDcImsReqHandler(int slot_id, int channel_id, RmcDcPdnManager* pdnManager)
    : RmcDcImsReqHandler(slot_id, channel_id, pdnManager) {
    RFX_LOG_D(RFX_LOG_TAG, "RmcOpDcImsReqHandler Ctor, slot_id=%d, channel_id=%d", slot_id,
              channel_id);
}

RmcOpDcImsReqHandler::~RmcOpDcImsReqHandler() {}
