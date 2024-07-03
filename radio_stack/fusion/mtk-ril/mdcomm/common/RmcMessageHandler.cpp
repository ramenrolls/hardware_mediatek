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
#include "RmcMessageHandler.h"

/*****************************************************************************
 * Class RmcBaseRequestHandler
 *****************************************************************************/
RmcBaseRequestHandler::~RmcBaseRequestHandler() {
    Vector<RmcAtRspInfo*>::iterator it;
    for (it = m_atRspInfos.begin(); it != m_atRspInfos.end();) {
        delete (*it);
        it = m_atRspInfos.erase(it);
    }
}

/*****************************************************************************
 * Class RmcBaseRequestHandler
 *****************************************************************************/
void RmcBaseRequestHandler::onHandleMessage(const sp<RfxMclMessage>& msg) {
    RmcBaseReqData* req = (RmcBaseReqData*)msg->getData();
    sendAtCmds(req);
    RmcBaseRspData* rsp = getRspData(req);
    getHandler()->responseToTelCore(rsp->toMessage(msg));
    releaseRspData(rsp);
    onAfterResponse();
}

void RmcBaseRequestHandler::sendAtCmds(RmcBaseReqData* data) {
    RmcAtSendInfo* info = data->getFirstAt(getHandler());
    if (info != NULL) {
        RmcAtRspInfo* rspInfo;
        do {
            sp<RfxAtResponse> rsp = info->sendAt(getHandler());
            rspInfo = new RmcAtRspInfo(info->getCmd(), rsp);
            m_atRspInfos.push(rspInfo);
        } while ((info = data->getNextAt(rspInfo, getHandler())) != NULL);
    }
}

RmcBaseRspData* RmcBaseRequestHandler::getRspData(RmcBaseReqData* data) {
    return onGetRspData(data);
}

RmcBaseRspData* RmcBaseRequestHandler::onGetRspData(RmcBaseReqData* data) {
    return new RmcVoidRsp(data->getError());
}

void RmcBaseRequestHandler::releaseRspData(RmcBaseRspData* data) {
    if (data != NULL) {
        delete data;
    }
}

/*****************************************************************************
 * Class RmcBaseUrcHandler
 *****************************************************************************/
void RmcBaseUrcHandler::onHandleMessage(const sp<RfxMclMessage>& msg) {
    RmcBaseUrspData* uRsp = getUrspData(msg);
    if (uRsp != NULL) {
        getHandler()->responseToTelCore(uRsp->toMessage());
        releaseUrspData(uRsp);
    }
}

RmcBaseUrspData* RmcBaseUrcHandler::getUrspData(const sp<RfxMclMessage>& msg) {
    int err;
    RfxAtLine* urc = msg->getRawUrc();
    urc->atTokStart(&err);
    if (err < 0) {
        return NULL;
    }
    if (!onHandleRawUrc(urc)) {
        return NULL;
    }
    RfxAtLine* urc2 = msg->getRawUrc2();
    if (urc2 != NULL) {
        urc2->atTokStart(&err);
        if (err < 0) {
            return NULL;
        }
        if (!onHandleRawUrc2(urc2)) {
            return NULL;
        }
    }
    return onGetUrcData(msg->getSlotId());
};

void RmcBaseUrcHandler::releaseUrspData(RmcBaseUrspData* data) {
    if (data != NULL) {
        delete data;
    }
}
