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

#include "RfxAuthMsgData.h"
#include <telephony/mtk_ril.h>

RFX_IMPLEMENT_DATA_CLASS(RfxAuthMsgData);

#define RFX_LOG_TAG "RfxAuthMsgData"

RfxAuthMsgData::RfxAuthMsgData(void* data, int length) : RfxBaseData(data, length) {
    if (data != NULL) {
        RIL_AuthMsg* pOriginal = (RIL_AuthMsg*)data;
        RIL_AuthMsg* pData = (RIL_AuthMsg*)calloc(1, sizeof(RIL_AuthMsg));
        if (pData == NULL) {
            RFX_LOG_E(RFX_LOG_TAG, "OOM");
            return;
        }

        pData->uid = pOriginal->uid;
        pData->msgLength = pOriginal->msgLength;
        if (pOriginal->msg != NULL) {
            pData->msg = (char*)calloc(1, pData->msgLength + 1);
            if (pData->msg == NULL) {
                mtkLogD(RFX_LOG_TAG, "OOM");
                return;
            }
            memcpy(pData->msg, pOriginal->msg, pData->msgLength);
            pData->msg[pData->msgLength] = '\0';
        }

        m_data = pData;
        m_length = length;
    }
}

/*int RfxAuthMsgData::getMsgLength() {
    return mMsgLength;
}*/

RfxAuthMsgData::~RfxAuthMsgData() {
    // free memory
    if (m_data != NULL) {
        RIL_AuthMsg* pData = (RIL_AuthMsg*)m_data;
        if (pData->msg != NULL) {
            free(pData->msg);
        }
    }
}
