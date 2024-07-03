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

#include <string>
#include <vector>

#include "RfxBaseHandler.h"
#include "RfxHandlerManager.h"
#include "RfxMclDispatcherThread.h"
#include "RfxDispatchThread.h"
#include "RfxIntsData.h"
#include "RfxIdToStringUtils.h"
#include "RfxStringData.h"
#include "RfxStringsData.h"
#include "RfxRawData.h"
#include "RfxVoidData.h"
#include "RfxMessageId.h"
#include "RfxLog.h"
#include "RfxVersionManager.h"

#ifdef __cplusplus
extern "C" {
#endif
#include <vendor/mediatek/ims/radio_stack/platformlib/common/libmtkrilutils/proto/sap-api.pb.h>
#include "pb_decode.h"
#include "pb_encode.h"
#ifdef __cplusplus
}
#endif

RFX_REGISTER_DATA_TO_URC_ID(RfxRawData, RFX_MSG_URC_SIM_SAP_STATUS);

#define RFX_LOG_TAG "RfxBaseHandler"

void RfxBaseHandler::processMessage(const sp<RfxMclMessage>& msg) {
    if (REQUEST == msg->getType() || SAP_REQUEST == msg->getType()) {
        onHandleRequest(msg);
    } else if (RAW_URC == msg->getType()) {
        // check radio state condition
        RIL_RadioState radioState = (RIL_RadioState)getMclStatusManager()->getIntValue(
                RFX_STATUS_KEY_RADIO_STATE, RADIO_STATE_OFF);
        if (onCheckIfRejectMessage(msg, radioState)) {
            return;
        }

        onHandleUrc(msg);
    } else if (EVENT == msg->getType()) {
        onHandleEvent(msg);
    }
}

void RfxBaseHandler::registerToHandleRequest(const int* request_id_list, size_t length) {
    int realChannelId = m_channel_id + RIL_PROXY_OFFSET * m_slot_id;
    RfxHandlerManager::registerToHandleRequest(this, realChannelId, m_slot_id, request_id_list,
                                               length);
}

void RfxBaseHandler::registerToHandleURC(const char** urc_prefix_list, size_t length) {
    int realChannelId = m_channel_id + RIL_PROXY_OFFSET * m_slot_id;
    RfxHandlerManager::registerToHandleUrc(this, realChannelId, m_slot_id, urc_prefix_list, length);
}

void RfxBaseHandler::registerToHandleEvent(const int* event_id_list, size_t length) {
    int realChannelId = m_channel_id + RIL_PROXY_OFFSET * m_slot_id;
    RfxHandlerManager::registerToHandleEvent(this, realChannelId, m_slot_id, event_id_list, length);
}

void RfxBaseHandler::registerToHandleEvent(int client_id, const int* event_id_list, size_t length) {
    int realChannelId = m_channel_id + RIL_PROXY_OFFSET * m_slot_id;
    RfxHandlerManager::registerToHandleEvent(this, realChannelId, m_slot_id, client_id,
                                             event_id_list, length);
}

void RfxBaseHandler::responseToTelCore(const sp<RfxMclMessage>& msg) {
    if (RESPONSE == msg->getType()) {
        RfxDispatchThread::enqueueResponseMessage(msg);
    } else if (URC == msg->getType()) {
        RfxDispatchThread::enqueueUrcMessage(msg);
    } else if (SAP_RESPONSE == msg->getType()) {
        RfxDispatchThread::enqueueSapResponseMessage(msg);
    } else if (SAP_URC == msg->getType()) {
        RfxDispatchThread::enqueueSapUrcMessage(msg);
    } else {
        logE(RFX_LOG_TAG, "responseToTelCore error!");
    }
}

void RfxBaseHandler::sendEvent(int id, const RfxBaseData& data, int channelId, int slotId,
                               int clientId, int token, nsecs_t delayTime,
                               MTK_RIL_REQUEST_PRIORITY priority) {
    sp<RfxMclMessage> msg = RfxMclMessage::obtainEvent(id, data, channelId, slotId, clientId, token,
                                                       delayTime, priority);
    sendEvent(msg);
}

void RfxBaseHandler::sendEvent(sp<RfxMclMessage> msg) {
    msg->setMainProtocolSlotId(
            getNonSlotMclStatusManager()->getIntValue(RFX_STATUS_KEY_MAIN_CAPABILITY_SLOT, 0));
    if (msg->getDelayTime() > 0) {
        RfxMclDispatcherThread::enqueueMclMessageDelay(msg);
    } else {
        RfxMclDispatcherThread::enqueueMclMessage(msg);
    }
}

