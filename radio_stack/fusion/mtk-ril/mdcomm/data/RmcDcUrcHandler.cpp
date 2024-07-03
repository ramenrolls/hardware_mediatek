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
#include "RfxStringData.h"
#include "RfxVoidData.h"
#include "RfxMessageId.h"
#include "RmcDcUrcHandler.h"
#include "RmcDcUtility.h"

#define RFX_LOG_TAG "RmcDcUrcHandler"

/*****************************************************************************
 * Class RmcDcUrcHandler
 *****************************************************************************/
RFX_IMPLEMENT_HANDLER_CLASS(RmcDcUrcHandler, RIL_CMD_PROXY_URC);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxStringData, RFX_MSG_EVENT_DATA_NW_PDN_ACT);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxStringData, RFX_MSG_EVENT_DATA_NW_PDN_DEACT);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxStringData, RFX_MSG_EVENT_DATA_NW_MODIFY);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxStringData, RFX_MSG_EVENT_DATA_NW_REACT);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxStringData, RFX_MSG_EVENT_DATA_ME_PDN_ACT);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxStringData, RFX_MSG_EVENT_DATA_ME_PDN_DEACT);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxStringData, RFX_MSG_EVENT_DATA_PDN_CHANGE);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxStringData, RFX_MSG_EVENT_DATA_UT_TEST);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxStringData, RFX_MSG_EVENT_DATA_LCE_STATUS_CHANGED);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxStringData, RFX_MSG_EVENT_LTE_ACCESS_STRATUM_STATE_CHANGE);
/// M: Ims Data Framework {@
RFX_REGISTER_DATA_TO_EVENT_ID(RfxStringData, RFX_MSG_EVENT_DATA_IMS_PDN_NOTIFY);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxStringData, RFX_MSG_EVENT_DATA_PCSCF_ADDRESS_DISCOVERY);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxStringData, RFX_MSG_EVENT_QUALIFIED_NETWORK_TYPES_CHANGED);
/// @}
RFX_REGISTER_DATA_TO_EVENT_ID(RfxVoidData, RFX_MSG_EVENT_URC_HANDLER_REGISTER_DONE);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxStringData, RFX_MSG_EVENT_QUERY_PCO_WITH_URC);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxStringData, RFX_MSG_EVENT_DATA_LINK_CAPACITY_ESTIMATE);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxStringData, RFX_MSG_EVENT_NETWORK_REJECT_CAUSE);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxStringData, RFX_MSG_EVENT_MOBILE_DATA_USAGE_NOTIFY);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxStringData, RFX_MSG_EVENT_KEEPALIVE_STATUS);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxStringData, RFX_MSG_EVENT_DATA_NW_LIMIT_NOTIFY);

RmcDcUrcHandler::RmcDcUrcHandler(int slot_id, int channel_id)
    : RfxBaseHandler(slot_id, channel_id) {
    const char* urcList[] = {
            "+CGEV:",    "+EPDN:",     "+EUTTEST:", "+ELCE:",     "+EDRBSTATE:",
            "+EIMSPDN:", "+EIMSPDIS:", "+EIAREG:",  "+EXLCE:",    "+ECCAUSE:",
            "+EIWLPL:",  "+EGCMD:",    "+EKALIVE:", "+ENWLIMIT:",
    };
    const int eventList[] = {RFX_MSG_EVENT_IMS_REQUEST_HANDLER_REGISTER_DONE};
    registerToHandleURC(urcList, sizeof(urcList) / sizeof(char*));
    registerToHandleEvent(eventList, sizeof(eventList) / sizeof(int));
    notifyUrcHandlerRegistrationDone();
}

RmcDcUrcHandler::~RmcDcUrcHandler() {}

