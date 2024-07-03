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
#include <dlfcn.h>
#include "RfxViaUtils.h"
#include "ratconfig.h"

/*****************************************************************************
 * Class RfxViaUtils
 *****************************************************************************/

#define RFX_LOG_TAG "RfxViaUtils"
#define VIA_RIL_PATH "libvia-ril.so"

ViaBaseHandler* RfxViaUtils::m_viaHandler = NULL;
void* RfxViaUtils::m_dlHandler = NULL;

void RfxViaUtils::initViaHandler() {
    if (RatConfig_isC2kSupported() == 0) {
        RFX_LOG_I(RFX_LOG_TAG, "[%s] not support C2K", __FUNCTION__);
        return;
    }

    if (m_dlHandler != NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "[%s] return, m_dlHandler = %p", __FUNCTION__, m_dlHandler);
        return;
    }

    m_dlHandler = dlopen(VIA_RIL_PATH, RTLD_NOW);
    if (m_dlHandler == NULL) {
        RFX_LOG_I(RFX_LOG_TAG, "[%s] dlopen failed in %s: %s", __FUNCTION__, VIA_RIL_PATH,
                  dlerror());
        return;
    }

    // reset errors
    dlerror();

    create_t* createViaHandler = (create_t*)dlsym(m_dlHandler, "create");
    const char* dlsym_error = dlerror();
    if (createViaHandler == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "[%s] create not defined or exported in %s: %s", __FUNCTION__,
                  VIA_RIL_PATH, dlsym_error);
        return;
    }

    // create an instance of ViaHandler
    m_viaHandler = createViaHandler();
    RFX_LOG_D(RFX_LOG_TAG, "[%s] completed", __FUNCTION__);
}

ViaBaseHandler* RfxViaUtils::getViaHandler() { return m_viaHandler; }

void RfxViaUtils::deInitViaHandler() {
    if (m_dlHandler == NULL) {
        RFX_LOG_I(RFX_LOG_TAG, "[%s] return, m_dlHandler == NULL", __FUNCTION__);
        return;
    }

    destroy_t* destroyViaHandler = (destroy_t*)dlsym(m_dlHandler, "destroy");
    const char* dlsym_error = dlerror();
    if (destroyViaHandler == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "[%s] destroy not defined or exported in %s: %s", __FUNCTION__,
                  VIA_RIL_PATH, dlsym_error);
        return;
    }

    destroyViaHandler(m_viaHandler);
    m_viaHandler = NULL;
    dlclose(m_dlHandler);
    m_dlHandler = NULL;
    RFX_LOG_D(RFX_LOG_TAG, "[%s] completed", __FUNCTION__);
}