void RfxBaseHandler::sendBtSapStatusInd(RIL_SIM_SAP_STATUS_IND_Status message) {
    // after encoding, send this message to telecore
    int msgId = MsgId_RIL_SIM_SAP_STATUS;
    RIL_SIM_SAP_STATUS_IND unsolMsg;
    size_t encoded_size = 0;
    size_t buffer_size = 0;
    pb_ostream_t ostream;
    bool success = false;
    ssize_t written_bytes;
    int status = getMclStatusManager()->getIntValue(RFX_STATUS_KEY_BTSAP_STATUS, BT_SAP_INIT);

    logD("[BTSAP]", "notifyBtSapStatusInd, (%d)", message);
    unsolMsg.statusChange = message;

    if (message == RIL_SIM_SAP_STATUS_IND_Status_RIL_SIM_STATUS_CARD_RESET &&
        status == BT_SAP_CONNECTION_SETUP) {
        getMclStatusManager()->setIntValue(RFX_STATUS_KEY_BTSAP_STATUS, BT_SAP_ONGOING_CONNECTION);
    }

    // send unsolicited message STATUS_IND
    if ((success = pb_get_encoded_size(&encoded_size, RIL_SIM_SAP_STATUS_IND_fields, &unsolMsg)) &&
        encoded_size <= INT32_MAX) {
        buffer_size = encoded_size;
        uint8_t buffer[buffer_size];
        ostream = pb_ostream_from_buffer(buffer, buffer_size);
        success = pb_encode(&ostream, RIL_SIM_SAP_STATUS_IND_fields, &unsolMsg);

        if (success) {
            logD("[BTSAP]", "notifyBtSapStatusInd, Size: %zu (0x%zx)", encoded_size, encoded_size);
            sp<RfxMclMessage> response = RfxMclMessage::obtainSapUrc(
                    RFX_MSG_URC_SIM_SAP_STATUS, m_slot_id, RfxRawData(buffer, buffer_size));
            responseToTelCore(response);
        } else {
            logE("[BTSAP]", "notifyBtSapStatusInd, Encode failed!");
        }
    } else {
        logE("[BTSAP]", "Not sending response type %d: encoded_size: %zu. encoded size result: %d",
             msgId, encoded_size, success);
    }
}

RfxSender* RfxBaseHandler::getSender() {
    int realChannelId = m_channel_id + RIL_PROXY_OFFSET * m_slot_id;
    return RfxChannelManager::getSender(realChannelId);
}

void RfxBaseHandler::logD(const char* tag, const char* fmt, ...) const {
    va_list ap;
    char buf[LOG_BUF_SIZE] = {0};

    String8 tagString;
    if (RfxRilUtils::getRilRunMode() == RilRunMode::RIL_RUN_MODE_MOCK) {
        tagString = String8::format("%s%s", "[GT]", tag);
    } else {
        tagString = String8::format("%s", tag);
    }
    va_start(ap, fmt);
    vsnprintf(buf, LOG_BUF_SIZE, fmt, ap);
    va_end(ap);

    RfxRilUtils::printLog(DEBUG, tagString, String8::format("%s", buf), m_slot_id);
}

void RfxBaseHandler::logI(const char* tag, const char* fmt, ...) const {
    va_list ap;
    char buf[LOG_BUF_SIZE] = {0};

    String8 tagString;
    if (RfxRilUtils::getRilRunMode() == RilRunMode::RIL_RUN_MODE_MOCK) {
        tagString = String8::format("%s%s", "[GT]", tag);
    } else {
        tagString = String8::format("%s", tag);
    }
    va_start(ap, fmt);
    vsnprintf(buf, LOG_BUF_SIZE, fmt, ap);
    va_end(ap);

    RfxRilUtils::printLog(INFO, tagString, String8::format("%s", buf), m_slot_id);
}

void RfxBaseHandler::logV(const char* tag, const char* fmt, ...) const {
    va_list ap;
    char buf[LOG_BUF_SIZE] = {0};

    String8 tagString;
    if (RfxRilUtils::getRilRunMode() == RilRunMode::RIL_RUN_MODE_MOCK) {
        tagString = String8::format("%s%s", "[GT]", tag);
    } else {
        tagString = String8::format("%s", tag);
    }
    va_start(ap, fmt);
    vsnprintf(buf, LOG_BUF_SIZE, fmt, ap);
    va_end(ap);

    RfxRilUtils::printLog(VERBOSE, tagString, String8::format("%s", buf), m_slot_id);
}

