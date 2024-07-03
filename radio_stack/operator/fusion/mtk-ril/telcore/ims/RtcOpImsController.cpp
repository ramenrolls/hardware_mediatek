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
#include <vector>
#include "RtcOpImsController.h"
#include "RfxRootController.h"

#include "RfxIntsData.h"
#include "RfxStringsData.h"
#include "RfxVoidData.h"
#include "RfxMessageId.h"

/*****************************************************************************
 * Class RfxController
 *****************************************************************************/

#define RFX_LOG_TAG "RtcOpIms"

RFX_IMPLEMENT_CLASS("RtcOpImsController", RtcOpImsController, RfxController);

RFX_REGISTER_DATA_TO_URC_ID(RfxStringsData, RFX_MSG_UNSOL_GET_TRN_INDICATION);

RtcOpImsController::RtcOpImsController() {}

RtcOpImsController::~RtcOpImsController() {}

void RtcOpImsController::onInit() {
    // Required: invoke super class implementation
    RfxController::onInit();
    const int request_id_list[] = {RFX_MSG_REQUEST_SET_DIGITS_LINE, RFX_MSG_REQUEST_SET_TRN,
                                   RFX_MSG_REQUEST_SWITCH_RCS_ROI_STATUS,
                                   RFX_MSG_REQUEST_UPDATE_RCS_CAPABILITIES,
                                   RFX_MSG_REQUEST_UPDATE_RCS_SESSION_INFO};

    const int urc_id_list[] = {RFX_MSG_UNSOL_DIGITS_LINE_INDICATION,
                               RFX_MSG_UNSOL_GET_TRN_INDICATION,
                               RFX_MSG_UNSOL_RCS_DIGITS_LINE_INFO};

    // register request & URC id list
    // NOTE. one id can only be registered by one controller
    registerToHandleRequest(request_id_list, sizeof(request_id_list) / sizeof(const int), DEFAULT);
    registerToHandleUrc(urc_id_list, sizeof(urc_id_list) / sizeof(const int));
}

void RtcOpImsController::onDeinit() { RfxController::onDeinit(); }

bool RtcOpImsController::onHandleRequest(const sp<RfxMessage>& message) {
    int msg_id = message->getId();

    switch (msg_id) {
        case RFX_MSG_REQUEST_SET_TRN:
            handleSetTrn(message);
            return true;
        default:
            break;
    }
    requestToMcl(message);
    return true;
}

bool RtcOpImsController::onHandleUrc(const sp<RfxMessage>& message) {
    responseToRilj(message);
    return true;
}

bool RtcOpImsController::onHandleResponse(const sp<RfxMessage>& response) {
    responseToRilj(response);
    return true;
}

bool RtcOpImsController::onCheckIfRejectMessage(const sp<RfxMessage>& message, bool isModemPowerOff,
                                                int radioState) {
    /* Reject the request when modem off */
    if (isModemPowerOff == true) {
        return true;
    }
    return false;
}

void RtcOpImsController::handleSetTrn(const sp<RfxMessage>& message) {
    char** data = (char**)message->getData()->getData();

    // data[0] is fromMsisdn
    // data[1] is toMsisdn
    // data[2] is TRN
    // When TRN is invalid as NULL or empty, set TRN as original toMsisdn.
    if (data[2] == NULL || strlen(data[2]) == 0) {
        String8 trn(data[1]);
        getStatusManager()->setString8Value(RFX_STATUS_KEY_TRN, trn);
    } else {
        String8 trn(data[2]);
        getStatusManager()->setString8Value(RFX_STATUS_KEY_TRN, trn);
    }

    // create the KEY for callback query with format as <fromMsisdn>_to_<toMsisdn>
    int len = strlen(data[0]) + strlen(data[1]) + 4;
    char* address = (char*)alloca(len + 1);
    memset(address, 0, len + 1);
    strncpy(address, data[0], strlen(data[0]));
    strncat(address, "_to_", 4);
    strncat(address, data[1], len - strlen(data[1]));
    std::string addrString(address);

    // If KEY not matched, won't callback
    sp<RfxAction> action = findAction(addrString);
    if ((sp<RfxAction>)NULL != action) {
        action->act();
        sp<RfxMessage> responseMsg = RfxMessage::obtainResponse(RIL_E_SUCCESS, message, true);
        responseToRilj(responseMsg);
    } else {
        sp<RfxMessage> responseMsg =
                RfxMessage::obtainResponse(RIL_E_GENERIC_FAILURE, message, true);
        responseToRilj(responseMsg);
    }
}

sp<RfxAction> RtcOpImsController::findAction(std::string addrString) {
    std::unordered_map<std::string, sp<RfxAction>>::const_iterator result =
            mActionMap.find(addrString);

    if (result == mActionMap.end()) {
        return NULL;
    }
    sp<RfxAction> action = result->second;
    mActionMap.erase(addrString);
    return action;
}

void RtcOpImsController::getTrn(const char* fromMsisdn, const char* toMsisdn, const char* address,
                                const sp<RfxAction>& action) {
    // create the KEY for callback query with format as <fromMsisdn>_to_<toMsisdn>
    char* data[2];
    data[0] = strdup(fromMsisdn);
    data[1] = strdup(toMsisdn);
    sp<RfxMessage> urc = RfxMessage::obtainUrc(m_slot_id, RFX_MSG_UNSOL_GET_TRN_INDICATION,
                                               RfxStringsData(data, 2));
    responseToRilj(urc);

    std::string addrString(address);

    if ((sp<RfxAction>)NULL != action) {
        mActionMap.insert({addrString, action});
    }
}
