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
#include "RfxOpUtils.h"
#include "RfxDefs.h"
#include "libmtkrilutils.h"

/*****************************************************************************
 * Class RfxOpUtils
 *****************************************************************************/

#define RFX_LOG_TAG "RfxOpUtils"
#define OP_RIL_PATH "libmtk-rilop.so"
void* RfxOpUtils::sDlOpHandler = NULL;

void RfxOpUtils::initOpLibrary() {
    if (sDlOpHandler != NULL || isOMSupport()) {
        RFX_LOG_E(RFX_LOG_TAG, "[%s] return, sDlOpHandler = %p", __FUNCTION__, sDlOpHandler);
        return;
    }

    sDlOpHandler = dlopen(OP_RIL_PATH, RTLD_NOW);
    if (sDlOpHandler == NULL) {
        RFX_LOG_D(RFX_LOG_TAG, "[%s] dlopen failed in %s: %s", __FUNCTION__, OP_RIL_PATH,
                  dlerror());
        return;
    }

    // reset errors
    dlerror();

    RFX_LOG_D(RFX_LOG_TAG, "[%s] completed", __FUNCTION__);
}

const rfx_ctrler_factory_init* RfxOpUtils::getPreNonSlotOpCtlerList() {
    if (sDlOpHandler == NULL) {
        RFX_LOG_I(RFX_LOG_TAG, "[%s] return, sDlOpHandler == NULL", __FUNCTION__);
        return NULL;
    }

    const rfx_ctrler_factory_init* (*func)();
    func = (const rfx_ctrler_factory_init* (*)())dlsym(sDlOpHandler, "getPreNonSlotOpCtlerList");
    const char* dlsym_error = dlerror();
    if (func == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "[%s] destroy not defined or exported in %s: %s", __FUNCTION__,
                  OP_RIL_PATH, dlsym_error);
        return NULL;
    }

    const rfx_ctrler_factory_init* factoryTable = NULL;
    factoryTable = func();
    RFX_LOG_D(RFX_LOG_TAG, "[%s] completed", __FUNCTION__);
    return factoryTable;
}

const rfx_ctrler_factory_init* RfxOpUtils::getSlotOpCtlerList() {
    if (sDlOpHandler == NULL) {
        RFX_LOG_D(RFX_LOG_TAG, "[%s] return, sDlOpHandler == NULL", __FUNCTION__);
        return NULL;
    }

    const rfx_ctrler_factory_init* (*func)();
    func = (const rfx_ctrler_factory_init* (*)())dlsym(sDlOpHandler, "getSlotOpCtlerList");
    const char* dlsym_error = dlerror();
    if (func == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "[%s] destroy not defined or exported in %s: %s", __FUNCTION__,
                  OP_RIL_PATH, dlsym_error);
        return NULL;
    }

    const rfx_ctrler_factory_init* factoryTable = NULL;
    factoryTable = func();
    RFX_LOG_D(RFX_LOG_TAG, "[%s] completed", __FUNCTION__);
    return factoryTable;
}

int RfxOpUtils::getNumOfPreNonSlotOpCtler() {
    if (sDlOpHandler == NULL) {
        RFX_LOG_D(RFX_LOG_TAG, "[%s] return, sDlOpHandler == NULL", __FUNCTION__);
        return 0;
    }

    int (*func)();
    func = (int (*)())dlsym(sDlOpHandler, "getNumOfPreNonSlotOpCtler");
    const char* dlsym_error = dlerror();
    if (func == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "[%s] destroy not defined or exported in %s: %s", __FUNCTION__,
                  OP_RIL_PATH, dlsym_error);
        return 0;
    }

    int num = func();
    RFX_LOG_D(RFX_LOG_TAG, "[%s] completed", __FUNCTION__);
    return num;
}

int RfxOpUtils::getNumOfSlotOpCtler() {
    if (sDlOpHandler == NULL) {
        RFX_LOG_D(RFX_LOG_TAG, "[%s] return, sDlOpHandler == NULL", __FUNCTION__);
        return 0;
    }

    int (*func)();
    func = (int (*)())dlsym(sDlOpHandler, "getNumOfSlotOpCtler");
    const char* dlsym_error = dlerror();
    if (func == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "[%s] destroy not defined or exported in %s: %s", __FUNCTION__,
                  OP_RIL_PATH, dlsym_error);
        return 0;
    }

    int num = func();
    RFX_LOG_D(RFX_LOG_TAG, "[%s] completed", __FUNCTION__);
    return num;
}

const rfx_ctrler_factory_init* RfxOpUtils::getNonSlotOpCtlerList() {
    if (sDlOpHandler == NULL) {
        RFX_LOG_D(RFX_LOG_TAG, "[%s] return, sDlOpHandler == NULL", __FUNCTION__);
        return NULL;
    }

    const rfx_ctrler_factory_init* (*func)();
    func = (const rfx_ctrler_factory_init* (*)())dlsym(sDlOpHandler, "getNonSlotOpCtlerList");
    const char* dlsym_error = dlerror();
    if (func == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "[%s] destroy not defined or exported in %s: %s", __FUNCTION__,
                  OP_RIL_PATH, dlsym_error);
        return NULL;
    }

    const rfx_ctrler_factory_init* factoryTable = NULL;
    factoryTable = func();
    RFX_LOG_D(RFX_LOG_TAG, "[%s] completed", __FUNCTION__);
    return factoryTable;
}

