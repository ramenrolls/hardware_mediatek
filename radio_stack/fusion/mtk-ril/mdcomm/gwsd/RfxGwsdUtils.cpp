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
#include "RfxGwsdUtils.h"

/*****************************************************************************
 * Class RfxGwsdUtils
 *****************************************************************************/

#define RFX_LOG_TAG "RfxGwsdUtils"
#define GWSD_RIL_PATH "libgwsd-ril.so"

RfxGwsdSettingBaseHandler* RfxGwsdUtils::m_settingHandler = NULL;
RfxGwsdCallControlBaseHandler* RfxGwsdUtils::m_callcontrolHandler = NULL;
void* RfxGwsdUtils::m_dlHandler = NULL;

void RfxGwsdUtils::initHandler() {
    if (RfxRilUtils::isGwsdSupport() == 0) {
        RFX_LOG_I(RFX_LOG_TAG, "[%s] not support gwsd", __FUNCTION__);
        return;
    }

    if (m_dlHandler != NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "[%s] return, m_dlHandler = %p", __FUNCTION__, m_dlHandler);
        return;
    }

    m_dlHandler = dlopen(GWSD_RIL_PATH, RTLD_NOW);
    if (m_dlHandler == NULL) {
        RFX_LOG_I(RFX_LOG_TAG, "[%s] dlopen failed in %s: %s", __FUNCTION__, GWSD_RIL_PATH,
                  dlerror());
        return;
    }

    dlerror();

    createSettingHandler_t* createSetting =
            (createSettingHandler_t*)dlsym(m_dlHandler, "createSettingHandler");
    const char* dlsym_error = dlerror();
    if (createSetting == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "[%s] create not defined or exported in %s: %s", __FUNCTION__,
                  GWSD_RIL_PATH, dlsym_error);
        return;
    }
    m_settingHandler = createSetting();

    createCallControlHandler_t* createCallControl =
            (createCallControlHandler_t*)dlsym(m_dlHandler, "createCallControlHandler");
    if (createCallControl == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "[%s] create not defined or exported in %s: %s", __FUNCTION__,
                  GWSD_RIL_PATH, dlsym_error);
        return;
    }
    m_callcontrolHandler = createCallControl();

    RFX_LOG_D(RFX_LOG_TAG, "[%s] completed", __FUNCTION__);
}

RfxGwsdSettingBaseHandler* RfxGwsdUtils::getSettingHandler() { return m_settingHandler; }

RfxGwsdCallControlBaseHandler* RfxGwsdUtils::getCallControlHandler() {
    return m_callcontrolHandler;
}

void RfxGwsdUtils::deInitHandler() {
    if (m_dlHandler == NULL) {
        RFX_LOG_I(RFX_LOG_TAG, "[%s] return, m_dlHandler == NULL", __FUNCTION__);
        return;
    }

    // release settingHandler resource
    if (m_settingHandler != NULL) {
        destroySettingHandler_t* destroyHandler =
                (destroySettingHandler_t*)dlsym(m_dlHandler, "destroySettingHandler");
        const char* dlsym_error = dlerror();
        if (destroyHandler == NULL) {
            RFX_LOG_E(RFX_LOG_TAG, "[%s] destroy not defined or exported in %s: %s", __FUNCTION__,
                      GWSD_RIL_PATH, dlsym_error);
        } else {
            destroyHandler(m_settingHandler);
            m_settingHandler = NULL;
        }
    }

    // release callcontrolHandler resource
    if (m_callcontrolHandler != NULL) {
        destroyCallControlHandler_t* destroyHandler =
                (destroyCallControlHandler_t*)dlsym(m_dlHandler, "destroyCallControlHandler");
        const char* dlsym_error = dlerror();
        if (destroyHandler == NULL) {
            RFX_LOG_E(RFX_LOG_TAG, "[%s] destroy not defined or exported in %s: %s", __FUNCTION__,
                      GWSD_RIL_PATH, dlsym_error);
        } else {
            destroyHandler(m_callcontrolHandler);
            m_callcontrolHandler = NULL;
        }
    }

    dlclose(m_dlHandler);
    m_dlHandler = NULL;
    RFX_LOG_D(RFX_LOG_TAG, "[%s] completed", __FUNCTION__);
}
