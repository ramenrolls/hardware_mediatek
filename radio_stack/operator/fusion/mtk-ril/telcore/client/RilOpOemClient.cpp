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

#include "RilOpOemClient.h"
#include "RfxMessageId.h"
#include <sys/socket.h>
#include "RfxOpUtils.h"
#include "RfxDispatchThread.h"

#define RFX_LOG_TAG "RilOpOemClient"
#define OEM_MAX_PARA_LENGTH 768
#define ISRSUFUNCTION(cmd, function) (0 == strcmp(cmd, (char*)function))
#define ISRSUMESSAGE(id)                                                                        \
    (((id == RFX_MSG_REQUEST_GET_SHARED_KEY) ||                                                 \
      (id == RFX_MSG_REQUEST_UPDATE_SIM_LOCK_SETTINGS) ||                                       \
      (id == RFX_MSG_REQUEST_GET_SIM_LOCK_INFO) ||                                              \
      (id == RFX_MSG_REQUEST_RESET_SIM_LOCK_SETTINGS) ||                                        \
      (id == RFX_MSG_REQUEST_GET_MODEM_STATUS) || (id == RFX_MSG_REQUEST_ATT_GET_SHARED_KEY) || \
      (id == RFX_MSG_REQUEST_ATT_UPDATE_SIM_LOCK_SETTINGS) ||                                   \
      (id == RFX_MSG_REQUEST_ATT_GET_SIM_LOCK_VERSION) ||                                       \
      (id == RFX_MSG_REQUEST_ATT_RESET_SIM_LOCK_SETTINGS) ||                                    \
      (id == RFX_MSG_REQUEST_ATT_GET_MODEM_STATUS) ||                                           \
      (id == RFX_MSG_REQUEST_ATT_GET_MODEM_CONFIG) ||                                           \
      (id == RFX_MSG_REQUEST_ATT_GET_API_VERSION) || (id == RFX_MSG_REQUEST_ATT_GET_SIM_SLOTS)) \
             ? true                                                                             \
             : false)

int RilOpOemClient::mRsuFd = -1;

RilOpOemClient::RilOpOemClient(int identity, char* socketName)
    : RilOemClient(identity, socketName) {
    RFX_LOG_D(RFX_LOG_TAG, "RilOpOemClient");
}

RilOpOemClient::~RilOpOemClient() {}

int RilOpOemClient::handleSpecialRequestWithArgs(int argCount, char** args) {
    char* cmd;
    char orgArgs[OEM_MAX_PARA_LENGTH] = {0};
    RfxAtLine* line;
    int err = 0;
    int slotId = 0;
    char* data = NULL;

    RFX_LOG_D(RFX_LOG_TAG, "handleSpecialRequestWithArgs : %d", argCount);
    if (1 == argCount) {
        strncpy(orgArgs, args[0], OEM_MAX_PARA_LENGTH - 1);
        line = new RfxAtLine(orgArgs, NULL);
        cmd = line->atTokNextstr(&err);
        if (err < 0) {
            RFX_LOG_E(RFX_LOG_TAG, "invalid command");
            delete line;
            return FAILURE;
        }
        slotId = line->atTokNextint(&err);
        if (line->atTokHasmore()) {
            data = line->atTokNextstr(&err);
            RFX_LOG_I(RFX_LOG_TAG, "data : %p", data);
            if (err < 0) {
                delete line;
                return FAILURE;
            }
        }
        if (ISRSUFUNCTION(cmd, "SIMMELOCK_GETKEY") || ISRSUFUNCTION(cmd, "SIMMELOCK_SET") ||
            ISRSUFUNCTION(cmd, "SIMMELOCK_GET_VERSION") || ISRSUFUNCTION(cmd, "SIMMELOCK_RESET") ||
            ISRSUFUNCTION(cmd, "SIMMELOCK_GET_STATUS") ||
            ISRSUFUNCTION(cmd, "ATT_SIMLOCK_GETKEY") || ISRSUFUNCTION(cmd, "ATT_SIMLOCK_SET") ||
            ISRSUFUNCTION(cmd, "ATT_SIMLOCK_GET_VERSION") ||
            ISRSUFUNCTION(cmd, "ATT_SIMLOCK_RESET") ||
            ISRSUFUNCTION(cmd, "ATT_SIMLOCK_GET_STATUS") ||
            ISRSUFUNCTION(cmd, "ATT_SIMLOCK_GET_CONFIG") ||
            ISRSUFUNCTION(cmd, "ATT_SIMLOCK_GET_API_VER") ||
            ISRSUFUNCTION(cmd, "ATT_SIMLOCK_GET_SLOTS")) {
            if (mRsuFd >= 0) {
                close(mRsuFd);
            }
            mRsuFd = commandFd;
            commandFd = -1;
            executeRemoteSimUnlockCommand(slotId, cmd, data);
            delete line;
            return SUCCESS;
        } else {
            RilOemClient::handleSpecialRequestWithArgs(argCount, args);
            delete line;
        }
    }
    RFX_LOG_E(RFX_LOG_TAG, "Invalid request");
    return FAILURE;
}

