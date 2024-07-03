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
#include "RilOpProxy.h"
#include <mtk_log.h>
#include "libmtkrilutils.h"

/*****************************************************************************
 * Define
 *****************************************************************************/
#undef LOG_TAG
#define LOG_TAG "RilOpProxy"

/*****************************************************************************
 * Class RilOpProxy
 *****************************************************************************/

#define OP_RIL_PATH "libmtk-rilop.so"
void* RilOpProxy::sDlOpHandler = NULL;

void RilOpProxy::initOpLibrary() {
    if (sDlOpHandler != NULL) {
        mtkLogI(LOG_TAG, "[%s] return, sDlOpHandler = %p", __FUNCTION__, sDlOpHandler);
        return;
    }

    sDlOpHandler = dlopen(OP_RIL_PATH, RTLD_NOW);
    if (sDlOpHandler == NULL) {
        mtkLogI(LOG_TAG, "[%s] dlopen failed in %s: %s", __FUNCTION__, OP_RIL_PATH, dlerror());
        return;
    }

    // reset errors
    dlerror();

    mtkLogI(LOG_TAG, "[%s] completed", __FUNCTION__);
}

void* RilOpProxy::getOpHandler() { return sDlOpHandler; }

void RilOpProxy::registerOpService(RIL_RadioFunctions* callbacks, android::CommandInfo* commands) {
    if (sDlOpHandler == NULL) {
        mtkLogI(LOG_TAG, "[%s] dlopen failed in %s: %s", __FUNCTION__, OP_RIL_PATH, dlerror());
        return;
    }

    // reset errors
    dlerror();

    mtkLogI(LOG_TAG, "[%s] completed", __FUNCTION__);

    void (*func)(RIL_RadioFunctions*, android::CommandInfo*);
    func = (void (*)(RIL_RadioFunctions*, android::CommandInfo*))dlsym(sDlOpHandler,
                                                                       "registerOpService");
    const char* dlsym_error = dlerror();
    if (func == NULL) {
        mtkLogI(LOG_TAG, "[%s] destroy not defined or exported in %s: %s", __FUNCTION__,
                OP_RIL_PATH, dlsym_error);
        return;
    }

    func(callbacks, commands);
    mtkLogI(LOG_TAG, "[%s] completed", __FUNCTION__);
}

android::CommandInfo* RilOpProxy::getOpCommandInfo(int request) {
    android::CommandInfo* pCi;
    if (sDlOpHandler == NULL || isOMSupport()) {
        mtkLogI(LOG_TAG, "[%s] dlopen failed in %s: %s", __FUNCTION__, OP_RIL_PATH, dlerror());
        return NULL;
    }

    android::CommandInfo* (*func)(int request);
    func = (android::CommandInfo * (*)(int request)) dlsym(sDlOpHandler, "getOpCommandInfo");
    const char* dlsym_error = dlerror();
    if (func == NULL) {
        mtkLogI(LOG_TAG, "[%s] destroy not defined or exported in %s: %s", __FUNCTION__,
                OP_RIL_PATH, dlsym_error);
        return NULL;
    }

    pCi = func(request);
    mtkLogI(LOG_TAG, "[%s] completed", __FUNCTION__);

    return pCi;
}

android::UnsolResponseInfo* RilOpProxy::getOpUnsolResponseInfo(int unsolResponse) {
    if (sDlOpHandler == NULL || isOMSupport()) {
        mtkLogI(LOG_TAG, "[%s] dlopen failed in %s: %s", __FUNCTION__, OP_RIL_PATH, dlerror());
        return NULL;
    }

    android::UnsolResponseInfo* pUnsolResponseInfo = NULL;

    android::UnsolResponseInfo* (*func)(int unsolResponse);
    func = (android::UnsolResponseInfo * (*)(int unsolResponse))
            dlsym(sDlOpHandler, "getOpUnsolResponseInfo");
    const char* dlsym_error = dlerror();
    if (func == NULL) {
        mtkLogI(LOG_TAG, "[%s] destroy not defined or exported in %s: %s", __FUNCTION__,
                OP_RIL_PATH, dlsym_error);
        return NULL;
    }

    pUnsolResponseInfo = func(unsolResponse);
    mtkLogD(LOG_TAG, "[%s] completed", __FUNCTION__);

    return pUnsolResponseInfo;
}
