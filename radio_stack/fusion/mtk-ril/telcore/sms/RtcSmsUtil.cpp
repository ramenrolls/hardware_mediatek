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
#include "RfxRootController.h"
#include "RtcSmsUtil.h"
#include "RtcSmsNSlotController.h"

/*****************************************************************************
 * Class RtcSmsParsingMessage
 *****************************************************************************/
RtcSmsParsingMessage::~RtcSmsParsingMessage() {
    RFX_LOG_V(SMS_TAG, "[RtcSmsParsingMessage]destroyMessage %d, 0x%zu", m_id, (size_t)m_parcel);
    if (m_parcel != NULL) {
        delete (m_parcel);
        m_parcel = NULL;
    }
}

sp<RtcSmsParsingMessage> RtcSmsParsingMessage::obtainMessage(int32_t id, Parcel* parcel) {
    sp<RtcSmsParsingMessage> new_msg = new RtcSmsParsingMessage();
    RFX_LOG_V(SMS_TAG, "[RtcSmsParsingMessage]obtainMessage %d, 0x%zu", id, (size_t)parcel);
    new_msg->m_id = id;
    new_msg->m_parcel = parcel;
    return new_msg;
};

/*****************************************************************************
 * Class RtcSmsHandler
 *****************************************************************************/
void RtcSmsHandler::sendMessage(sp<Looper> looper) { looper->sendMessage(this, m_dummyMsg); }

/*****************************************************************************
 * Class RtcSmsParsingThreadHandler
 *****************************************************************************/
void RtcSmsParsingThreadHandler::handleMessage(const Message& message) {
    RFX_UNUSED(message);
    RfxRootController* root = RFX_OBJ_GET_INSTANCE(RfxRootController);
    RtcSmsNSlotController* ctrl =
            (RtcSmsNSlotController*)root->findController(RFX_OBJ_CLASS_INFO(RtcSmsNSlotController));
    if (ctrl != NULL) {
        ctrl->onHandleSmsMessage(m_msg);
    }
}
