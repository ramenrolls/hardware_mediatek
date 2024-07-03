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
#include "RmcCdmaSmsHandler.h"
#include "RmcCdmaMoSms.h"
#include "RmcCdmaMtSms.h"
#include "RmcCdmaSmsAck.h"
#include "RmcRuimSmsWrite.h"
#include "RmcRuimSmsDelete.h"
#include "RmcRuimSmsMem.h"
#include "RmcCdmaSmsMemFull.h"
#include "RmcCdmaEsnMeid.h"
#include "RmcCdmaBcActivate.h"
#include "RmcCdmaBcRangeParser.h"
#include "RmcCdmaBcConfigSet.h"
#include "RmcCdmaBcConfigGet.h"
#include "RfxStringData.h"
#include "RfxIntsData.h"

/*****************************************************************************
 * Const Value
 *****************************************************************************/
const char* memFul = "+ESMFULL";
const char* esnMeid = "+ECARDESNME";
const char* mt = "+EC2KCMT";
const char* vm = "+EVMI";

RFX_REGISTER_DATA_TO_EVENT_ID(RfxStringData, RFX_MSG_EVENT_CDMA_SMS_NEW_SMS_ERR_ACK);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxIntsData, RFX_MSG_EVENT_CDMA_SMS_PENDING_VMI);

/*****************************************************************************
 * Class RmcCdmaSmsReqHandler
 *****************************************************************************/
RFX_IMPLEMENT_HANDLER_CLASS(RmcCdmaSmsReqHandler, RIL_CMD_PROXY_1);

RmcCdmaSmsReqHandler::RmcCdmaSmsReqHandler(int slotId, int channelId)
    : RmcBaseHandler(slotId, channelId) {
    const int request[] = {
            RFX_MSG_REQUEST_CDMA_SMS_BROADCAST_ACTIVATION,
            RFX_MSG_REQUEST_CDMA_GET_BROADCAST_SMS_CONFIG,
            RFX_MSG_REQUEST_CDMA_SET_BROADCAST_SMS_CONFIG,
            RFX_MSG_REQUEST_CDMA_DELETE_SMS_ON_RUIM,
            RFX_MSG_REQUEST_CDMA_WRITE_SMS_TO_RUIM,
            RFX_MSG_REQUEST_GET_SMS_RUIM_MEM_STATUS,
    };
    registerToHandleRequest(request, sizeof(request) / sizeof(int));
}

RmcCdmaSmsReqHandler::~RmcCdmaSmsReqHandler() {}

RmcMessageHandler* RmcCdmaSmsReqHandler::onCreateReqHandler(const sp<RfxMclMessage>& msg) {
    RmcMessageHandler* handler = NULL;
    switch (msg->getId()) {
        case RFX_MSG_REQUEST_CDMA_SMS_BROADCAST_ACTIVATION: {
            handler = new RmcBaseRequestHandler(this);
            break;
        }

        case RFX_MSG_REQUEST_CDMA_DELETE_SMS_ON_RUIM: {
            handler = new RmcBaseRequestHandler(this);
            break;
        }

        case RFX_MSG_REQUEST_CDMA_WRITE_SMS_TO_RUIM: {
            handler = new RmcCdmaWriteRuimSmsHdlr(this);
            break;
        }

        case RFX_MSG_REQUEST_GET_SMS_RUIM_MEM_STATUS: {
            handler = new RmcCdmaGetRuimSmsMemHdlr(this);
            break;
        }

        case RFX_MSG_REQUEST_CDMA_GET_BROADCAST_SMS_CONFIG: {
            handler = new RmcCdmaBcGetConfigHdlr(this);
            break;
        }

        case RFX_MSG_REQUEST_CDMA_SET_BROADCAST_SMS_CONFIG: {
            handler = new RmcBaseRequestHandler(this);
            break;
        }

        default:
            RFX_ASSERT(0);
    }
    return handler;
}

void RmcCdmaSmsReqHandler::onHandleEvent(const sp<RfxMclMessage>& msg) {
    RfxBaseHandler::onHandleEvent(msg);
}

/*****************************************************************************
 * Class RmcCdmaMoSmsReqHandler
 *****************************************************************************/
RFX_IMPLEMENT_HANDLER_CLASS(RmcCdmaMoSmsReqHandler, RIL_CMD_PROXY_8);

RmcCdmaMoSmsReqHandler::RmcCdmaMoSmsReqHandler(int slotId, int channelId)
    : RmcBaseHandler(slotId, channelId) {
    const int request[] = {RFX_MSG_REQUEST_CDMA_SEND_SMS, RFX_MSG_REQUEST_IMS_SEND_CDMA_SMS,
                           RFX_MSG_REQUEST_IMS_SEND_CDMA_SMS_EX};
    registerToHandleRequest(request, sizeof(request) / sizeof(int));
}

RmcCdmaMoSmsReqHandler::~RmcCdmaMoSmsReqHandler() {}

