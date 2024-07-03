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

#include "RfxLog.h"
#include "RfxRedialData.h"
#include "RfxRilUtils.h"

RFX_IMPLEMENT_DATA_CLASS(RfxRedialData);

RfxRedialData::RfxRedialData(void* data, int length) : RfxBaseData(data, length) {
    RFX_Redial* dupData;
    RFX_Redial* srcData = (RFX_Redial*)data;

    dupData = (RFX_Redial*)calloc(1, sizeof(RFX_Redial));
    RFX_ASSERT(dupData != NULL);
    mDialData = new RfxDialData(srcData->dial_data, sizeof(RIL_Dial));

    dupData->dial_data = (RIL_Dial*)(mDialData->getData());
    dupData->call_id = srcData->call_id;

    m_length = sizeof(RFX_Redial);
    m_data = dupData;

    mCallId = srcData->call_id;
}

RfxRedialData::RfxRedialData(RIL_Dial* data, int callId) : RfxBaseData(NULL, 0) {
    RFX_Redial* dupData;

    dupData = (RFX_Redial*)calloc(1, sizeof(RFX_Redial));
    RFX_ASSERT(dupData != NULL);
    mDialData = new RfxDialData(data, sizeof(RIL_Dial));

    dupData->dial_data = (RIL_Dial*)(mDialData->getData());
    dupData->call_id = callId;

    m_length = sizeof(RFX_Redial);
    m_data = dupData;

    mCallId = callId;
}

RfxRedialData::~RfxRedialData() {
    if (mDialData != NULL) {
        delete (mDialData);
    }

    if (m_data != NULL) {
        free(m_data);
    }
}

void RfxRedialData::dump() {
    RFX_LOG_D(
            "RfxRedialData", "Dump mCallId:%d, number:%s", mCallId,
            (RfxRilUtils::isUserLoad() ? "[hidden]" : (((RFX_Redial*)m_data)->dial_data)->address));
}