void RilOpOemClient::executeRemoteSimUnlockCommand(int slotId, char* command, char* data) {
    RFX_LOG_I(RFX_LOG_TAG, "executeRSUCommand cmd: %s", command);
    char* cmd;
    int err = 0;
    Parcel p;
    char* slb = NULL;

    RfxRequestInfo* pRI = (RfxRequestInfo*)calloc(1, sizeof(RfxRequestInfo));
    RFX_ASSERT(pRI != NULL);
    pRI->socket_id = (RIL_SOCKET_ID)slotId;
    pRI->token = 0xffffffff;
    pRI->clientId = (ClientId)CLIENT_ID_OEM;

    if (ISRSUFUNCTION(command, "SIMMELOCK_GETKEY")) {
        pRI->request = RFX_MSG_REQUEST_GET_SHARED_KEY;
    } else if (ISRSUFUNCTION(command, "SIMMELOCK_SET")) {
        pRI->request = RFX_MSG_REQUEST_UPDATE_SIM_LOCK_SETTINGS;
        slb = data;
    } else if (ISRSUFUNCTION(command, "SIMMELOCK_GET_VERSION")) {
        pRI->request = RFX_MSG_REQUEST_GET_SIM_LOCK_INFO;
    } else if (ISRSUFUNCTION(command, "SIMMELOCK_RESET")) {
        pRI->request = RFX_MSG_REQUEST_RESET_SIM_LOCK_SETTINGS;
    } else if (ISRSUFUNCTION(command, "SIMMELOCK_GET_STATUS")) {
        pRI->request = RFX_MSG_REQUEST_GET_MODEM_STATUS;
    } else if (ISRSUFUNCTION(command, "ATT_SIMLOCK_GETKEY")) {
        pRI->request = RFX_MSG_REQUEST_ATT_GET_SHARED_KEY;
    } else if (ISRSUFUNCTION(command, "ATT_SIMLOCK_SET")) {
        pRI->request = RFX_MSG_REQUEST_ATT_UPDATE_SIM_LOCK_SETTINGS;
        slb = data;
    } else if (ISRSUFUNCTION(command, "ATT_SIMLOCK_GET_VERSION")) {
        pRI->request = RFX_MSG_REQUEST_ATT_GET_SIM_LOCK_VERSION;
    } else if (ISRSUFUNCTION(command, "ATT_SIMLOCK_RESET")) {
        pRI->request = RFX_MSG_REQUEST_ATT_RESET_SIM_LOCK_SETTINGS;
    } else if (ISRSUFUNCTION(command, "ATT_SIMLOCK_GET_STATUS")) {
        pRI->request = RFX_MSG_REQUEST_ATT_GET_MODEM_STATUS;
    } else if (ISRSUFUNCTION(command, "ATT_SIMLOCK_GET_CONFIG")) {
        pRI->request = RFX_MSG_REQUEST_ATT_GET_MODEM_CONFIG;
    } else if (ISRSUFUNCTION(command, "ATT_SIMLOCK_GET_API_VER")) {
        pRI->request = RFX_MSG_REQUEST_ATT_GET_API_VERSION;
    } else if (ISRSUFUNCTION(command, "ATT_SIMLOCK_GET_SLOTS")) {
        pRI->request = RFX_MSG_REQUEST_ATT_GET_SIM_SLOTS;
    } else {
        RFX_LOG_E(RFX_LOG_TAG, "[RSU-SIMLOCK] executeRSUCommand, not RSU cmd: %s, %d", command,
                  pRI->request);
    }
    rfx_enqueue_request_message_client(pRI->request, slb, (slb == NULL) ? 0 : strlen(slb), pRI,
                                       (RIL_SOCKET_ID)slotId);
}

void RilOpOemClient::requestComplete(RIL_Token token, RIL_Errno e, void* response,
                                     size_t responselen) {
    RFX_UNUSED(responselen);
    RfxRequestInfo* info = (RfxRequestInfo*)token;
    // op logic
    if (ISRSUMESSAGE(info->request)) {
        char* strResult = NULL;
        RFX_LOG_I(RFX_LOG_TAG, "[RSU-SIMLOCK] request for RSU returned");
        if (RIL_E_SUCCESS == e) {
            asprintf(&strResult, "%s", (char*)response);
        } else {
            asprintf(&strResult, "ERROR:%d", e);
        }

        if (mRsuFd >= 0) {
            RFX_LOG_I(RFX_LOG_TAG, "[RSU-SIMLOCK] mRsuFd is valid strResult is %s", strResult);
            int len = (int)strlen(strResult);
            int ret;
            ret = send(mRsuFd, &len, sizeof(int), MSG_NOSIGNAL);
            ret = send(mRsuFd, strResult, len, MSG_NOSIGNAL);
            RFX_LOG_I(RFX_LOG_TAG, "[RSU-SIMLOCK] e %d, len %d, ret=%d.", e, len, ret);
            if (ret != (ssize_t)len) {
                RFX_LOG_E(RFX_LOG_TAG, "[RSU-SIMLOCK] lose data when send response.");
            }
            close(mRsuFd);
            mRsuFd = -1;
        } else {
            RFX_LOG_E(RFX_LOG_TAG, "[RSU-SIMLOCK] mRsuFd is < 0");
        }
        free(strResult);

        setClientState(CLIENT_DEACTIVE); /* or should set to init state*/
        free(info);
    } else {
        RilOemClient::requestComplete(token, e, response, responselen);
    }
}