void RfxBaseHandler::logE(const char* tag, const char* fmt, ...) const {
    va_list ap;
    char buf[LOG_BUF_SIZE] = {0};

    String8 tagString;
    if (RfxRilUtils::getRilRunMode() == RilRunMode::RIL_RUN_MODE_MOCK) {
        tagString = String8::format("%s%s", "[GT]", tag);
    } else {
        tagString = String8::format("%s", tag);
    }
    va_start(ap, fmt);
    vsnprintf(buf, LOG_BUF_SIZE, fmt, ap);
    va_end(ap);

    RfxRilUtils::printLog(ERROR, tagString, String8::format("%s", buf), m_slot_id);
}

void RfxBaseHandler::logW(const char* tag, const char* fmt, ...) const {
    va_list ap;
    char buf[LOG_BUF_SIZE] = {0};

    String8 tagString;
    if (RfxRilUtils::getRilRunMode() == RilRunMode::RIL_RUN_MODE_MOCK) {
        tagString = String8::format("%s%s", "[GT]", tag);
    } else {
        tagString = String8::format("%s", tag);
    }
    va_start(ap, fmt);
    vsnprintf(buf, LOG_BUF_SIZE, fmt, ap);
    va_end(ap);

    RfxRilUtils::printLog(WARN, tagString, String8::format("%s", buf), m_slot_id);
}

RfxMclStatusManager* RfxBaseHandler::getMclStatusManager() {
    return getMclStatusManager(m_slot_id);
}

RfxMclStatusManager* RfxBaseHandler::getMclStatusManager(int slotId) {
    return RfxMclStatusManager::getMclStatusManager(slotId);
}

RfxMclStatusManager* RfxBaseHandler::getNonSlotMclStatusManager() {
    return RfxMclStatusManager::getMclStatusManager(RFX_SLOT_ID_UNKNOWN);
}

sp<RfxAtResponse> RfxBaseHandler::atSendCommandSingleline(const char* command,
                                                          const char* responsePrefix,
                                                          RIL_Token ackToken) {
    return getSender()->atSendCommandSinglelineAck(command, responsePrefix, ackToken);
}

sp<RfxAtResponse> RfxBaseHandler::atSendCommandSingleline(const String8& command,
                                                          const char* responsePrefix,
                                                          RIL_Token ackToken) {
    return getSender()->atSendCommandSinglelineAck(command.string(), responsePrefix, ackToken);
}

sp<RfxAtResponse> RfxBaseHandler::atSendCommandNumeric(const char* command, RIL_Token ackToken) {
    return getSender()->atSendCommandNumericAck(command, ackToken);
}
sp<RfxAtResponse> RfxBaseHandler::atSendCommandNumeric(const String8& command, RIL_Token ackToken) {
    return getSender()->atSendCommandNumericAck(command.string(), ackToken);
}

sp<RfxAtResponse> RfxBaseHandler::atSendCommandMultiline(const char* command,
                                                         const char* responsePrefix,
                                                         RIL_Token ackToken) {
    return getSender()->atSendCommandMultilineAck(command, responsePrefix, ackToken);
}
sp<RfxAtResponse> RfxBaseHandler::atSendCommandMultiline(const String8& command,
                                                         const char* responsePrefix,
                                                         RIL_Token ackToken) {
    return getSender()->atSendCommandMultilineAck(command.string(), responsePrefix, ackToken);
}

sp<RfxAtResponse> RfxBaseHandler::atSendCommand(const char* command, RIL_Token ackToken) {
    return getSender()->atSendCommandAck(command, ackToken);
}
sp<RfxAtResponse> RfxBaseHandler::atSendCommand(const String8& command, RIL_Token ackToken) {
    return getSender()->atSendCommandAck(command.string(), ackToken);
}

sp<RfxAtResponse> RfxBaseHandler::atSendCommandRaw(const char* command, RIL_Token ackToken) {
    return getSender()->atSendCommandRawAck(command, ackToken);
}
sp<RfxAtResponse> RfxBaseHandler::atSendCommandRaw(const String8& command, RIL_Token ackToken) {
    return getSender()->atSendCommandRawAck(command.string(), ackToken);
}

bool RfxBaseHandler::sendUserData(int clientId, unsigned char* data, size_t length) {
    return getSender()->sendUserData(clientId, data, length);
}

