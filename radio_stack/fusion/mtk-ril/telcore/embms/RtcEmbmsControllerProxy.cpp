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

#include "RtcEmbmsAtController.h"
#include "RtcEmbmsControllerProxy.h"
#include "RtcEmbmsUtils.h"
#include "RfxLog.h"
#include "rfx_properties.h"
#include <telephony/mtk_ril.h>
#include "RfxMessageId.h"
#include "RfxStringData.h"
#include "RfxIntsData.h"
#include "RfxVoidData.h"
#include "embms/RfxEmbmsGetCoverageRespData.h"
#include "embms/RfxEmbmsEnableRespData.h"
#include "embms/RfxEmbmsDisableRespData.h"
#include "embms/RfxEmbmsStartSessionReqData.h"
#include "embms/RfxEmbmsStartSessionRespData.h"
#include "embms/RfxEmbmsStopSessionReqData.h"
#include "embms/RfxEmbmsModemEeNotifyData.h"
#include "embms/RfxEmbmsGetTimeRespData.h"
#include "embms/RfxEmbmsLocalEnableRespData.h"
#include "embms/RfxEmbmsLocalStartSessionReqData.h"
#include "embms/RfxEmbmsLocalStartSessionRespData.h"
#include "embms/RfxEmbmsLocalStopSessionReqData.h"
#include "embms/RfxEmbmsLocalSessionNotifyData.h"
#include "embms/RfxEmbmsLocalSaiNotifyData.h"
#include "embms/RfxEmbmsLocalOosNotifyData.h"
#include "embms/RfxEmbmsCellInfoNotifyData.h"
#include "embms/RfxEmbmsActiveSessionNotifyData.h"
#include "embms/RfxEmbmsSaiNotifyData.h"
#include "embms/RfxEmbmsOosNotifyData.h"

#define RFX_LOG_TAG "RTC_EMBMS_CON_PROXY"

#define RTC_EMBMS_PROPERTY_ENABLE "persist.vendor.sys.embms.enable"

/*****************************************************************************
 * Class RtcEmbmsControllerProxy
 *****************************************************************************/

RFX_IMPLEMENT_CLASS("RtcEmbmsControllerProxy", RtcEmbmsControllerProxy, RfxController);
// between RILJ and Rtc layer
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringData, RfxStringData, RFX_MSG_REQUEST_EMBMS_AT_CMD);
RFX_REGISTER_DATA_TO_URC_ID(RfxStringData, RFX_MSG_URC_EMBMS_AT_INFO);
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_URC_RTC_EMBMS_SESSION_STATUS);

// between Rtc and Rmc layer
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxEmbmsLocalEnableRespData,
                                RFX_MSG_REQUEST_EMBMS_ENABLE);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxEmbmsDisableRespData,
                                RFX_MSG_REQUEST_EMBMS_DISABLE);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxEmbmsLocalStartSessionReqData, RfxEmbmsLocalStartSessionRespData,
                                RFX_MSG_REQUEST_EMBMS_START_SESSION);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxEmbmsLocalStopSessionReqData, RfxEmbmsLocalStartSessionRespData,
                                RFX_MSG_REQUEST_EMBMS_STOP_SESSION);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxEmbmsGetTimeRespData,
                                RFX_MSG_REQUEST_EMBMS_GET_TIME);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxEmbmsGetCoverageRespData,
                                RFX_MSG_REQUEST_RTC_EMBMS_GET_COVERAGE_STATE);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_EMBMS_SET_E911);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxVoidData,
                                RFX_MSG_REQUEST_EMBMS_TRIGGER_CELL_INFO_NOTIFY);

RFX_REGISTER_DATA_TO_URC_ID(RfxEmbmsLocalStartSessionRespData,
                            RFX_MSG_URC_EMBMS_START_SESSION_RESPONSE);
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_URC_EMBMS_CELL_INFO_NOTIFICATION);
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_URC_EMBMS_COVERAGE_STATE);
RFX_REGISTER_DATA_TO_URC_ID(RfxEmbmsLocalSessionNotifyData, RFX_MSG_URC_EMBMS_ACTIVE_SESSION);
RFX_REGISTER_DATA_TO_URC_ID(RfxEmbmsLocalSessionNotifyData, RFX_MSG_URC_EMBMS_AVAILABLE_SESSION);
RFX_REGISTER_DATA_TO_URC_ID(RfxEmbmsLocalSaiNotifyData, RFX_MSG_URC_EMBMS_SAI_LIST_NOTIFICATION);
RFX_REGISTER_DATA_TO_URC_ID(RfxEmbmsLocalOosNotifyData, RFX_MSG_URC_EMBMS_OOS_NOTIFICATION);

// between Rtc layers
RFX_REGISTER_DATA_TO_EVENT_ID(RfxVoidData, RFX_MSG_EVENT_EMBMS_INITIAL_VARIABLE);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxStringData, RFX_MSG_EVENT_EMBMS_POST_NETWORK_UPDATE);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxStringData, RFX_MSG_EVENT_EMBMS_POST_HVOLTE_UPDATE);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxVoidData, RFX_MSG_EVENT_EMBMS_POST_SAI_UPDATE);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxVoidData, RFX_MSG_EVENT_EMBMS_POST_SESSION_UPDATE);

RtcEmbmsControllerProxy::RtcEmbmsControllerProxy() : mActiveEmbmsController(NULL) {}

RtcEmbmsControllerProxy::~RtcEmbmsControllerProxy() {}

void RtcEmbmsControllerProxy::onInit() {
    RfxController::onInit();  // Required: invoke super class implementation

    RtcEmbmsAtController* p;
    RFX_OBJ_CREATE(p, RtcEmbmsAtController, this);
    mActiveEmbmsController = (RfxController*)p;
    logD(RFX_LOG_TAG, "onInit to TK-AT: ctrl = %p.", mActiveEmbmsController);

    getStatusManager()->registerStatusChanged(
            RFX_STATUS_KEY_RADIO_STATE,
            RfxStatusChangeCallback(this, &RtcEmbmsControllerProxy::onRadioStateChanged));
}

void RtcEmbmsControllerProxy::onDeinit() {
    logD(RFX_LOG_TAG, "onDeinit");
    RfxController::onDeinit();
}

void RtcEmbmsControllerProxy::onRadioStateChanged(RfxStatusKeyEnum key, RfxVariant old_value,
                                                  RfxVariant value) {
    char defaultData[RFX_PROPERTY_VALUE_MAX] = {0};
    RFX_UNUSED(key);
    RFX_UNUSED(old_value);

    RIL_RadioState radioState = (RIL_RadioState)value.asInt();
    logD(RFX_LOG_TAG, "radioState %d", radioState);

    if (radioState == RADIO_STATE_ON) {
        rfx_property_get(RTC_EMBMS_PROPERTY_ENABLE, defaultData, "");
        if (strlen(defaultData) == 0 || strcmp("0", defaultData) == 0) {
            return;
        }

        int trans_id;
        int intdata[2];
        intdata[0] = 0;  // trans_id
        intdata[1] = EMBMS_COMMAND_RIL;

        sp<RfxMessage> newMsg =
                RfxMessage::obtainRequest(getSlotId(), RFX_MSG_REQUEST_EMBMS_ENABLE,
                                          RfxIntsData(intdata, 2), RADIO_TECH_GROUP_GSM);

        requestToMcl(newMsg);
    }
}
