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

#include "RfxDialogInfoData.h"

#include "RfxLog.h"
#define RFX_LOG_TAG "RfxDialogInfoData"

RFX_IMPLEMENT_DATA_CLASS(RfxDialogInfoData);

RfxDialogInfoData::RfxDialogInfoData(void* data, int length) : RfxBaseData(data, length) {
    if (data == NULL) {
        m_data = NULL;
        return;
    }

    RFX_LOG_D(RFX_LOG_TAG, "RfxDialogInfoData start length = %d", length);
    m_length = length;
    m_data = NULL;
    int countOfDialog = m_length / sizeof(RIL_DialogInfo*);
    RIL_DialogInfo** dialogInfo = (RIL_DialogInfo**)calloc(countOfDialog, sizeof(RIL_DialogInfo*));
    RFX_ASSERT(dialogInfo != NULL);
    if (countOfDialog > 0) {
        RIL_DialogInfo** tmp = (RIL_DialogInfo**)data;
        for (int i = 0; i < countOfDialog; i++) {
            RFX_LOG_D(RFX_LOG_TAG, "RfxDialogInfoData memcpy i = %d", i);
            dialogInfo[i] = (RIL_DialogInfo*)calloc(1, sizeof(RIL_DialogInfo));
            RFX_ASSERT(dialogInfo[i] != NULL);
            memset(dialogInfo[i], 0, sizeof(RIL_DialogInfo));
            // memcpy(dialogInfo[i], tmp[i], sizeof(RIL_DialogInfo));

            dialogInfo[i]->dialogId = tmp[i]->dialogId;
            dialogInfo[i]->callState = tmp[i]->callState;
            dialogInfo[i]->callType = tmp[i]->callType;
            dialogInfo[i]->isCallHeld = tmp[i]->isCallHeld;
            dialogInfo[i]->isPullable = tmp[i]->isPullable;
            dialogInfo[i]->isMt = tmp[i]->isMt;

            if (tmp[i]->address != NULL) {
                int len = strlen(tmp[i]->address);
                // Allocate and zero-initialize array
                dialogInfo[i]->address = (char*)calloc(len + 1, sizeof(char));
                strncpy(dialogInfo[i]->address, tmp[i]->address, len);
            }
            if (tmp[i]->remoteAddress != NULL) {
                int len = strlen(tmp[i]->remoteAddress);
                // Allocate and zero-initialize array
                dialogInfo[i]->remoteAddress = (char*)calloc(len + 1, sizeof(char));
                strncpy(dialogInfo[i]->remoteAddress, tmp[i]->remoteAddress, len);
            }
            /*RFX_LOG_V(RFX_LOG_TAG,
                    "RfxDialogInfoData set dialogId:%d, address: %s, remoteAddress: %s",
                    dialogInfo[i]->dialogId, dialogInfo[i]->address,
               dialogInfo[i]->remoteAddress);*/
        }
    }
    RFX_LOG_D(RFX_LOG_TAG, "RfxDialogInfoData end");
    m_data = dialogInfo;
}

RfxDialogInfoData::~RfxDialogInfoData() {
    // free
    int countOfCall = m_length / sizeof(RIL_DialogInfo*);
    RIL_DialogInfo** dialog_info = (RIL_DialogInfo**)m_data;

    if (dialog_info != NULL) {
        for (int i = 0; i < countOfCall; i++) {
            if (dialog_info[i] != NULL) {
                if (dialog_info[i]->address != NULL) {
                    free(dialog_info[i]->address);
                }
            }
        }
        free(dialog_info);
    }
    m_data = NULL;
    RFX_LOG_D(RFX_LOG_TAG, "~RfxDialogInfoData");
}
