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
#include "RmcData.h"

/*****************************************************************************
 * Class RmcMultiAtReq
 *****************************************************************************/
RmcAtSendInfo* RmcMultiAtReq::getNextAt(RmcAtRspInfo* cur, RfxBaseHandler* h) {
    RfxAtLine* intermediate = cur->getResponse()->getIntermediates();
    RfxAtLine* finalResponse = cur->getResponse()->getFinalResponse();
    int err;
    bool stop = false;
    if (cur->getResponse()->isATCmdRspErr()) {
        setError(RIL_E_SYSTEM_ERR);
        if (finalResponse != NULL) {
            finalResponse->atTokStart(&err);
            onHandleFinalResponseForError(cur->getCmd(), finalResponse, h);
        }
    } else {
        while (intermediate) {
            intermediate->atTokStart(&err);
            if (onHandleIntermediates(cur->getCmd(), intermediate, h)) {
                intermediate = intermediate->getNext();
            } else {
                stop = true;
                break;
            }
        }
        if (finalResponse != NULL) {
            finalResponse->atTokStart(&err);
            onHandleFinalResponse(cur->getCmd(), finalResponse, h);
        }
        if ((getError() == RIL_E_SUCCESS) && (!stop)) {
            return onGetNextAtInfo(cur->getCmd(), h);
        }
    }
    return NULL;
}

/*****************************************************************************
 * Class RmcSingleAtReq
 *****************************************************************************/
RmcAtSendInfo* RmcSingleAtReq::onGetFirstAtInfo(RfxBaseHandler* h) { return onGetAtInfo(h); }

RmcAtSendInfo* RmcSingleAtReq::onGetNextAtInfo(const String8& curCmd, RfxBaseHandler* h) {
    RFX_UNUSED(curCmd);
    RFX_UNUSED(h);
    return NULL;
};

void RmcSingleAtReq::onHandleFinalResponse(const String8& cmd, RfxAtLine* line, RfxBaseHandler* h) {
    RFX_UNUSED(cmd);
    onHandleFinalResponse(line, h);
}

void RmcSingleAtReq::onHandleFinalResponseForError(const String8& cmd, RfxAtLine* line,
                                                   RfxBaseHandler* h) {
    RFX_UNUSED(cmd);
    onHandleFinalResponseForError(line, h);
}

bool RmcSingleAtReq::onHandleIntermediates(const String8& cmd, RfxAtLine* line, RfxBaseHandler* h) {
    RFX_UNUSED(cmd);
    return onHandleIntermediates(line, h);
}

/*****************************************************************************
 * Class RmcBaseRspData
 *****************************************************************************/
sp<RfxMclMessage> RmcBaseRspData::toMessage(const sp<RfxMclMessage>& msg) {
    sp<RfxMclMessage> response =
            RfxMclMessage::obtainResponse(msg->getId(), getError(), getData(), msg);
    return response;
}

/*****************************************************************************
 * Class RmcVoidRsp
 *****************************************************************************/
RFX_IMPLEMENT_DATA_CLASS(RmcVoidRsp);
RmcVoidRsp::RmcVoidRsp(void* data, int length)
    : RmcBaseRspData(data, length), m_error(RIL_E_SUCCESS) {
    m_data = NULL;
    m_length = 0;
}

RmcVoidRsp::~RmcVoidRsp() {}

/*****************************************************************************
 * Class RmcBaseUrspData
 *****************************************************************************/
sp<RfxMclMessage> RmcBaseUrspData::toMessage() {
    sp<RfxMclMessage> urc = RfxMclMessage::obtainUrc(getUrcId(), getSlotId(), getData());
    return urc;
}

/*****************************************************************************
 * Class RmcVoidUrsp
 *****************************************************************************/
RFX_IMPLEMENT_DATA_CLASS(RmcVoidUrsp);

RmcVoidUrsp::RmcVoidUrsp(void* data, int length)
    : RmcBaseUrspData(data, length), m_urc(0), m_slotId(-1) {
    m_data = NULL;
    m_length = 0;
}

RmcVoidUrsp::~RmcVoidUrsp() {}

/*****************************************************************************
 * Class RmcStringUrsp
 *****************************************************************************/
RFX_IMPLEMENT_DATA_CLASS(RmcStringUrsp);

RmcStringUrsp::RmcStringUrsp(void* data, int length) : RmcVoidUrsp(data, length) {
    m_string.setTo((const char*)data, length);
    m_data = (void*)m_string.string();
    m_length = length;
}

RmcStringUrsp::~RmcStringUrsp() {}