RmcMessageHandler* RmcCdmaMoSmsReqHandler::onCreateReqHandler(const sp<RfxMclMessage>& msg) {
    RmcMessageHandler* handler = NULL;
    int id = msg->getId();
    RFX_ASSERT((id == RFX_MSG_REQUEST_CDMA_SEND_SMS) || (id == RFX_MSG_REQUEST_IMS_SEND_CDMA_SMS) ||
               (id == RFX_MSG_REQUEST_IMS_SEND_CDMA_SMS_EX));
    handler = new RmcCdmaMoSmsHdlr(this);
    return handler;
}

void RmcCdmaMoSmsReqHandler::onHandleEvent(const sp<RfxMclMessage>& msg) {
    RfxBaseHandler::onHandleEvent(msg);
}

/*****************************************************************************
 * Class RmcCdmaMtSmsAckReqHandler
 *****************************************************************************/
RFX_IMPLEMENT_HANDLER_CLASS(RmcCdmaMtSmsAckReqHandler, RIL_CMD_PROXY_7);

RmcCdmaMtSmsAckReqHandler::RmcCdmaMtSmsAckReqHandler(int slotId, int channelId)
    : RmcBaseHandler(slotId, channelId) {
    const int request[] = {RFX_MSG_REQUEST_CDMA_SMS_ACKNOWLEDGE,
                           RFX_MSG_REQUEST_CDMA_SMS_ACKNOWLEDGE_EX,
                           RFX_MSG_REQUEST_CDMA_SMS_ACKNOWLEDGE_INTERNAL};
    registerToHandleRequest(request, sizeof(request) / sizeof(int));
    const int event[] = {RFX_MSG_EVENT_CDMA_SMS_NEW_SMS_ERR_ACK};
    registerToHandleEvent(event, sizeof(event) / sizeof(int));
}

RmcCdmaMtSmsAckReqHandler::~RmcCdmaMtSmsAckReqHandler() {}

RmcMessageHandler* RmcCdmaMtSmsAckReqHandler::onCreateReqHandler(const sp<RfxMclMessage>& msg) {
    RmcMessageHandler* handler = NULL;
    int id = msg->getId();
    RFX_ASSERT(id == RFX_MSG_REQUEST_CDMA_SMS_ACKNOWLEDGE ||
               id == RFX_MSG_REQUEST_CDMA_SMS_ACKNOWLEDGE_EX ||
               id == RFX_MSG_REQUEST_CDMA_SMS_ACKNOWLEDGE_INTERNAL);
    handler = new RmcCdmaSmsAckHdlr(this);
    return handler;
}

void RmcCdmaMtSmsAckReqHandler::onHandleEvent(const sp<RfxMclMessage>& msg) {
    if (msg->getId() == RFX_MSG_EVENT_CDMA_SMS_NEW_SMS_ERR_ACK) {
        String8 at((const char*)msg->getData()->getData(), msg->getData()->getDataLength());
        atSendCommand(at);
        return;
    }
    RfxBaseHandler::onHandleEvent(msg);
}

/*****************************************************************************
 * Class RmcCdmaSmsUrcHandler
 *****************************************************************************/
RFX_IMPLEMENT_HANDLER_CLASS(RmcCdmaSmsUrcHandler, RIL_CMD_PROXY_URC);

RmcCdmaSmsUrcHandler::RmcCdmaSmsUrcHandler(int slotId, int channelId)
    : RmcBaseHandler(slotId, channelId) {
    const char* urc[] = {mt, memFul, esnMeid, vm};
    registerToHandleURC(urc, sizeof(urc) / sizeof(urc[0]));
    const int event[] = {RFX_MSG_EVENT_CDMA_SMS_PENDING_VMI};
    registerToHandleEvent(event, sizeof(event) / sizeof(int));
}

RmcCdmaSmsUrcHandler::~RmcCdmaSmsUrcHandler() {}

RmcMessageHandler* RmcCdmaSmsUrcHandler::onCreateUrcHandler(const sp<RfxMclMessage>& msg) {
    RmcMessageHandler* handler = NULL;

    const char* urc = msg->getRawUrc()->getLine();
    if (strStartsWith(urc, mt)) {
        handler = new RmcCdmaMtSmsHdlr(this, CDMA_SMS_INBOUND_COMM);
    } else if (strStartsWith(urc, vm)) {
        handler = new RmcCdmaMtSmsHdlr(this, CDMA_SMS_INBOUND_VMI);
    } else if (strStartsWith(urc, memFul)) {
        handler = new RmcCdmaSmsMemFullHdlr(this);
    } else if (strStartsWith(urc, esnMeid)) {
        handler = new RmcCdmaCardEsnMeidHdlr(this);
    }
    return handler;
}

void RmcCdmaSmsUrcHandler::onHandleEvent(const sp<RfxMclMessage>& msg) {
    if (msg->getId() == RFX_MSG_EVENT_CDMA_SMS_PENDING_VMI) {
        int vmi = *((int*)msg->getData()->getData());
        String8 urc = String8::format("+EVMI:%d", vmi);
        RfxAtLine* atLine = new RfxAtLine(urc.string(), NULL);
        sp<RfxMclMessage> message =
                RfxMclMessage::obtainRawUrc(msg->getSlotId() * RIL_CHANNEL_OFFSET, atLine, NULL);
        RmcCdmaSmsUrcHandler::onHandleUrc(message);
        return;
    }
    RfxBaseHandler::onHandleEvent(msg);
}
