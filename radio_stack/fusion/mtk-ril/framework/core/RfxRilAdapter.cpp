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

#include <sys/types.h>
#include <stdlib.h>
#include "RfxRilAdapter.h"
#include "RfxRilUtils.h"
#include "RfxMclMessage.h"
#include "RfxMclDispatcherThread.h"
#include "RfxDispatchThread.h"
#include "ril_callbacks.h"
#include "RfxIdToMsgIdUtils.h"
#include "RfxDefs.h"
#include "RfxTimer.h"
#include "client/RtcRilClientController.h"

/*************************************************************
 * RfxRilAdapter Class Implementation
 *************************************************************/
RFX_IMPLEMENT_CLASS("RfxRilAdapter", RfxRilAdapter, RfxObject);
RFX_OBJ_IMPLEMENT_SINGLETON_CLASS(RfxRilAdapter);

#define RFX_LOG_TAG "RfxRilAdapter"

RfxRilAdapter::RfxRilAdapter() {
    // TODO Auto-generated constructor stub
}

RfxRilAdapter::~RfxRilAdapter() {}

void RfxRilAdapter::requestAckToRilj(const sp<RfxMessage>& message) {
    // arg check
    if (message == NULL || message.get() == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "requestAckToRilj arg check fail");
        return;
    }

    RFX_MESSAGE_TYPE type = message->getType();

    if (message->getClientId() != -1) {
        RFX_LOG_D(RFX_LOG_TAG, "requestAckToRilj not support message with client Id: %d",
                  message->getClientId());
        return;
    }

    RFX_onRequestAck(message->getRilToken());
    RFX_LOG_D(RFX_LOG_TAG, "requestAckToRilj send done (pId=%d, pToken=%d, id=%d, token=%d)",
              message->getPId(), message->getPToken(), message->getId(), message->getToken());
}

bool RfxRilAdapter::responseToRilj(const sp<RfxMessage>& message) {
    void* data = message->getData() == NULL ? NULL : message->getData()->getData();
    int dataLength = message->getData() == NULL ? 0 : message->getData()->getDataLength();

    // replace msg id to id at RfxMessage
    int id = RfxIdToMsgIdUtils::msgIdToId(message->getPId());
    if (INVALID_ID != id) {
        // Keep correct id of RfxMessage
        message->setPId(id);
    }

    if (message->getSlotId() < 0 || message->getSlotId() >= RfxRilUtils::rfxGetSimCount()) {
        RFX_LOG_E(RFX_LOG_TAG, "Wrong slot id, return");
        return false;
    }

    if (RESPONSE == message->getType()) {
        if (message->getClientId() == -1) {
            RFX_onRequestComplete(message->getRilToken(), message->getError(), data, dataLength);
            RFX_LOG_D(RFX_LOG_TAG, "responseToRilj, request id = %d", message->getPId());
        } else {
            RtcRilClientController::onClientRequestComplete(message->getRilToken(),
                                                            message->getError(), data, dataLength,
                                                            message->getClientId());
        }
    } else if (URC == message->getType()) {
        UrcDispatchRule rule = message->getUrcDispatchRule();
        if (URC_DISPATCH_TO_IMSRIL != rule) {
#if defined(ANDROID_MULTI_SIM)
            RFX_onUnsolicitedResponse(message->getPId(), data, dataLength,
                                      (RIL_SOCKET_ID)message->getSlotId());
#else
            RFX_onUnsolicitedResponse(message->getPId(), data, dataLength);
#endif
        }
        RtcRilClientController::onClientUnsolicitedResponse(message->getSlotId(), message->getPId(),
                                                            data, dataLength, rule);
        RFX_LOG_D(RFX_LOG_TAG, "responseToRilj, urc id = %d", message->getPId());
    }

    return true;
}

bool RfxRilAdapter::responseToBT(const sp<RfxMessage>& message) {
    void* data = message->getData() == NULL ? NULL : message->getData()->getData();
    int dataLength = message->getData() == NULL ? 0 : message->getData()->getDataLength();

    // replace msg id to id at RfxMessage
    int id = RfxIdToMsgIdUtils::sapMsgIdToId(message->getPId());
    if (INVALID_ID != id) {
        // Keep correct id of RfxMessage
        message->setPId(id);
    }

    if (SAP_RESPONSE == message->getType()) {
        RFX_SAP_onRequestComplete(message->getRilToken(), message->getError(), data, dataLength);
        RFX_LOG_D(RFX_LOG_TAG, "responseToBT, request id = %d", message->getPId());
    } else if (SAP_URC == message->getType()) {
#if defined(ANDROID_MULTI_SIM)
        RFX_SAP_onUnsolicitedResponse(message->getPId(), data, dataLength, message->getSlotId());
#else
        RFX_SAP_onUnsolicitedResponse(message->getPId(), data, dataLength);
#endif
        RFX_LOG_D(RFX_LOG_TAG, "responseToBT, urc id = %d", message->getPId());
    }
    return true;
}

