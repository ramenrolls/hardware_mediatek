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
#include "RfxMessageId.h"
#include "RtcAtciController.h"
#include "rfx_properties.h"

/*****************************************************************************
 * Class RtcAtciController
 *****************************************************************************/
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringData, RfxStringData,
                                RFX_MSG_REQUEST_OEM_HOOK_ATCI_INTERNAL);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringData, RfxStringData, RFX_MSG_REQUEST_ATCI_SPECIAL_COMMAND);
RFX_REGISTER_DATA_TO_URC_ID(RfxStringData, RFX_MSG_UNSOL_ATCI_RESPONSE);
RFX_IMPLEMENT_CLASS("RtcAtciController", RtcAtciController, RfxController);

const char* RtcAtciController::LOG_TAG_ATCI = "RtcAtciController";
const char* RtcAtciController::ENABLE_URC_PROP = "persist.vendor.service.atci_urc.enable";

void RtcAtciController::onInit() {
    logD(LOG_TAG_ATCI, "[%s] enter", __FUNCTION__);
    RfxController::onInit();
    const int request_id_list[] = {
            RFX_MSG_REQUEST_OEM_HOOK_ATCI_INTERNAL,
    };
    registerToHandleRequest(request_id_list, sizeof(request_id_list) / sizeof(const int), NORMAL);
}

bool RtcAtciController::onHandleAtciRequest(const sp<RfxMessage>& message) {
    int messageId = message->getId();
    logD(LOG_TAG_ATCI, "[%s] message: %d %s", __FUNCTION__, messageId, idToString(messageId));
    switch (messageId) {
        case RFX_MSG_REQUEST_OEM_HOOK_ATCI_INTERNAL: {
            String8 command = String8((char*)message->getData()->getData());
            if (command == String8("AT+CAPL")) {
                sp<RfxMessage> newMessage =
                        RfxMessage::obtainRequest(RFX_MSG_REQUEST_ATCI_SPECIAL_COMMAND,
                                                  RfxStringData((char*)command.string()), message);
                requestToMcl(newMessage);
            } else if (command.find("AT%EURC") >= 0) {
                logD(LOG_TAG_ATCI, "[%s] message: %s", __FUNCTION__, command.string());
                RfxStringData data((char*)"\r\nOK\r\n");
                command.removeAll("\r");
                command.removeAll("\n");
                if (command == String8("AT%EURC=1")) {
                    rfx_property_set(ENABLE_URC_PROP, "1");
                } else if (command == String8("AT%EURC=0")) {
                    rfx_property_set(ENABLE_URC_PROP, "0");
                } else {
                    data = RfxStringData((char*)"\r\nERROR\r\n");
                }
                responseToRilj(RfxMessage::obtainResponse(getSlotId(),
                                                          RFX_MSG_REQUEST_OEM_HOOK_ATCI_INTERNAL,
                                                          RIL_E_SUCCESS, data, message));
            } else {
                requestToMcl(message);
            }
            break;
        }

        default:
            logE(LOG_TAG_ATCI, "[%s] should not be here", __FUNCTION__);
            return false;
    }
    return true;
}

bool RtcAtciController::onHandleAtciResponse(const sp<RfxMessage>& message) {
    int messageId = message->getId();
    logD(LOG_TAG_ATCI, "[%s] message: %d %s", __FUNCTION__, messageId, idToString(messageId));
    switch (messageId) {
        case RFX_MSG_REQUEST_OEM_HOOK_ATCI_INTERNAL:
            responseToRilj(message);
            break;

        case RFX_MSG_REQUEST_ATCI_SPECIAL_COMMAND: {
            sp<RfxMessage> response =
                    RfxMessage::obtainResponse(RFX_MSG_REQUEST_OEM_HOOK_ATCI_INTERNAL, message);
            responseToRilj(response);
            break;
        }

        default:
            logE(LOG_TAG_ATCI, "[%s] should not be here", __FUNCTION__);
            return false;
    }
    return true;
}
