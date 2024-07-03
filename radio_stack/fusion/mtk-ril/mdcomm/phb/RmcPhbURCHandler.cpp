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

#include "RmcPhbURCHandler.h"
#include <telephony/mtk_ril.h>
#include "RfxMessageId.h"
#include <libmtkrilutils.h>

#define TRUE 1
#define FALSE 0
#define RFX_LOG_TAG "RmcPhbUrc"

using ::android::String8;

RFX_IMPLEMENT_HANDLER_CLASS(RmcPhbURCHandler, RIL_CMD_PROXY_URC);
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_URC_PHB_READY_NOTIFICATION);

RmcPhbURCHandler::RmcPhbURCHandler(int slot_id, int channel_id)
    : RfxBaseHandler(slot_id, channel_id) {
    const char* urc[] = {"+EIND: 2", "+EIND: 32"};
    registerToHandleURC(urc, sizeof(urc) / sizeof(char*));
    mLock = PTHREAD_MUTEX_INITIALIZER;
    mPLock = &mLock;
}

RmcPhbURCHandler::~RmcPhbURCHandler() {}

void RmcPhbURCHandler::onHandleUrc(const sp<RfxMclMessage>& msg) {
    char* urc = (char*)msg->getRawUrc()->getLine();
    if (strStartsWith(urc, "+EIND: 2")) {
        onPhbStateChanged(TRUE);
    } else if (strStartsWith(urc, "+EIND: 32")) {
        onPhbStateChanged(FALSE);
    }
}

bool RmcPhbURCHandler::onCheckIfRejectMessage(const sp<RfxMclMessage>& msg,
                                              RIL_RadioState radioState) {
    bool reject = false;
    RFX_UNUSED(msg);
    logD(RFX_LOG_TAG, "onCheckIfRejectMessage: %d %d", radioState, reject);
    return reject;
}

void RmcPhbURCHandler::onPhbStateChanged(int isPhbReady) {
    bool isModemResetStarted =
            getNonSlotMclStatusManager()->getBoolValue(RFX_STATUS_KEY_MODEM_POWER_OFF, false);
    int isSimInserted = getMclStatusManager()->getIntValue(RFX_STATUS_KEY_CARD_TYPE, FALSE);

    logI(RFX_LOG_TAG, "onPhbStateChanged slot=%d, isPhbReady=%d", m_slot_id, isPhbReady);

    if (RFX_SLOT_COUNT >= 2) {
        logI(RFX_LOG_TAG, "onPhbStateChanged isSimInserted=%d, isModemResetStarted=%d",
             isSimInserted, isModemResetStarted);

        if (isSimInserted == FALSE) {
            return;
        }

        if ((isPhbReady == TRUE) && (isModemResetStarted)) {
            return;
        }
    }

    if (isPhbReady == TRUE) {
        setMSimPropertyThreadSafe(m_slot_id, (char*)PROPERTY_RIL_PHB_READY, (char*)"true", mPLock);
    } else {
        sendEvent(RFX_MSG_EVENT_PHB_CURRENT_STORAGE_RESET, RfxVoidData(), RIL_CMD_PROXY_1,
                  m_slot_id);
        setMSimPropertyThreadSafe(m_slot_id, (char*)PROPERTY_RIL_PHB_READY, (char*)"false", mPLock);
    }

    // response to TeleCore
    sp<RfxMclMessage> urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_PHB_READY_NOTIFICATION, m_slot_id,
                                                     RfxIntsData((void*)&isPhbReady, sizeof(int)));
    responseToTelCore(urc);
}
