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
#include "RtcHelloController.h"
#include "RtcSampleController.h"

/*****************************************************************************
 * Class RfxController
 *****************************************************************************/

RFX_IMPLEMENT_CLASS("RtcHelloController", RtcHelloController, RfxController);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxStringData, RIL_REQUEST_GET_IMEI);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxStringData, RIL_REQUEST_GET_IMEISV);
RFX_REGISTER_DATA_TO_URC_ID(RfxStringsData, RIL_UNSOL_ON_USSD);

RtcHelloController::RtcHelloController() : m_member_data1(0), m_member_data2(1) {}

RtcHelloController::~RtcHelloController() {}

void RtcHelloController::onInit() {
    // Required: invoke super class implementation
    RfxController::onInit();

    const int request_id_list[] = {RIL_REQUEST_GET_IMEI, RIL_REQUEST_GET_IMEISV, RIL_REQUEST_UDUB,
                                   RIL_REQUEST_RADIO_POWER};

    const int urc_id_list[] = {
            RIL_UNSOL_ON_USSD,
            RIL_UNSOL_ON_USSD_REQUEST,
            RFX_MSG_URC_DATA_CALL_LIST_CHANGED,
    };

    // register request & URC id list
    // NOTE. one id can only be registered by one controller
    registerToHandleRequest(request_id_list, sizeof(request_id_list) / sizeof(const int));
    registerToHandleUrc(urc_id_list, sizeof(urc_id_list) / sizeof(const int));

    // register callbacks to get required information
    getStatusManager()->registerStatusChanged(
            RFX_STATUS_KEY_CARD_TYPE,
            RfxStatusChangeCallback(this, &RtcHelloController::onCardTypeChanged));

    RtcSampleController* sample_controller;

    // create the object of RfxSampleController as the child controller
    // of this object (an instance of RtcSampleController)
    RFX_OBJ_CREATE(sample_controller, RtcSampleController, this);

    // connect the signal defined by another module
    sample_controller->m_something_changed_singal.connect(
            this, &RtcHelloController::onSampleControlerSomethingChanged);
}

void RtcHelloController::onCardTypeChanged(RfxStatusKeyEnum key, RfxVariant old_value,
                                           RfxVariant value) {
    int card_type = value.asInt();
}

bool RtcHelloController::onHandleRequest(const sp<RfxMessage>& message) {
    int msg_id = message->getId();
    switch (msg_id) {
        case RIL_REQUEST_GET_IMEI: {
            /* Scenario 1: No modify to RfxMessage. Send the RfxMessage request to RILD directly. */
            requestToMcl(message);

            /* Scenario 2: Create a new RfxMessage based on an existing RfxMessage.
             *   And, with the same RfxBaseData
             */
            sp<RfxMessage> request =
                    RfxMessage::obtainRequest(m_slot_id, RIL_REQUEST_GET_IMEI, message);
            // Obtain function will cpoy RfxDataBase to new RfxMessage automatically.
            requestToMcl(request);

            /* Scenario 3: Create a new RfxMessage based on an existing RfxMessage.
             *   But, with different RfxBaseData and id
             */
            int foo[3] = {1, 2, 3};
            sp<RfxMessage> request_2 = RfxMessage::obtainRequest(m_slot_id, RIL_REQUEST_GET_IMEISV,
                                                                 RfxIntsData(foo, 3));
            // Obtain function will create new RfxMessage with id and RfxBaseData
            requestToMcl(request_2);

            // post invoke a function if required
            rfxPostInvoke0(this, &RtcHelloController::onGetIMEI);

            m_timer_handle =
                    RfxTimer::start(RfxCallback0(this, &RtcHelloController::onTimer), s2ns(1));

            sp<RfxMessage> msg =
                    RfxMessage::obtainRequest(RFX_SLOT_ID_1, RIL_REQUEST_UDUB, RfxVoidData());

            /* Scenario 4: send URC to framework
             */
            char* imei = "123456789123456";
            sp<RfxMessage> urc =
                    RfxMessage::obtainUrc(m_slot_id, RIL_REQUEST_GET_IMEI, RfxStringData(imei));
        } break;
        default:
            break;
    }
    return true;
}

void RtcHelloController::onGetIMEI() {
    // post to do something
}

bool RtcHelloController::onHandleUrc(const sp<RfxMessage>& message) {
    int msg_id = message->getId();
    if (msg_id == RIL_UNSOL_ON_USSD) {
        // 1. decompress message.parcel to get URC data

        // 2. if any predefined status need to be shared to other modules
        //    set it to status manager, status manager will inform
        //    registed callbacks
        getStatusManager()->setIntValue(RFX_STATUS_KEY_CARD_TYPE, 2);

        RfxTimer::stop(m_timer_handle);

        // 3. if the URC need to be sent to RILJ, send it,
        //    be able to update parceled data if required
        responseToRilj(message);
    }
    return true;
}

bool RtcHelloController::onHandleResponse(const sp<RfxMessage>& message) {
    responseToRilj(message);
    sp<RfxMessage> msg = sp<RfxMessage>(NULL);
    ResponseStatus responseStatus = preprocessResponse(
            message, msg,
            RfxWaitResponseTimedOutCallback(this, &RtcHelloController::responseCallBack), s2ns(10));

    if (responseStatus == RESPONSE_STATUS_HAVE_MATCHED) {
        // DO SOMETHING.
    } else if (responseStatus == RESPONSE_STATUS_HAVE_MATCHED) {
        // DO SOMETHING.
    }
    return true;
}

void RtcHelloController::responseCallBack(const sp<RfxMessage>& message) {
    /// DO Something.
}

void RtcHelloController::onSampleControlerSomethingChanged(int d1, int d2, int d3, int d4) {
    // do something
}

void RtcHelloController::onTimer() {
    // do something
}
