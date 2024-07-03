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
#include "RmcSimBaseHandler.h"
#include "RmcGsmSimOpUrcHandler.h"
#include <telephony/mtk_ril.h>
#include "rfx_properties.h"
#include "RfxTokUtils.h"
#include "RmcCommSimDefs.h"
#include "RfxIntsData.h"
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif
#include "usim_fcp_parser.h"
#ifdef __cplusplus
}
#endif

using ::android::String8;

static const char* gsmOpUrcList[] = {
        "+ETMOEVT:",
};

RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_URC_SIM_MELOCK_NOTIFICATION);

/*****************************************************************************
 * Class RfxController
 *****************************************************************************/
RmcGsmSimOpUrcHandler::RmcGsmSimOpUrcHandler(int slot_id, int channel_id)
    : RmcGsmSimUrcHandler(slot_id, channel_id) {
    setTag(String8("RmcGsmSimOpUrcHandler"));
    logD(mTag, "RmcGsmSimOpUrcHandler %d, %d", slot_id, m_channel_id);
}

RmcGsmSimOpUrcHandler::~RmcGsmSimOpUrcHandler() {}

RmcSimBaseHandler::SIM_HANDLE_RESULT RmcGsmSimOpUrcHandler::needHandle(
        const sp<RfxMclMessage>& msg) {
    RmcSimBaseHandler::SIM_HANDLE_RESULT result = RmcSimBaseHandler::RESULT_IGNORE;
    char* ss = msg->getRawUrc()->getLine();

    if (strStartsWith(ss, "+ETMOEVT:")) {
        result = RmcSimBaseHandler::RESULT_NEED;
    } else {
        result = RmcGsmSimUrcHandler::needHandle(msg);
    }
    return result;
}

void RmcGsmSimOpUrcHandler::handleUrc(const sp<RfxMclMessage>& msg, RfxAtLine* urc) {
    String8 ss(urc->getLine());

    if (strStartsWith(ss, "+ETMOEVT:")) {
        //        handleRemoteSimUnlockEvent(msg, ss);
    } else {
        RmcGsmSimUrcHandler::handleUrc(msg, urc);
    }
}

const char** RmcGsmSimOpUrcHandler::queryUrcTable(int* record_num) {
    const char** superTable = RmcGsmSimUrcHandler::queryUrcTable(record_num);
    int subRecordNumber = 0;
    int supRecordNumber = *record_num;
    char** bufTable = NULL;
    int i = 0, j = 0;

    subRecordNumber = sizeof(gsmOpUrcList) / sizeof(char*);
    *record_num = subRecordNumber + supRecordNumber;
    bufTable = (char**)calloc(1, *record_num * sizeof(char*));
    RFX_ASSERT(bufTable != NULL);
    for (i = 0; i < supRecordNumber; i++) {
        asprintf(&(bufTable[i]), "%s", superTable[i]);
    }
    for (j = i; j < *record_num; j++) {
        asprintf(&(bufTable[j]), "%s", gsmOpUrcList[j - supRecordNumber]);
    }
    return (const char**)bufTable;
}

void RmcGsmSimOpUrcHandler::handleRemoteSimUnlockEvent(const sp<RfxMclMessage>& msg, String8 urc) {
    int eventId = -1;

    RFX_UNUSED(msg);

    if (strStartsWith(urc, "+ETMOEVT: 0")) {  // +ETMOEVT
        eventId = 0;
    } else {
        logD(mTag, "handleRemoteSimUnlockEvent: unsupport type");
    }

    logD(mTag, "handleRemoteSimUnlockEvent: eventId = %d", eventId);
    sp<RfxMclMessage> unsol = RfxMclMessage::obtainUrc(RFX_MSG_URC_SIM_MELOCK_NOTIFICATION,
                                                       m_slot_id, RfxIntsData(&eventId, 1));
    responseToTelCore(unsol);
}