void RfxRilAdapter::requestToMcl(const sp<RfxMessage>& message) {
    sp<RfxMclMessage> mclMessage;
    if (REQUEST == message->getType()) {
        mclMessage = RfxMclMessage::obtainRequest(
                message->getId(), message->getData(), message->getSlotId(), message->getToken(),
                message->getSendToMainProtocol(), message->getRilToken(), message->getPTimeStamp(),
                message->getAddAtFront());
        mclMessage->setMainProtocolSlotId(message->getMainProtocolSlotId());
        // add to pending list
        RfxDispatchThread::addMessageToPendingQueue(message);
    } else if (STATUS_SYNC == message->getType()) {
        mclMessage = RfxMclMessage::obtainStatusSync(
                message->getSlotId(), message->getStatusKey(), message->getStatusValue(),
                message->getForceNotify(), message->getIsDefault(), message->getIsUpdateForMock());
    } else if (SAP_REQUEST == message->getType()) {
        mclMessage = RfxMclMessage::obtainSapRequest(message->getId(), message->getData(),
                                                     message->getSlotId(), message->getToken());
        // add to pending list
        RfxDispatchThread::addMessageToPendingQueue(message);
    }
    // Copy request priority value.
    if (mclMessage != NULL && mclMessage.get() != NULL) {
        mclMessage->setPriority(message->getPriority());
    } else {
        RFX_LOG_E(RFX_LOG_TAG, "requestToMcl failed, mclMessage is null.");
        return;
    }

    RfxMclDispatcherThread::waitLooper();
    if (message->getAddAtFront()) {
        RfxMclDispatcherThread::enqueueMclMessageFront(mclMessage);
    } else {
        RfxMclDispatcherThread::enqueueMclMessage(mclMessage);
    }
    if (RfxRilUtils::isEngLoad() && !RfxRilUtils::isInLogReductionList(message->getId())) {
        RFX_LOG_D(RFX_LOG_TAG, "requestToMcl, id = %d, slotId = %d, token = %d", message->getId(),
                  message->getSlotId(), message->getToken());
    } else {
        RFX_LOG_D(RFX_LOG_TAG, "requestToMcl, id = %d, slotId = %d, token = %d", message->getId(),
                  message->getSlotId(), message->getToken());
    }
}

void RfxRilAdapter::requestToMclWithDelay(const sp<RfxMessage>& message, nsecs_t nsec) {
    sp<RfxMclMessage> mclMessage;
    if (STATUS_SYNC == message->getType()) {
        mclMessage = RfxMclMessage::obtainStatusSync(
                message->getSlotId(), message->getStatusKey(), message->getStatusValue(),
                message->getForceNotify(), message->getIsDefault(), message->getIsUpdateForMock());
    } else if (SAP_REQUEST == message->getType()) {
        mclMessage = RfxMclMessage::obtainSapRequest(message->getId(), message->getData(),
                                                     message->getSlotId(), message->getToken());
        // add to pending list
        RfxDispatchThread::addMessageToPendingQueue(message);
    } else {
        // REQUEST
        mclMessage = RfxMclMessage::obtainRequest(
                message->getId(), message->getData(), message->getSlotId(), message->getToken(),
                message->getSendToMainProtocol(), message->getRilToken(), nsec,
                message->getPTimeStamp(), message->getAddAtFront());
        mclMessage->setMainProtocolSlotId(message->getMainProtocolSlotId());
        // add to pending list
        RfxDispatchThread::addMessageToPendingQueue(message);
    }

    RfxMclDispatcherThread::waitLooper();
    RfxMclDispatcherThread::enqueueMclMessageDelay(mclMessage);
    if (RfxRilUtils::isEngLoad() && !RfxRilUtils::isInLogReductionList(message->getId())) {
        RFX_LOG_D(RFX_LOG_TAG, "requestToMcl, message = %s", message->toString().string());
    } else {
        RFX_LOG_D(RFX_LOG_TAG, "requestToMcl, message = %s", message->toString().string());
    }
}
