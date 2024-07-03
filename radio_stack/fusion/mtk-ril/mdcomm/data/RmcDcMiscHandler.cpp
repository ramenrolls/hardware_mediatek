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
#include <regex>
#include <string>
#include "RmcDataDefs.h"
#include "RmcDcMiscHandler.h"
#include "RfxMessageId.h"
#include "RfxIntsData.h"

#define RFX_LOG_TAG "RmcDcMiscHandler"

/*****************************************************************************
 * Class RmcDcMiscHandler
 * Only receive request and event in this class.
 * Create RmcDcMiscImpl object to send AT Command to modem.
 *****************************************************************************/
RFX_IMPLEMENT_HANDLER_CLASS(RmcDcMiscHandler, RIL_CMD_PROXY_5);
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_URC_NETWORK_REJECT_CAUSE);

RmcDcMiscHandler::RmcDcMiscHandler(int slotId, int channelId)
    : RfxBaseHandler(slotId, channelId), m_pRmcDcMiscImpl(NULL) {
    const int requestList[] = {
            RFX_MSG_REQUEST_START_LCE,
            RFX_MSG_REQUEST_STOP_LCE,
            RFX_MSG_REQUEST_PULL_LCEDATA,
            RFX_MSG_REQUEST_SET_FD_MODE,
            RFX_MSG_REQUEST_SET_LINK_CAPACITY_REPORTING_CRITERIA,
            RFX_MSG_REQUEST_SEND_DEVICE_STATE,
    };

    const int eventList[] = {
            RFX_MSG_EVENT_DATA_LCE_STATUS_CHANGED,
            RFX_MSG_EVENT_RADIO_CAPABILITY_UPDATED,
            RFX_MSG_EVENT_DATA_LINK_CAPACITY_ESTIMATE,
            RFX_MSG_EVENT_NETWORK_REJECT_CAUSE,
    };

    registerToHandleRequest(requestList, sizeof(requestList) / sizeof(int));
    registerToHandleEvent(eventList, sizeof(eventList) / sizeof(int));

    m_pRmcDcMiscImpl = new RmcDcMiscImpl(this, slotId);
    m_pRmcDcMiscImpl->init();
}

RmcDcMiscHandler::~RmcDcMiscHandler() {
    delete m_pRmcDcMiscImpl;
    m_pRmcDcMiscImpl = NULL;
}

void RmcDcMiscHandler::onHandleRequest(const sp<RfxMclMessage>& msg) {
    switch (msg->getId()) {
        case RFX_MSG_REQUEST_START_LCE:
            handleStartLceRequest(msg);
            break;
        case RFX_MSG_REQUEST_STOP_LCE:
            handleStopLceRequest(msg);
            break;
        case RFX_MSG_REQUEST_PULL_LCEDATA:
            handlePullLceDataRequest(msg);
            break;
        case RFX_MSG_REQUEST_SET_FD_MODE:
            handleSetFdModeRequest(msg);
            break;
        case RFX_MSG_REQUEST_SET_LINK_CAPACITY_REPORTING_CRITERIA:
            handleSetLinkCapacityReportingCriteriaRequest(msg);
            break;
        case RFX_MSG_REQUEST_SEND_DEVICE_STATE:
            handleSendDeviceStateRequest(msg);
            break;
        default:
            RFX_LOG_W(RFX_LOG_TAG, "[%d][%s]: Unknown request, ignore!", m_slot_id, __FUNCTION__);
            break;
    }
}

void RmcDcMiscHandler::onHandleEvent(const sp<RfxMclMessage>& msg) {
    switch (msg->getId()) {
        case RFX_MSG_EVENT_DATA_LCE_STATUS_CHANGED:
            handleLceStatusChanged(msg);
            break;
        case RFX_MSG_EVENT_RADIO_CAPABILITY_UPDATED:
            init();
            break;
        case RFX_MSG_EVENT_DATA_LINK_CAPACITY_ESTIMATE:
            handleLinkCapacityEstimate(msg);
            break;
        case RFX_MSG_EVENT_NETWORK_REJECT_CAUSE:
            handleNetworkRejectCause(msg);
            break;
        default:
            RFX_LOG_W(RFX_LOG_TAG, "[%d][%s]: Unknown event, ignore!", m_slot_id, __FUNCTION__);
            break;
    }
}

void RmcDcMiscHandler::init() {
    RFX_ASSERT(m_pRmcDcMiscImpl != NULL);
    m_pRmcDcMiscImpl->init();
}

void RmcDcMiscHandler::handleStartLceRequest(const sp<RfxMclMessage>& msg) {
    RFX_ASSERT(m_pRmcDcMiscImpl != NULL);
    m_pRmcDcMiscImpl->requestStartLce(msg);
}

void RmcDcMiscHandler::handleStopLceRequest(const sp<RfxMclMessage>& msg) {
    RFX_ASSERT(m_pRmcDcMiscImpl != NULL);
    m_pRmcDcMiscImpl->requestStopLce(msg);
}

