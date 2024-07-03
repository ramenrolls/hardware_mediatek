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

#include "RmcImsProvisioningUrcHandler.h"
#include <string>
#include <vector>

// register handler to channel
RFX_IMPLEMENT_HANDLER_CLASS(RmcImsProvisioningUrcHandler, RIL_CMD_PROXY_URC);

// register data
RFX_REGISTER_DATA_TO_URC_ID(RfxStringsData, RFX_MSG_UNSOL_GET_PROVISION_DONE);

RmcImsProvisioningUrcHandler::RmcImsProvisioningUrcHandler(int slot_id, int channel_id)
    : RfxBaseHandler(slot_id, channel_id) {
    logD(RFX_LOG_TAG, "RmcImsProvisioningUrcHandler constructor");
    int m_slot_id = slot_id;
    int m_channel_id = channel_id;
    const char* urc[] = {
            "+EPVSGET",
    };
    registerToHandleURC(urc, sizeof(urc) / sizeof(char*));
}

RmcImsProvisioningUrcHandler::~RmcImsProvisioningUrcHandler() {}

void RmcImsProvisioningUrcHandler::onHandleUrc(const sp<RfxMclMessage>& msg) {
    if (strStartsWith(msg->getRawUrc()->getLine(), "+EPVSGET")) {
        handleProvisionValueReceived(msg);
    }
}

void RmcImsProvisioningUrcHandler::handleProvisionValueReceived(const sp<RfxMclMessage>& msg) {
    /*
     * +EPVSGET: <provision item>, <provision value>
     */
    logD(RFX_LOG_TAG, "handleProvisionValueReceived");
    const int maxLen = 2;
    int rfxMsg = RFX_MSG_UNSOL_GET_PROVISION_DONE;
    bool appendPhoneId = true;
    notifyStringsDataToTcl(msg, rfxMsg, maxLen, appendPhoneId);
}
