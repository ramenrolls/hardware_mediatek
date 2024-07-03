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
#include "RmcCdmaMtSms.h"
#include "RmcCdmaSmsConverter.h"
#include "RmcCdmaSmsAck.h"
#include "RfxStringData.h"

/*****************************************************************************
 * Register Data Class
 *****************************************************************************/

RFX_REGISTER_DATA_TO_URC_ID(RmcCdmaMtSmsMessage, RFX_MSG_URC_CDMA_NEW_SMS);
RFX_REGISTER_DATA_TO_URC_ID(RmcCdmaMtSmsMessage, RFX_MSG_URC_CDMA_NEW_SMS_EX);

/*****************************************************************************
 * Class RmcCdmaMtSmsMessage
 *****************************************************************************/
RFX_IMPLEMENT_DATA_CLASS(RmcCdmaMtSmsMessage);
RmcCdmaMtSmsMessage::RmcCdmaMtSmsMessage(void* data, int length)
    : RmcVoidUrsp(data, length), m_replySeqNo(-1) {
    RFX_ASSERT(data != NULL);
    RFX_ASSERT(length == sizeof(m_msg));
    m_msg = *((RIL_CDMA_SMS_Message*)data);
    m_data = (void*)&m_msg;
    m_length = length;
}

RmcCdmaMtSmsMessage::RmcCdmaMtSmsMessage(int urcId, int slotId, String8& hexPdu)
    : RmcVoidUrsp(urcId, slotId), m_replySeqNo(-1) {
    RmcCdmaSmsConverter::toMessage(&m_msg, hexPdu, &m_replySeqNo);
    m_data = (void*)&m_msg;
    m_length = sizeof(m_msg);
}

RmcCdmaMtSmsMessage::~RmcCdmaMtSmsMessage() {}

/*****************************************************************************
 * Class RmcCdmaMtSmsHdlr
 *****************************************************************************/
bool RmcCdmaMtSmsHdlr::onHandleRawUrc(RfxAtLine* line) {
    int err;
    m_data = line->atTokNextint(&err);
    if (err < 0) {
        return false;
    }
    if (m_type == CDMA_SMS_INBOUND_VMI) {
        int num = m_data;
        if (num > 99) {
            num = 99;
        }
        num = ((num / 10) << 4) | (num % 10);
        m_hexPdu.appendFormat("000002100302020040080c0003100000010210000B01%02x", num);
    } else {
        const char* pdu = line->atTokNextstr(&err);
        if (err < 0 || pdu == NULL) {
            return false;
        }
        m_hexPdu.setTo(pdu);
    }
    return true;
}

RmcBaseUrspData* RmcCdmaMtSmsHdlr::onGetUrcData(int slotId) {
    RmcCdmaMtSmsMessage* pMtMsg =
            new RmcCdmaMtSmsMessage(RFX_MSG_URC_CDMA_NEW_SMS, slotId, m_hexPdu);
    int mtType = getHandler()->getMclStatusManager()->getIntValue(
            RFX_STATUS_KEY_CDMA_INBOUND_SMS_TYPE, CDMA_SMS_INBOUND_NONE);
    if (mtType != CDMA_SMS_INBOUND_NONE) {
        if (m_type == CDMA_SMS_INBOUND_VMI) {
            getHandler()->getMclStatusManager()->setIntValue(RFX_STATUS_KEY_CDMA_PENDING_VMI,
                                                             m_data);
        } else {
            RIL_CDMA_SMS_Ack ack;
            ack.uErrorClass = (RIL_CDMA_SMS_ErrorClass)2;
            ack.uSMSCauseCode = 33;
            RmcCdmaSmsAck errorAck(m_type, pMtMsg->getReplySeqNo(), ack, pMtMsg->getAddress());
            getHandler()->sendEvent(
                    RFX_MSG_EVENT_CDMA_SMS_NEW_SMS_ERR_ACK,
                    RfxStringData((void*)errorAck.getCmd().string(), errorAck.getCmd().length()),
                    RIL_CMD_PROXY_7, slotId);
        }
        delete pMtMsg;
        return NULL;
    }

    getHandler()->getMclStatusManager()->setIntValue(RFX_STATUS_KEY_CDMA_INBOUND_SMS_TYPE, m_type);
    if (m_type == CDMA_SMS_INBOUND_VMI) {
        getHandler()->getMclStatusManager()->setIntValue(RFX_STATUS_KEY_CDMA_PENDING_VMI, -1);
    } else {
        getHandler()->getMclStatusManager()->setIntValue(RFX_STATUS_KEY_CDMA_SMS_REPLY_SEQ_NO,
                                                         pMtMsg->getReplySeqNo());
        Vector<char> addr;
        addr.appendArray((const char*)pMtMsg->getAddress(), sizeof(RIL_CDMA_SMS_Address));
        getHandler()->getMclStatusManager()->setValue(RFX_STATUS_KEY_CDMA_SMS_ADDR,
                                                      RfxVariant(addr));
    }
    return pMtMsg;
}
