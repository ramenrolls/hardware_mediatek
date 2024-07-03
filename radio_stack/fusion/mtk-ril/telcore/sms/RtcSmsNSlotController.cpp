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
#include "RtcSmsNSlotController.h"
#include <stdlib.h>
#include "RfxLog.h"
#include "telephony/mtk_ril.h"
#include "RfxRilUtils.h"

/*****************************************************************************
 * Defines
 *****************************************************************************/
#define EVENT_SMS_DISPATCH_SUPL_SMS 1

/*****************************************************************************
 * Class RfxController
 *****************************************************************************/
RFX_IMPLEMENT_CLASS("RtcSmsNSlotController", RtcSmsNSlotController, RfxController);

RtcSmsNSlotController::RtcSmsNSlotController() : m_smsThread(NULL), mDispatcherList(NULL) {}

RtcSmsNSlotController::~RtcSmsNSlotController() {}

void RtcSmsNSlotController::onInit() {
    RfxController::onInit();
    m_smsThread = RtcSmsThread::create();
    m_smsThread->run("RtcSmsThread");

    mDispatcherList = new Vector<SuplMsgDispatcher*>();

    for (int i = 0; i < RfxRilUtils::rfxGetSimCount(); i++) {
        SuplMsgDispatcher* messageDispatcher;
        RFX_OBJ_CREATE(messageDispatcher, SuplMsgDispatcher, this);
        mDispatcherList->add(messageDispatcher);
    }
}

void RtcSmsNSlotController::onDeinit() {
    m_smsThread->requestExit();
    if (m_smsThread->getLooper() != NULL) {
        m_smsThread->getLooper()->wake();
    }
    m_smsThread = NULL;

    if (mDispatcherList != NULL) {
        for (SuplMsgDispatcher* item : *mDispatcherList) {
            RFX_OBJ_CLOSE(item);
        }
        mDispatcherList->clear();
        delete mDispatcherList;
    }
    mDispatcherList = NULL;

    RfxController::onDeinit();
}

void RtcSmsNSlotController::onHandleSmsMessage(const sp<RtcSmsParsingMessage>& message) {
    logD(SMS_TAG, "[RtcSmsNSlotController]onHandleSmsMessage %d", message->getId());
    switch (message->getId()) {
        case EVENT_SMS_DISPATCH_SUPL_SMS: {
            Parcel* p = message->getParcel();
            p->setDataPosition(0);
            int slotId = p->readInt32();
            const char* data = p->readCString();
            string str = data;
            SuplMsgDispatcher* messageDispatcher = mDispatcherList->itemAt(slotId);
            messageDispatcher->dispatchSuplMsg(str);
            break;
        }
        default: {
            RFX_LOG_E(SMS_TAG, "[RtcSmsNSlotController]onHandleSmsMessage unknown!");
            break;
        }
    }
}

sp<RtcSmsThread> RtcSmsNSlotController::getSmsThread() { return m_smsThread; }

void RtcSmsNSlotController::dispatchSms(const sp<RfxMessage>& msg) {
    if (mDispatcherList == NULL) {
        return;
    } else {
        SuplMsgDispatcher* messageDispatcher = mDispatcherList->itemAt(0);
        if (messageDispatcher == NULL || !messageDispatcher->doesSuplExist()) {
            return;
        }
    }

    char* data = (char*)msg->getData()->getData();
    int slotId = msg->getSlotId();
    // logD(SMS_TAG, "dispatchSms: %s, slotId: %d", data, slotId);
    if (data == NULL) {
        logD(SMS_TAG, "handleNewSms data is null.");
        return;
    }
    int length = strlen(data);
    char* pdu = (char*)malloc(length + 1);
    RFX_ASSERT(pdu != NULL);
    strncpy(pdu, data, length);
    pdu[length] = '\0';

    Parcel* p = new Parcel();
    p->writeInt32(slotId);
    p->writeCString(pdu);
    sp<RtcSmsParsingMessage> smsMsg =
            RtcSmsParsingMessage::obtainMessage(EVENT_SMS_DISPATCH_SUPL_SMS, p);
    sp<RtcSmsParsingThreadHandler> handler = new RtcSmsParsingThreadHandler(smsMsg);
    if (m_smsThread->getLooper() == NULL) {
        logD(SMS_TAG, "[RtcSmsNSlotController]sendSmsSmsMessage: ignore message");
        free(pdu);
        return;
    }
    handler->sendMessage(m_smsThread->getLooper());
    free(pdu);
}