void RmcDcMiscHandler::handlePullLceDataRequest(const sp<RfxMclMessage>& msg) {
    RFX_ASSERT(m_pRmcDcMiscImpl != NULL);
    m_pRmcDcMiscImpl->requestPullLceData(msg);
}

void RmcDcMiscHandler::handleLceStatusChanged(const sp<RfxMclMessage>& msg) {
    RFX_ASSERT(m_pRmcDcMiscImpl != NULL);
    m_pRmcDcMiscImpl->onLceStatusChanged(msg);
}

void RmcDcMiscHandler::handleSetFdModeRequest(const sp<RfxMclMessage>& msg) {
    RFX_ASSERT(m_pRmcDcMiscImpl != NULL);
    m_pRmcDcMiscImpl->setFdMode(msg);
}

void RmcDcMiscHandler::handleSetLinkCapacityReportingCriteriaRequest(const sp<RfxMclMessage>& msg) {
    RFX_ASSERT(m_pRmcDcMiscImpl != NULL);
    m_pRmcDcMiscImpl->requestSetLinkCapacityReportingCriteria(msg);
}

void RmcDcMiscHandler::handleLinkCapacityEstimate(const sp<RfxMclMessage>& msg) {
    RFX_ASSERT(m_pRmcDcMiscImpl != NULL);
    m_pRmcDcMiscImpl->onLinkCapacityEstimate(msg);
}

void RmcDcMiscHandler::handleNetworkRejectCause(const sp<RfxMclMessage>& msg) {
    RFX_ASSERT(m_pRmcDcMiscImpl != NULL);
    std::string prefix = "ECCAUSE:";
    std::smatch m;
    static std::regex r1(prefix + " *(\\d+), *(\\d+), *(\\d+)");
    int emmCause = -1;
    int esmCause = -1;
    int event = -1;
    std::string urc = std::string((char*)msg->getData()->getData());

    // ECCAUSE: <emm_cause>,<esm_cause>,<event>
    RFX_LOG_V(RFX_LOG_TAG, "[%d][%s] urc %s", m_slot_id, __FUNCTION__, urc.c_str());
    if (urc.find(prefix) != std::string::npos) {
        std::regex_search(urc, m, r1);

        if (m.empty() || m.size() != 4) {
            for (std::size_t n = 0; n < m.size(); ++n) {
                RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] match error [%s]", m_slot_id, __FUNCTION__,
                          m.str(n).c_str());
            }
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] ECCAUSE content error, #matched %d, return", m_slot_id,
                      __FUNCTION__, (int)m.size());
            return;
        } else {
            emmCause = std::stoi(m.str(1), NULL, 10);
            esmCause = std::stoi(m.str(2), NULL, 10);
            event = std::stoi(m.str(3), NULL, 10);

            RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] emm %d, esm %d, event %d", m_slot_id, __FUNCTION__,
                      emmCause, esmCause, event);
        }
    } else {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] urc format error, return", m_slot_id, __FUNCTION__);
        return;
    }

    sp<RfxMclMessage> rejCauseUrc;
    int response[3] = {0};
    response[0] = emmCause;
    response[1] = esmCause;
    response[2] = event;
    rejCauseUrc = RfxMclMessage::obtainUrc(RFX_MSG_URC_NETWORK_REJECT_CAUSE, m_slot_id,
                                           RfxIntsData(response, 3));
    responseToTelCore(rejCauseUrc);
    return;
}

void RmcDcMiscHandler::handleSendDeviceStateRequest(const sp<RfxMclMessage>& msg) {
    RFX_ASSERT(m_pRmcDcMiscImpl != NULL);
    int* pReqInt = (int*)msg->getData()->getData();

    // Only handle LOW_DATA_EXPECTED
    if (pReqInt[0] == FASTDORMANCY_LOW_DATA_EXPECTED) {
        int settings[FASTDORMANCY_PARAMETER_LENGTH] = {0};
        settings[0] = 1;           // args Num
        settings[1] = pReqInt[1];  // state
        RFX_LOG_D(RFX_LOG_TAG, "[%d][%s]: state: %d", m_slot_id, __FUNCTION__, settings[1]);

        RfxIntsData data(settings, FASTDORMANCY_PARAMETER_LENGTH);
        sp<RfxMclMessage> request =
                RfxMclMessage::obtainRequest(msg->getId(), &data, msg->getSlotId(), msg->getToken(),
                                             msg->getSendToMainProtocol(), msg->getRilToken(),
                                             msg->getTimeStamp(), msg->getAddAtFront());
        m_pRmcDcMiscImpl->setFdMode(request);
    } else {
        sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(
                msg->getId(), RIL_E_REQUEST_NOT_SUPPORTED, RfxVoidData(), msg, false);
        responseToTelCore(response);
    }
}