int RfxOpUtils::getNumOfNonSlotOpCtler() {
    if (sDlOpHandler == NULL) {
        RFX_LOG_D(RFX_LOG_TAG, "[%s] return, sDlOpHandler == NULL", __FUNCTION__);
        return 0;
    }

    int (*func)();
    func = (int (*)())dlsym(sDlOpHandler, "getNumOfNonSlotOpCtler");
    const char* dlsym_error = dlerror();
    if (func == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "[%s] destroy not defined or exported in %s: %s", __FUNCTION__,
                  OP_RIL_PATH, dlsym_error);
        return 0;
    }

    int num = func();
    RFX_LOG_D(RFX_LOG_TAG, "[%s] completed", __FUNCTION__);
    return num;
}

int RfxOpUtils::getOpMsgIdFromRequestId(int requestId) {
    if (sDlOpHandler == NULL) {
        RFX_LOG_D(RFX_LOG_TAG, "[%s] return, sDlOpHandler == NULL", __FUNCTION__);
        return 0;
    }

    int (*func)(int);
    func = (int (*)(int))dlsym(sDlOpHandler, "getOpMsgIdFromRequestId");
    const char* dlsym_error = dlerror();
    if (func == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "[%s] destroy not defined or exported in %s: %s", __FUNCTION__,
                  OP_RIL_PATH, dlsym_error);
        return 0;
    }

    int msgId = func(requestId);
    RFX_LOG_D(RFX_LOG_TAG, "[%s] completed", __FUNCTION__);
    if (msgId == INVALID_ID) {
        RFX_LOG_D(RFX_LOG_TAG, "idToMsgId, no match ID in op package");
    }
    return msgId;
}

int RfxOpUtils::getOpRequestIdFromMsgId(int msgId) {
    if (sDlOpHandler == NULL) {
        RFX_LOG_D(RFX_LOG_TAG, "[%s] return, sDlOpHandler == NULL", __FUNCTION__);
        return 0;
    }

    int (*func)(int);
    func = (int (*)(int))dlsym(sDlOpHandler, "getOpRequestIdFromMsgId");
    const char* dlsym_error = dlerror();
    if (func == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "[%s] destroy not defined or exported in %s: %s", __FUNCTION__,
                  OP_RIL_PATH, dlsym_error);
        return 0;
    }

    int requestId = func(msgId);
    RFX_LOG_D(RFX_LOG_TAG, "[%s] completed", __FUNCTION__);
    if (requestId == INVALID_ID) {
        RFX_LOG_D(RFX_LOG_TAG, "idToMsgId, no match ID in op package");
    }
    return requestId;
}

void* RfxOpUtils::getOpHandler() {
    if (sDlOpHandler == NULL && !isOMSupport()) {
        initOpLibrary();
    }
    return sDlOpHandler;
}

void* RfxOpUtils::getDcImsOpHandler(int slotId, int channelId, void* pPdnManager) {
    if (sDlOpHandler == NULL) {
        RFX_LOG_I(RFX_LOG_TAG, "[%s] return, sDlOpHandler == NULL", __FUNCTION__);
        return 0;
    }

    void* (*func)(int, int, void*);
    func = (void* (*)(int, int, void*))dlsym(sDlOpHandler, "createDcImsOpReqHandler");

    const char* dlsym_error = dlerror();
    if (func == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "[%s] destroy not defined or exported in %s: %s", __FUNCTION__,
                  OP_RIL_PATH, dlsym_error);
        return 0;
    }

    void* handler = func(slotId, channelId, pPdnManager);
    return handler;
}

void* RfxOpUtils::getSimOpHandler(int type, int slotId, int channelId) {
    if (sDlOpHandler == NULL) {
        RFX_LOG_I(RFX_LOG_TAG, "[%s] return, sDlOpHandler == NULL", __FUNCTION__);
        return 0;
    }

    void* (*func)(int, int);
    if (type == MTK_RIL_SIM_GSM_REQ) {
        func = (void* (*)(int, int))dlsym(sDlOpHandler, "createGsmSimOpRequestHandler");
    } else if (type == MTK_RIL_SIM_CDMA_REQ) {
        func = (void* (*)(int, int))dlsym(sDlOpHandler, "createCdmaSimOpRequestHandler");
    } else if (type == MTK_RIL_SIM_COMM_REQ) {
        func = (void* (*)(int, int))dlsym(sDlOpHandler, "createCommSimOpRequestHandler");
    } else if (type == MTK_RIL_SIM_GSM_URC) {
        func = (void* (*)(int, int))dlsym(sDlOpHandler, "createGsmSimOpUrcHandler");
    } else if (type == MTK_RIL_SIM_CDMA_URC) {
        func = (void* (*)(int, int))dlsym(sDlOpHandler, "createCdmaSimOpUrcHandler");
    } else if (type == MTK_RIL_SIM_COMM_URC) {
        func = (void* (*)(int, int))dlsym(sDlOpHandler, "createCommSimOpUrcHandler");
    } else {
        return 0;
    }
    const char* dlsym_error = dlerror();
    if (func == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "[%s] destroy not defined or exported in %s: %s", __FUNCTION__,
                  OP_RIL_PATH, dlsym_error);
        return 0;
    }

    void* handler = func(slotId, channelId);
    return handler;
}
