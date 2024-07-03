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

#include "RmcGwsdRequestHandler.h"
#include "RfxGwsdUtils.h"

static const int requests[] = {
        RFX_MSG_REQUEST_SET_GWSD_MODE,
        RFX_MSG_REQUEST_SET_GWSD_CALL_VALID,
        RFX_MSG_REQUEST_SET_GWSD_IGNORE_CALL_INTERVAL,
        RFX_MSG_REQUEST_SET_GWSD_KEEP_ALIVE_PDCP,
        RFX_MSG_REQUEST_SET_GWSD_KEEP_ALIVE_IPDATA,
};

// register data
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringsData, RfxVoidData, RFX_MSG_REQUEST_SET_GWSD_MODE);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_SET_GWSD_CALL_VALID);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData,
                                RFX_MSG_REQUEST_SET_GWSD_IGNORE_CALL_INTERVAL);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringData, RfxVoidData,
                                RFX_MSG_REQUEST_SET_GWSD_KEEP_ALIVE_PDCP);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringData, RfxVoidData,
                                RFX_MSG_REQUEST_SET_GWSD_KEEP_ALIVE_IPDATA);

// register handler to channel
RFX_IMPLEMENT_HANDLER_CLASS(RmcGwsdRequestHandler, RIL_CMD_PROXY_1);

RmcGwsdRequestHandler::RmcGwsdRequestHandler(int slot_id, int channel_id)
    : RfxBaseHandler(slot_id, channel_id) {
    logD(RFX_LOG_TAG, "RmcGwsdRequestHandler constructor");
    registerToHandleRequest(requests, sizeof(requests) / sizeof(int));
}

RmcGwsdRequestHandler::~RmcGwsdRequestHandler() {}

void RmcGwsdRequestHandler::onHandleRequest(const sp<RfxMclMessage>& msg) {
    logD(RFX_LOG_TAG, "onHandleRequest: %d", msg->getId());
    int request = msg->getId();
    switch (request) {
        case RFX_MSG_REQUEST_SET_GWSD_MODE:
            requestSetGwsdMode(msg);
            break;
        case RFX_MSG_REQUEST_SET_GWSD_CALL_VALID:
            requestSetCallValidTimer(msg);
            break;
        case RFX_MSG_REQUEST_SET_GWSD_IGNORE_CALL_INTERVAL:
            requestSetIgnoreSameNumberInterval(msg);
            break;
        case RFX_MSG_REQUEST_SET_GWSD_KEEP_ALIVE_PDCP:
            requestSetKeepAliveByPDCPCtrlPDU(msg);
            break;
        case RFX_MSG_REQUEST_SET_GWSD_KEEP_ALIVE_IPDATA:
            requestSetKeepAliveByIpData(msg);
            break;
        default:
            logE(RFX_LOG_TAG, "Should not be here");
            break;
    }
}

void RmcGwsdRequestHandler::requestSetGwsdMode(const sp<RfxMclMessage>& msg) {
    RIL_Errno ril_errno = RIL_E_MODE_NOT_SUPPORTED;
    sp<RfxMclMessage> resp;

    char** params = (char**)msg->getData()->getData();
    logD(RFX_LOG_TAG, "requestSetGwsdMode, mode:%s,kaMode:%s,kaCycle:%s", params[0], params[1],
         params[2]);

    if (RfxGwsdUtils::getSettingHandler() == NULL) {
        logD(RFX_LOG_TAG, "not support gwsd");
        goto error;
    }

    ril_errno = RfxGwsdUtils::getSettingHandler()->requestSetGwsdMode(this, atoi(params[0]),
                                                                      params[1], params[2]);

error:
    resp = RfxMclMessage::obtainResponse(ril_errno, RfxVoidData(), msg);
    responseToTelCore(resp);
}

void RmcGwsdRequestHandler::requestSetCallValidTimer(const sp<RfxMclMessage>& msg) {
    RIL_Errno ril_errno = RIL_E_MODE_NOT_SUPPORTED;
    sp<RfxMclMessage> resp;

    int* timer = (int*)msg->getData()->getData();
    logD(RFX_LOG_TAG, "requestSetCallValidTimer, timer: %d", timer[0]);

    if (RfxGwsdUtils::getSettingHandler() == NULL) {
        logD(RFX_LOG_TAG, "not support gwsd");
        goto error;
    }

    ril_errno = RfxGwsdUtils::getSettingHandler()->requestSetCallValidTimer(this, timer[0]);

error:
    resp = RfxMclMessage::obtainResponse(ril_errno, RfxVoidData(), msg);
    responseToTelCore(resp);
}

void RmcGwsdRequestHandler::requestSetIgnoreSameNumberInterval(const sp<RfxMclMessage>& msg) {
    RIL_Errno ril_errno = RIL_E_MODE_NOT_SUPPORTED;
    sp<RfxMclMessage> resp;

    int* interval = (int*)msg->getData()->getData();
    logD(RFX_LOG_TAG, "requestSetIgnoreSameNumberInterval, interval: %d", interval[0]);

    if (RfxGwsdUtils::getSettingHandler() == NULL) {
        logD(RFX_LOG_TAG, "not support gwsd");
        goto error;
    }

    ril_errno = RfxGwsdUtils::getSettingHandler()->requestSetIgnoreSameNumberInterval(this,
                                                                                      interval[0]);

error:
    resp = RfxMclMessage::obtainResponse(ril_errno, RfxVoidData(), msg);
    responseToTelCore(resp);
}

void RmcGwsdRequestHandler::requestSetKeepAliveByPDCPCtrlPDU(const sp<RfxMclMessage>& msg) {
    RIL_Errno ril_errno = RIL_E_MODE_NOT_SUPPORTED;
    sp<RfxMclMessage> resp;

    char* pData = (char*)(msg->getData()->getData());
    logD(RFX_LOG_TAG, "%s, KeepAlive: %s", __FUNCTION__, pData);

    if (RfxGwsdUtils::getSettingHandler() == NULL) {
        logD(RFX_LOG_TAG, "not support gwsd");
        goto error;
    }

    ril_errno = RfxGwsdUtils::getSettingHandler()->requestSetKeepAliveByPDCPCtrlPDU(this, pData);

error:
    resp = RfxMclMessage::obtainResponse(ril_errno, RfxVoidData(), msg);
    responseToTelCore(resp);
}

void RmcGwsdRequestHandler::requestSetKeepAliveByIpData(const sp<RfxMclMessage>& msg) {
    RIL_Errno ril_errno = RIL_E_MODE_NOT_SUPPORTED;
    sp<RfxMclMessage> resp;

    char* pData = (char*)(msg->getData()->getData());
    logD(RFX_LOG_TAG, "%s, KeepAlive: %s", __FUNCTION__, pData);

    if (RfxGwsdUtils::getSettingHandler() == NULL) {
        logD(RFX_LOG_TAG, "not support gwsd");
        goto error;
    }

    ril_errno = RfxGwsdUtils::getSettingHandler()->requestSetKeepAliveByIpData(this, pData);

error:
    resp = RfxMclMessage::obtainResponse(ril_errno, RfxVoidData(), msg);
    responseToTelCore(resp);
}
