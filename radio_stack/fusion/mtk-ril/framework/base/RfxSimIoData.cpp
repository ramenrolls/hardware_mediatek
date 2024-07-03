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

#include "RfxSimIoData.h"
#include <telephony/mtk_ril.h>

RFX_IMPLEMENT_DATA_CLASS(RfxSimIoData);

RfxSimIoData::RfxSimIoData(void* _data, int _length) : RfxBaseData(_data, _length) {
    if (_data != NULL) {
        RIL_SIM_IO_v6* pSimIoV6 = (RIL_SIM_IO_v6*)_data;
        RIL_SIM_IO_v6* pData = (RIL_SIM_IO_v6*)calloc(1, sizeof(RIL_SIM_IO_v6));
        RFX_ASSERT(pData != NULL);

        pData->command = pSimIoV6->command;
        pData->fileid = pSimIoV6->fileid;

        if (pSimIoV6->path != NULL) {
            asprintf(&pData->path, "%s", pSimIoV6->path);
        }

        pData->p1 = pSimIoV6->p1;
        pData->p2 = pSimIoV6->p2;
        pData->p3 = pSimIoV6->p3;

        if (pSimIoV6->data != NULL) {
            asprintf(&pData->data, "%s", pSimIoV6->data);
        }
        if (pSimIoV6->pin2 != NULL) {
            asprintf(&pData->pin2, "%s", pSimIoV6->pin2);
        }
        if (pSimIoV6->aidPtr != NULL) {
            asprintf(&pData->aidPtr, "%s", pSimIoV6->aidPtr);
        }

        m_data = (void*)pData;
        m_length = _length;
    }
}

RfxSimIoData::~RfxSimIoData() {
    // free memory
    if (m_data != NULL) {
        RIL_SIM_IO_v6* pData = (RIL_SIM_IO_v6*)m_data;
        if (pData->path != NULL) {
            free(pData->path);
            pData->path = NULL;
        }
        if (pData->data != NULL) {
            free(pData->data);
            pData->data = NULL;
        }
        if (pData->pin2 != NULL) {
            free(pData->pin2);
            pData->pin2 = NULL;
        }
        if (pData->aidPtr != NULL) {
            free(pData->aidPtr);
            pData->aidPtr = NULL;
        }
        free(m_data);
        m_data = NULL;
    }
}