bool RfxBaseHandler::sendUserData(int clientId, int config, unsigned char* data, size_t length) {
    return getSender()->sendUserData(clientId, config, data, length);
}

int RfxBaseHandler::strStartsWith(const char* line, const char* prefix) {
    return RfxMisc::strStartsWith(line, prefix);
}

const char* RfxBaseHandler::idToString(int id) { return RFX_ID_TO_STR(id); }

int RfxBaseHandler::getFeatureVersion(char* feature, int defaultVaule) {
    return RfxVersionManager::getInstance()->getFeatureVersion(feature, defaultVaule);
}

int RfxBaseHandler::getFeatureVersion(char* feature) { return getFeatureVersion(feature, 0); }

void RfxBaseHandler::onHandleRequest(const sp<RfxMclMessage>& msg) { RFX_UNUSED(msg); }

void RfxBaseHandler::onHandleUrc(const sp<RfxMclMessage>& msg) { RFX_UNUSED(msg); }

void RfxBaseHandler::onHandleEvent(const sp<RfxMclMessage>& msg) { RFX_UNUSED(msg); }

bool RfxBaseHandler::onCheckIfRejectMessage(const sp<RfxMclMessage>& msg,
                                            RIL_RadioState radioState) {
    RFX_UNUSED(msg);
    RFX_UNUSED(radioState);
    return false;
}

/* An utility function for Urc.
 * To parse the URC and convert to the MCL data without any logic.
 */
void RfxBaseHandler::notifyStringsDataToTcl(const sp<RfxMclMessage>& msg, int rfxMsg, int maxLen,
                                            bool appendPhoneId) {
    RfxAtLine* urc = msg->getRawUrc();
    if (urc == NULL) return;
    // token init
    int ret;
    urc->atTokStart(&ret);
    if (ret < 0) return;

    std::vector<char*> data;

    // start to access token
    int index = 0;
    while (maxLen == 0 || index < maxLen) {
        char* str = urc->atTokNextstr(&ret);
        if (ret < 0) break;
        if (str == NULL) {
            str = (char*)"";  // assign an empty string
        }
        data.push_back(str);
        ++index;
    }

    // token more than expected
    if (urc->atTokHasmore()) {
        logD(RFX_LOG_TAG, "Wrong URC format: params more than limit");
    }

    if (appendPhoneId) {
        data.push_back((char*)std::to_string(m_slot_id).c_str());
    }

    RfxStringsData rfxData((char**)data.data(), (int)data.size());

    sp<RfxMclMessage> unsol = RfxMclMessage::obtainUrc(rfxMsg, m_slot_id, rfxData);
    responseToTelCore(unsol);
}

void RfxBaseHandler::notifyIntsDataToTcl(const sp<RfxMclMessage>& msg, int rfxMsg, int maxLen,
                                         bool appendPhoneId) {
    int err;
    RfxAtLine* line = msg->getRawUrc();

    line->atTokStart(&err);
    if (err < 0) return;

    std::vector<int> params;

    int index = 0;
    while (maxLen == 0 || index < maxLen) {
        int val = line->atTokNextint(&err);
        if (err < 0) break;
        params.push_back(val);
        ++index;
    }

    if (appendPhoneId) {
        params.push_back(m_slot_id);
    }

    sp<RfxMclMessage> urc = RfxMclMessage::obtainUrc(
            rfxMsg, m_slot_id, RfxIntsData(params.data(), (int)params.size()));
    responseToTelCore(urc);
}

RIL_Errno RfxBaseHandler::handleCmdWithVoidResponse(const sp<RfxMclMessage>& msg, String8 cmd) {
    sp<RfxAtResponse> atResponse = atSendCommand(cmd);
    // check at cmd result, consider default as success
    RIL_Errno result = RIL_E_SUCCESS;
    if (atResponse->getError() != 0 || atResponse->getSuccess() != 1) {
        // CC: Special VTS handling for normal call ATD when no SIM (ALPS03640532)
        // Call fail cause will be CM_CALL_REJECTED (21)
        if (atResponse->atGetCmeError() == CME_OPERATION_NOT_ALLOWED_ERR) {
            result = RIL_E_OPERATION_NOT_ALLOWED;
        } else {
            result = RIL_E_INTERNAL_ERR;
        }
        logE(RFX_LOG_TAG, "%d failed!", msg->getId());
    }

    // report to tcl
    sp<RfxMclMessage> mclResponse =
            RfxMclMessage::obtainResponse(msg->getId(), result, RfxVoidData(), msg);
    responseToTelCore(mclResponse);
    return result;
}
