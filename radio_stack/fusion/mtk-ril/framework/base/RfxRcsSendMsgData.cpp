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

#include "RfxRcsSendMsgData.h"

RFX_IMPLEMENT_DATA_CLASS(RfxRcsSendMsgData);

RfxRcsSendMsgData::RfxRcsSendMsgData(void* _data, int _length) : RfxBaseData(_data, _length) {
    if (_data != NULL) {
        char* sendData = NULL;
        sendData = (char*)calloc(_length, sizeof(char));
        if (sendData != NULL) {
            memcpy(sendData, _data, _length);

            m_data = (void*)sendData;
            m_length = _length;
            // RFX_LOG_I("RfxRcsSendMsgData", "sendData = %p, m_length = %d", m_data, m_length);
        } else {
            RFX_LOG_E("RfxRcsSendMsgData", "calloc return null");
        }
    }
}

RfxRcsSendMsgData::~RfxRcsSendMsgData() {
    // free memory
    if (m_data != NULL) {
        free(m_data);
        m_data = NULL;
    }
}
