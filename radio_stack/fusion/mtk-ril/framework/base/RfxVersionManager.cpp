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

#include "RfxVersionManager.h"
#include <semaphore.h>
#include "utils/Mutex.h"
#include "RfxLog.h"
#include "RfxRilUtils.h"

using ::android::Mutex;

#define RFX_LOG_TAG "RfxVersionMgr"

static sem_t sWaitLooperSem;
static bool sNeedWaitLooper = true;
static Mutex sWaitLooperMutex;

RfxVersionManager* RfxVersionManager::sSelf = NULL;

void RfxVersionManager::waitVersion() {
    sWaitLooperMutex.lock();
    if (sNeedWaitLooper) {
        RFX_LOG_D(RFX_LOG_TAG, "waitLooper() begin");
        sem_wait(&sWaitLooperSem);
        sNeedWaitLooper = false;
        sem_destroy(&sWaitLooperSem);
        RFX_LOG_D(RFX_LOG_TAG, "waitLooper() end");
    }
    sWaitLooperMutex.unlock();
}

RfxVersionManager* RfxVersionManager::init() {
    if (sSelf == NULL) {
        RFX_LOG_D(RFX_LOG_TAG, "init");
        sSelf = new RfxVersionManager();
        sem_init(&sWaitLooperSem, 0, 0);
    }
    return sSelf;
}

RfxVersionManager* RfxVersionManager::getInstance() { return sSelf; }

void RfxVersionManager::initVersion(RfxAtLine* line) {
    int err = 0;
    char* feature = NULL;
    int version = 0;

    if (err < 0) {
        RFX_LOG_E(RFX_LOG_TAG, "initVersion error: %d!", err);
        return;
    }

    if (RfxRilUtils::getRilRunMode() == RIL_RUN_MODE_MOCK) {
        RFX_LOG_E(RFX_LOG_TAG, "initVersion ignored in UT test");
        return;
    }

    while (1) {
        line->atTokStart(&err);
        feature = line->atTokNextstr(&err);
        version = line->atTokNextint(&err);
        FeatureVersion fv(String8(feature), version);
        mVersionList.add(fv);
        if (NULL == line->getNext()) {
            break;
        }
        line = line->getNext();
    }

    // debug
    for (size_t i = 0; i < mVersionList.size(); i++) {
        FeatureVersion fv = mVersionList.itemAt(i);
        RFX_LOG_D(RFX_LOG_TAG, "[%zu] feature: [%s], version: [%d]", i, fv.getFeature().string(),
                  fv.getVersion());
    }

    // finish to update all feature version
    sem_post(&sWaitLooperSem);
}

int RfxVersionManager::getFeatureVersion(char* feature) {
    waitVersion();
    return getFeatureVersion(feature, 0);
}

int RfxVersionManager::getFeatureVersion(char* feature, int defaultVaule) {
    if (RfxRilUtils::getRilRunMode() == RIL_RUN_MODE_MOCK) {
        RFX_LOG_E(RFX_LOG_TAG, "return default version on UT");
        return defaultVaule;
    }
    waitVersion();
    for (size_t i = 0; i < mVersionList.size(); i++) {
        FeatureVersion fv = mVersionList.itemAt(i);
        if (fv.getFeature() == String8(feature)) {
            RFX_LOG_D(RFX_LOG_TAG, "find version: %d", fv.getVersion());
            return fv.getVersion();
        }
    }
    return defaultVaule;
}
