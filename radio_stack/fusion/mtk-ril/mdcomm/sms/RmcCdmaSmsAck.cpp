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
#include "RmcCdmaSmsConverter.h"
#include "RmcCdmaSmsAck.h"
#include "RfxIntsData.h"

/*****************************************************************************
 * Register Data Class
 *****************************************************************************/
RFX_REGISTER_DATA_TO_REQUEST_ID(RmcCdmaSmsAck, RmcVoidRsp, RFX_MSG_REQUEST_CDMA_SMS_ACKNOWLEDGE);

RFX_REGISTER_DATA_TO_REQUEST_ID(RmcCdmaSmsAck, RmcVoidRsp, RFX_MSG_REQUEST_CDMA_SMS_ACKNOWLEDGE_EX);

RFX_REGISTER_DATA_TO_REQUEST_ID(RmcCdmaSmsAck, RmcVoidRsp,
                                RFX_MSG_REQUEST_CDMA_SMS_ACKNOWLEDGE_INTERNAL);

/*****************************************************************************
 * Class RmcCdmaSmsAck
 *****************************************************************************/
RFX_IMPLEMENT_DATA_CLASS(RmcCdmaSmsAck);
RmcCdmaSmsAck::RmcCdmaSmsAck(void* data, int length) : RmcSingleAtReq(data, length) {
    RFX_ASSERT(data != NULL);
    RFX_ASSERT(length == sizeof(m_ack));
    m_ack = *((RIL_CDMA_SMS_Ack*)data);
    m_data = (void*)&m_ack;
    m_length = length;
}

RmcCdmaSmsAck::~RmcCdmaSmsAck() {}

RmcCdmaSmsAck::RmcCdmaSmsAck(int type, int replyReqNo, RIL_CDMA_SMS_Ack ack,
                             const RIL_CDMA_SMS_Address* pAddr)
    : RmcSingleAtReq(NULL, 0), m_ack(ack) {
    setCmd(type, replyReqNo, pAddr);
}

RmcAtSendInfo* RmcCdmaSmsAck::onGetAtInfo(RfxBaseHandler* h) {
    int mtType = h->getMclStatusManager()->getIntValue(RFX_STATUS_KEY_CDMA_INBOUND_SMS_TYPE,
                                                       CDMA_SMS_INBOUND_NONE);

    String8 hexPdu;
    int replySeqNo =
            h->getMclStatusManager()->getIntValue(RFX_STATUS_KEY_CDMA_SMS_REPLY_SEQ_NO, -1);
    if (replySeqNo == -1) {
        setError(RIL_E_NO_SMS_TO_ACK);
        return NULL;
    }
    const RfxVariant& addr = h->getMclStatusManager()->getValue(RFX_STATUS_KEY_CDMA_SMS_ADDR);
    RIL_CDMA_SMS_Address address;
    RFX_ASSERT(sizeof(address) == addr.asCharVector().size());
    memcpy(&address, addr.asCharVector().array(), sizeof(address));
    if (setCmd(mtType, replySeqNo, &address)) {
        return new RmcNoLineAtSendInfo(m_cmd);
    } else {
        setError(RIL_E_NO_SMS_TO_ACK);
        return NULL;
    }
}

bool RmcCdmaSmsAck::setCmd(int type, int replySeqNo, const RIL_CDMA_SMS_Address* pAddr) {
    switch (type) {
        case CDMA_SMS_INBOUND_COMM:
            m_cmd.setTo("AT+EC2KCNMA=");
            break;

        case CDMA_SMS_INBOUND_VMI:

        default:
            return false;
    }

    String8 hexPdu;
    RmcCdmaSmsConverter::toHexPdu(&m_ack, replySeqNo, (RIL_CDMA_SMS_Address*)pAddr, hexPdu);
    m_cmd.appendFormat("%d,\"%s\"", (int)hexPdu.length() / 2, hexPdu.string());
    return true;
}

/*****************************************************************************
 * Class RmcCdmaSmsAckHdlr
 *****************************************************************************/
void RmcCdmaSmsAckHdlr::onAfterResponse() {
    getHandler()->getMclStatusManager()->setIntValue(RFX_STATUS_KEY_CDMA_INBOUND_SMS_TYPE,
                                                     CDMA_SMS_INBOUND_NONE);
    int vmi = getHandler()->getMclStatusManager()->getIntValue(RFX_STATUS_KEY_CDMA_PENDING_VMI, -1);
    if (vmi != -1) {
        int slotId = getHandler()->getMclStatusManager()->getSlotId();
        getHandler()->sendEvent(RFX_MSG_EVENT_CDMA_SMS_PENDING_VMI, RfxIntsData(&vmi, 1),
                                RIL_CMD_PROXY_URC, slotId, -1, -1, 100);
    }
}
