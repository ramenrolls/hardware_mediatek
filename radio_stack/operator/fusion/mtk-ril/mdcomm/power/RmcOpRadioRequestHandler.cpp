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

#include "RmcOpRadioRequestHandler.h"
#include "libmtkrilutils.h"

#define RFX_LOG_TAG "RmcOpRadioReq"

RFX_IMPLEMENT_OP_HANDLER_CLASS(RmcOpRadioRequestHandler, RIL_CMD_PROXY_9);

RmcOpRadioRequestHandler::RmcOpRadioRequestHandler(int slotId, int channelId)
    : RmcRadioRequestHandler(slotId, channelId) {
    const int requests[] = {
            RFX_MSG_REQUEST_COMMAND_BEFORE_RADIO_POWER,
    };
    registerToHandleRequest(requests, sizeof(requests) / sizeof(int));
}

RmcOpRadioRequestHandler::~RmcOpRadioRequestHandler() {}

void RmcOpRadioRequestHandler::onHandleRequest(const sp<RfxMclMessage>& msg) {
    int id = msg->getId();
    logD(RFX_LOG_TAG, "onHandleRequest: %d", id);
    switch (id) {
        case RFX_MSG_REQUEST_COMMAND_BEFORE_RADIO_POWER:
            onhandlePreRadioCommand(msg);
            break;
        default:
            RmcRadioRequestHandler::onHandleRequest(msg);
            break;
    }
}

void RmcOpRadioRequestHandler::onhandlePreRadioCommand(const sp<RfxMclMessage>& msg) {
    if (isOp12Support()) {
        String8 pco =
                getMclStatusManager()->getString8Value(RFX_STATUS_KEY_PCO_STATUS, String8(""));
        int power = ((int*)msg->getData()->getData())[0];
        char propValue[RFX_PROPERTY_VALUE_MAX] = {0};
        // new test case.: don't control radio flow at open device
        rfx_property_get("persist.vendor.pco5.radio.ctrl", propValue, "0");
        if (pco == String8::format("FF00:5") && !power && (atoi(propValue) != 0)) {
            logI(RFX_LOG_TAG, "Send COPS");
            atSendCommand("AT+COPS=2");
        }
    }
}
