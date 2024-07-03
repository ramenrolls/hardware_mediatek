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

#include <string.h>
#include "RfxSetDataProfileData.h"
#include "RfxLog.h"

#define RFX_LOG_TAG "RfxSetDataProfileData"

RFX_IMPLEMENT_DATA_CLASS(RfxSetDataProfileData);

RfxSetDataProfileData::RfxSetDataProfileData(void* data, int length) : RfxBaseData(data, length) {
    m_length = length;
    int num = m_length / sizeof(RIL_MtkDataProfileInfo*);
    if (num == 0 || data == NULL) {
        RFX_LOG_E(RFX_LOG_TAG,
                  "[%s] The number of DataProfile is %d (if num != 0, means data is null)",
                  __FUNCTION__, num);
        return;
    }

    RIL_MtkDataProfileInfo** curPtr = (RIL_MtkDataProfileInfo**)data;
    RIL_MtkDataProfileInfo** tmpPtr =
            (RIL_MtkDataProfileInfo**)calloc(num, sizeof(RIL_MtkDataProfileInfo*));
    RFX_ASSERT(tmpPtr != NULL);

    for (int i = 0; i < num; i++) {
        RIL_MtkDataProfileInfo* dataPtr =
                (RIL_MtkDataProfileInfo*)calloc(1, sizeof(RIL_MtkDataProfileInfo));
        RFX_ASSERT(dataPtr != NULL);

        tmpPtr[i] = dataPtr;

        // profileId
        dataPtr->profileId = curPtr[i]->profileId;
        // apn
        if ((curPtr[i])->apn != NULL) {
            asprintf(&dataPtr->apn, "%s", curPtr[i]->apn);
        } else {
            asprintf(&dataPtr->apn, "");
        }
        // protocol
        if ((curPtr[i])->protocol != NULL) {
            asprintf(&dataPtr->protocol, "%s", curPtr[i]->protocol);
        } else {
            asprintf(&dataPtr->protocol, "");
        }
        // roamingProtocol
        if ((curPtr[i])->roamingProtocol != NULL) {
            asprintf(&dataPtr->roamingProtocol, "%s", curPtr[i]->roamingProtocol);
        } else {
            asprintf(&dataPtr->roamingProtocol, "");
        }
        // authType
        dataPtr->authType = curPtr[i]->authType;
        // user
        if ((curPtr[i])->user != NULL) {
            asprintf(&dataPtr->user, "%s", curPtr[i]->user);
        } else {
            asprintf(&dataPtr->user, "");
        }
        // password
        if ((curPtr[i])->password != NULL) {
            asprintf(&dataPtr->password, "%s", curPtr[i]->password);
        } else {
            asprintf(&dataPtr->password, "");
        }
        // type
        dataPtr->type = curPtr[i]->type;
        // maxConnsTime
        dataPtr->maxConnsTime = curPtr[i]->maxConnsTime;
        // maxConns
        dataPtr->maxConns = curPtr[i]->maxConns;
        // waitTime
        dataPtr->waitTime = curPtr[i]->waitTime;
        // enabled
        dataPtr->enabled = curPtr[i]->enabled;
        // supportedTypesBitmask
        dataPtr->supportedTypesBitmask = curPtr[i]->supportedTypesBitmask;
        // bearerBitmask
        dataPtr->bearerBitmask = curPtr[i]->bearerBitmask;
        // mtu
        dataPtr->mtu = curPtr[i]->mtu;
        // mvnoType
        if ((curPtr[i])->mvnoType != NULL) {
            asprintf(&dataPtr->mvnoType, "%s", curPtr[i]->mvnoType);
        } else {
            asprintf(&dataPtr->mvnoType, "");
        }
        // mvnoMatchData
        if ((curPtr[i])->mvnoMatchData != NULL) {
            asprintf(&dataPtr->mvnoMatchData, "%s", curPtr[i]->mvnoMatchData);
        } else {
            asprintf(&dataPtr->mvnoMatchData, "");
        }
        // inactiveTimer
        dataPtr->inactiveTimer = curPtr[i]->inactiveTimer;
    }
    m_data = tmpPtr;
}

RfxSetDataProfileData::~RfxSetDataProfileData() {
    RIL_MtkDataProfileInfo** tmpPtr = (RIL_MtkDataProfileInfo**)m_data;
    int num = m_length / sizeof(RIL_MtkDataProfileInfo*);

    if (tmpPtr != NULL) {
        // free memory
        for (int i = 0; i < num; i++) {
            RIL_MtkDataProfileInfo* dataPtr = (RIL_MtkDataProfileInfo*)tmpPtr[i];
            RFX_ASSERT(dataPtr != NULL);
            FREEIF(dataPtr->apn);
            FREEIF(dataPtr->protocol);
            FREEIF(dataPtr->roamingProtocol);
            FREEIF(dataPtr->user);
            FREEIF(dataPtr->password);
            FREEIF(dataPtr->mvnoType);
            FREEIF(dataPtr->mvnoMatchData);
            free(dataPtr);
        }
        free(tmpPtr);
        tmpPtr = NULL;
    }
}
