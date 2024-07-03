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
#include "RmcCdmaSmsMemFull.h"

/*****************************************************************************
 * Register Data Class
 *****************************************************************************/
RFX_REGISTER_DATA_TO_URC_ID(RmcVoidUrsp, RFX_MSG_URC_CDMA_RUIM_SMS_STORAGE_FULL);

RFX_REGISTER_DATA_TO_URC_ID(RmcVoidUrsp, RFX_MSG_URC_ME_SMS_STORAGE_FULL);

/*****************************************************************************
 * Class RmcCdmaSmsMemFullHdlr
 *****************************************************************************/
bool RmcCdmaSmsMemFullHdlr::onHandleRawUrc(RfxAtLine* line) {
    int err;
    char* type = line->atTokNextstr(&err);
    if (err < 0 || type == NULL) {
        return false;
    }
    m_memType.setTo(type);
    return true;
}

RmcBaseUrspData* RmcCdmaSmsMemFullHdlr::onGetUrcData(int slotId) {
    if (String8("SM") == m_memType) {
        return new RmcVoidUrsp(RFX_MSG_URC_CDMA_RUIM_SMS_STORAGE_FULL, slotId);

    } else if (String8("ME") == m_memType) {
        return new RmcVoidUrsp(RFX_MSG_URC_ME_SMS_STORAGE_FULL, slotId);
    } else {
        return NULL;
    }
}