void RmcDcUrcHandler::onHandleUrc(const sp<RfxMclMessage>& msg) {
    char* pRawUrc = msg->getRawUrc()->getLine();

    if (strStartsWith(pRawUrc, "+CGEV: NW REACT")) {
        sendEvent(RFX_MSG_EVENT_DATA_NW_REACT, RfxStringData(pRawUrc), RIL_CMD_PROXY_5, m_slot_id);
    } else if (strStartsWith(pRawUrc, "+CGEV: NW PDN ACT")) {
        sendEvent(RFX_MSG_EVENT_DATA_NW_PDN_ACT, RfxStringData(pRawUrc), RIL_CMD_PROXY_5,
                  m_slot_id);
    } else if (strStartsWith(pRawUrc, "+CGEV: NW PDN DEACT")) {
        sendEvent(RFX_MSG_EVENT_DATA_NW_PDN_DEACT, RfxStringData(pRawUrc), RIL_CMD_PROXY_5,
                  m_slot_id);
    } else if (strStartsWith(pRawUrc, "+CGEV: NW MODIFY")) {
        sendEvent(RFX_MSG_EVENT_DATA_NW_MODIFY, RfxStringData(pRawUrc), RIL_CMD_PROXY_5, m_slot_id);
    } else if (strStartsWith(pRawUrc, "+CGEV: ME PDN ACT")) {
        sendEvent(RFX_MSG_EVENT_DATA_ME_PDN_ACT, RfxStringData(pRawUrc), RIL_CMD_PROXY_5,
                  m_slot_id);
    } else if (strStartsWith(pRawUrc, "+CGEV: ME PDN DEACT")) {
        sendEvent(RFX_MSG_EVENT_DATA_ME_PDN_DEACT, RfxStringData(pRawUrc), RIL_CMD_PROXY_5,
                  m_slot_id);
    } else if (strStartsWith(pRawUrc, "+EPDN:")) {
        sendEvent(RFX_MSG_EVENT_DATA_PDN_CHANGE, RfxStringData(pRawUrc), RIL_CMD_PROXY_5,
                  m_slot_id);
    } else if (strStartsWith(pRawUrc, "+EUTTEST:")) {
        sendEvent(RFX_MSG_EVENT_DATA_UT_TEST, RfxStringData(pRawUrc), RIL_CMD_PROXY_5, m_slot_id);
    } else if (strStartsWith(pRawUrc, "+ELCE:")) {
        sendEvent(RFX_MSG_EVENT_DATA_LCE_STATUS_CHANGED, RfxStringData(pRawUrc), RIL_CMD_PROXY_5,
                  m_slot_id);
    } else if (strStartsWith(pRawUrc, "+EDRBSTATE:")) {
        sendEvent(RFX_MSG_EVENT_LTE_ACCESS_STRATUM_STATE_CHANGE, RfxStringData(pRawUrc),
                  RIL_CMD_PROXY_5, m_slot_id);
    } else if (strStartsWith(pRawUrc, "+EIMSPDN:")) {
        sendEvent(RFX_MSG_EVENT_DATA_IMS_PDN_NOTIFY, RfxStringData(pRawUrc), RIL_CMD_PROXY_5,
                  m_slot_id);
    } else if (strStartsWith(pRawUrc, "+EIMSPDIS:")) {
        sendEvent(RFX_MSG_EVENT_DATA_PCSCF_ADDRESS_DISCOVERY, RfxStringData(pRawUrc),
                  RIL_CMD_PROXY_5, m_slot_id);
    } else if (strStartsWith(pRawUrc, "+CGEV: ME MODIFY")) {
        sendEvent(RFX_MSG_EVENT_QUERY_PCO_WITH_URC, RfxStringData(pRawUrc), RIL_CMD_PROXY_5,
                  m_slot_id);
    } else if (strStartsWith(pRawUrc, "+EIAREG: ME ATTACH")) {
        sendEvent(RFX_MSG_EVENT_QUERY_PCO_WITH_URC, RfxStringData(pRawUrc), RIL_CMD_PROXY_5,
                  m_slot_id);
    } else if (strStartsWith(pRawUrc, "+EXLCE:")) {
        sendEvent(RFX_MSG_EVENT_DATA_LINK_CAPACITY_ESTIMATE, RfxStringData(pRawUrc),
                  RIL_CMD_PROXY_5, m_slot_id);
    } else if (strStartsWith(pRawUrc, "+ECCAUSE:")) {
        sendEvent(RFX_MSG_EVENT_NETWORK_REJECT_CAUSE, RfxStringData(pRawUrc), RIL_CMD_PROXY_5,
                  m_slot_id);
    } else if (strStartsWith(pRawUrc, "+EIWLPL:")) {
        // Support IWLAN AP-ASSISTED mode
        sendEvent(RFX_MSG_EVENT_QUALIFIED_NETWORK_TYPES_CHANGED, RfxStringData(pRawUrc),
                  RIL_CMD_PROXY_5, m_slot_id);
    } else if (strStartsWith(pRawUrc, "+EGCMD:")) {
        sendEvent(RFX_MSG_EVENT_MOBILE_DATA_USAGE_NOTIFY, RfxStringData(pRawUrc), RIL_CMD_PROXY_5,
                  m_slot_id);
    } else if (strStartsWith(pRawUrc, "+EKALIVE:")) {
        sendEvent(RFX_MSG_EVENT_KEEPALIVE_STATUS, RfxStringData(pRawUrc), RIL_CMD_PROXY_5,
                  m_slot_id);
    } else if (strStartsWith(pRawUrc, "+ENWLIMIT:")) {
        sendEvent(RFX_MSG_EVENT_DATA_NW_LIMIT_NOTIFY, RfxStringData(pRawUrc), RIL_CMD_PROXY_5,
                  m_slot_id);
    }
}

void RmcDcUrcHandler::onHandleEvent(const sp<RfxMclMessage>& msg) {
    switch (msg->getId()) {
        case RFX_MSG_EVENT_IMS_REQUEST_HANDLER_REGISTER_DONE:
            notifyUrcHandlerRegistrationDone();
            break;
        default:
            RFX_LOG_W(RFX_LOG_TAG, "[%d][%s]: Unknown event, ignore!", m_slot_id, __FUNCTION__);
            break;
    }
}

// notify ImsReqHandler about URC registration done.
void RmcDcUrcHandler::notifyUrcHandlerRegistrationDone() {
    sendEvent(RFX_MSG_EVENT_URC_HANDLER_REGISTER_DONE, RfxVoidData(), RIL_CMD_PROXY_5, m_slot_id);
    RFX_LOG_D(RFX_LOG_TAG, "[%d]Notify ImsRegHandler URC registration is done", m_slot_id);
}
