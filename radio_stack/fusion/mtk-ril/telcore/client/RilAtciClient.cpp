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
#include <sys/socket.h>
#include "RfxDispatchThread.h"
#include "RfxBasics.h"
#include "RfxLog.h"
#include "RfxMessageId.h"
#include "RilAtciClient.h"
#include "rfx_properties.h"

/*****************************************************************************
 * Class RilAtciClient
 *****************************************************************************/

#define RFX_LOG_TAG "RilAtciClient"
#define ATCI_SIM_PROP "persist.vendor.service.atci.sim"

RilAtciClient::RilAtciClient(int identity, char* socketName) : RilClient(identity, socketName) {}

RilAtciClient::~RilAtciClient() {}

void RilAtciClient::processCommands(void* buffer, size_t buflen, int clientId) {
    RFX_UNUSED(clientId);
    int request = RFX_MSG_REQUEST_OEM_HOOK_ATCI_INTERNAL;
    int token = 0xFFFFFFFF;
    char prop[RFX_PROPERTY_VALUE_MAX] = {0};
    rfx_property_get(ATCI_SIM_PROP, prop, "0");

    int slotId = atoi(prop);
    RFX_LOG_I(RFX_LOG_TAG, "slotId %d", slotId);
    if (slotId >= 0 && slotId < RIL_SOCKET_NUM) {
        RfxRequestInfo* pRI = (RfxRequestInfo*)calloc(1, sizeof(RfxRequestInfo));
        if (pRI == NULL) {
            RFX_LOG_E(RFX_LOG_TAG, "OOM");
            return;
        }
        pRI->socket_id = (RIL_SOCKET_ID)slotId;
        pRI->token = token;
        pRI->clientId = (ClientId)CLIENT_ID_ATCI;
        pRI->request = request;
        char* atBuffer = (char*)calloc(1, buflen + 1);
        if (atBuffer == NULL) {
            RFX_LOG_E(RFX_LOG_TAG, "fail to calloc buffer");
            free(pRI);
            return;
        }
        memcpy(atBuffer, buffer, buflen);
        RFX_LOG_I(RFX_LOG_TAG, "buffer %s, length %d", atBuffer, (int)buflen);
        rfx_enqueue_request_message_client(request, atBuffer, buflen, pRI, (RIL_SOCKET_ID)slotId);
        free(atBuffer);
    } else {
        RFX_LOG_E(RFX_LOG_TAG, "unsupported slot number");
    }
}

void RilAtciClient::requestComplete(RIL_Token token, RIL_Errno e, void* response,
                                    size_t responselen) {
    RFX_UNUSED(e);
    RfxRequestInfo* info = (RfxRequestInfo*)token;
    if (info != NULL && RFX_MSG_REQUEST_OEM_HOOK_ATCI_INTERNAL == info->request) {
        String8 strResult;
        if (response != NULL && responselen > 0) {
            strResult = String8((char*)response);
        } else {
            strResult = String8((char*)"\r\nERROR\r\n");
        }

        if (commandFd >= 0) {
            RFX_LOG_I(RFX_LOG_TAG, "commandFd is valid, strResult is %s", strResult.string());
            size_t len = strResult.size();
            ssize_t ret = send(commandFd, strResult, len, 0);
            if (ret != (ssize_t)len) {
                RFX_LOG_E(RFX_LOG_TAG, "lose data when send response.");
            }
        } else {
            RFX_LOG_E(RFX_LOG_TAG, "commandFd < 0");
        }
        free(info);
    }
}

void RilAtciClient::handleUnsolicited(int slotId, int unsolResponse, void* data, size_t datalen,
                                      UrcDispatchRule rule) {
    RFX_UNUSED(slotId);
    RFX_UNUSED(rule);
    if (RIL_UNSOL_ATCI_RESPONSE == unsolResponse) {
        if (data != NULL && datalen > 0) {
            String8 str = String8((char*)data);

            if (commandFd >= 0) {
                RFX_LOG_I(RFX_LOG_TAG, "commandFd is valid, str is %s", str.string());
                size_t len = str.size();
                ssize_t ret = send(commandFd, str, len, 0);
                if (ret != (ssize_t)len) {
                    RFX_LOG_E(RFX_LOG_TAG, "lose data when send response.");
                }
            } else {
                RFX_LOG_E(RFX_LOG_TAG, "commandFd < 0");
            }
        }
    }
}
