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

#include "Rfx.h"
#include "RfxMainThread.h"
#include "RfxRilAdapter.h"
#include "RfxDispatchThread.h"
#include "RfxLog.h"
#include "SapSocketManager.h"
#include "RfxMclDispatcherThread.h"
#include "RfxDataCloneManager.h"
#include "RfxMclMessage.h"
#include "RfxViaUtils.h"
#include <binder/ProcessState.h>
#include <libmtkrilutils.h>
#include "RfxOpUtils.h"
#include "RfxGwsdUtils.h"

/*****************************************************************************
 * Implementation
 *****************************************************************************/
RfxDispatchThread* dispatchThread;

#define RFX_LOG_TAG "RFX"

int RFX_SLOT_COUNT = 2;

void rfx_init(void) {
#ifdef MTK_USE_HIDL
    android::ProcessState::initWithDriver("/dev/vndbinder");
#endif
    RFX_SLOT_COUNT = getSimCount();
    RFX_LOG_D(RFX_LOG_TAG, "rfx_init (RFX_SLOT_COUNT=%d, SIM_COUNT=%d)", RFX_SLOT_COUNT, SIM_COUNT);

    // init operator library for operator software decouple
    RfxOpUtils::initOpLibrary();
    // init ViaHandler
    RfxViaUtils::initViaHandler();
    // init MdComm layer
    RfxMclDispatcherThread::init();
    // init TelCore layer
    RfxMainThread::init();
    dispatchThread = RfxDispatchThread::init();
    // init Gwsd
    RfxGwsdUtils::initHandler();
}

void rfx_enqueue_request_message(int request, void* data, size_t datalen, RIL_Token t,
                                 RIL_SOCKET_ID socketId) {
    char property_value[RFX_PROPERTY_VALUE_MAX] = {0};
    rfx_property_get("persist.vendor.ril.test_mode", property_value, "0");
    if (atoi(property_value) == 1) {
        RFX_LOG_D(RFX_LOG_TAG, "test mode on, return not support : %d", request);
        RfxRequestInfo* requestInfo = (RfxRequestInfo*)t;
        sp<RfxMessage> resMsg =
                RfxMessage::obtainResponse(socketId, request, requestInfo->token, INVALID_ID, -1,
                                           RIL_E_REQUEST_NOT_SUPPORTED, NULL, 0, t);
        MessageObj* obj = createMessageObj(resMsg);
        dispatchResponseQueue.enqueue(obj);
        return;
    }
    dispatchThread->enqueueRequestMessage(request, data, datalen, t, socketId);
}

void rfx_enqueue_request_message_client(int request, void* data, size_t datalen, RIL_Token t,
                                        RIL_SOCKET_ID socketId) {
    char property_value[RFX_PROPERTY_VALUE_MAX] = {0};
    rfx_property_get("persist.vendor.ril.test_mode", property_value, "0");
    RfxRequestInfo* requestInfo = (RfxRequestInfo*)t;
    int clientId = requestInfo->clientId;
    if (atoi(property_value) == 1 && (clientId != CLIENT_ID_MTTS1 && clientId != CLIENT_ID_MTTS2 &&
                                      clientId != CLIENT_ID_MTTS3 && clientId != CLIENT_ID_MTTS4)) {
        RFX_LOG_D(RFX_LOG_TAG, "test mode but not MTTS request, return not support : %d", request);
        sp<RfxMessage> resMsg =
                RfxMessage::obtainResponse(socketId, request, requestInfo->token, INVALID_ID, -1,
                                           RIL_E_REQUEST_NOT_SUPPORTED, NULL, 0, t);
        MessageObj* obj = createMessageObj(resMsg);
        dispatchResponseQueue.enqueue(obj);
        return;
    }
    dispatchThread->enqueueRequestMessage(request, data, datalen, t, socketId, clientId);
}

void rfx_enqueue_sap_request_message(int request, void* data, size_t datalen, RIL_Token t,
                                     RIL_SOCKET_ID socketId) {
    char property_value[RFX_PROPERTY_VALUE_MAX] = {0};
    rfx_property_get("persist.vendor.ril.test_mode", property_value, "0");
    if (atoi(property_value) == 1) {
        RFX_LOG_D(RFX_LOG_TAG, "test mode on, return not support : %d", request);
        RfxRequestInfo* requestInfo = (RfxRequestInfo*)t;
        sp<RfxMessage> resMsg =
                RfxMessage::obtainResponse(socketId, request, requestInfo->token, INVALID_ID, -1,
                                           RIL_E_REQUEST_NOT_SUPPORTED, NULL, 0, t);
        MessageObj* obj = createMessageObj(resMsg);
        dispatchResponseQueue.enqueue(obj);
        return;
    }
    dispatchThread->enqueueSapRequestMessage(request, data, datalen, t, socketId);
}

void rfx_update_connection_state(RIL_SOCKET_ID socketId, int isConnected) {
    dispatchThread->updateConnectionState(socketId, isConnected);
}
