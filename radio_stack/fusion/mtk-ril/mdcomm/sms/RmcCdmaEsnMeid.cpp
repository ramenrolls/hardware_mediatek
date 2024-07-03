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
#include "RmcCdmaEsnMeid.h"

/*****************************************************************************
 * Register Data Class
 *****************************************************************************/
RFX_REGISTER_DATA_TO_URC_ID(RmcStringUrsp, RFX_MSG_URC_CDMA_CARD_INITIAL_ESN_OR_MEID);

/*****************************************************************************
 * Class RmcCdmaCardEsnMeidHdlr
 *****************************************************************************/

bool RmcCdmaCardEsnMeidHdlr::onHandleRawUrc(RfxAtLine* line) {
    int err;
    char* oldEsnMe = line->atTokNextstr(&err);
    if (err < 0 || oldEsnMe == NULL) {
        return false;
    }
    char* newEsnMe = line->atTokNextstr(&err);
    if (err < 0 || oldEsnMe == NULL) {
        return false;
    }
    m_oldEsnMe.setTo(oldEsnMe);
    m_newEsnMe.setTo(newEsnMe);
    return true;
}

RmcBaseUrspData* RmcCdmaCardEsnMeidHdlr::onGetUrcData(int slotId) {
    return new RmcStringUrsp(RFX_MSG_URC_CDMA_CARD_INITIAL_ESN_OR_MEID, slotId,
                             m_oldEsnMe + String8(",") + m_newEsnMe);
}
