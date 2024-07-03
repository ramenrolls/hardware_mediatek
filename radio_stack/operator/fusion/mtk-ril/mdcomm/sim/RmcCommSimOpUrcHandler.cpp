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
#include "RfxStatusDefs.h"
#include "RfxVoidData.h"
#include "RfxStringData.h"
#include "RfxStringsData.h"
#include "RmcSimBaseHandler.h"
#include "RmcCommSimUrcHandler.h"
#include "RmcCommSimOpUrcHandler.h"
#include "RmcCommSimDefs.h"
#include "utils/String8.h"
#include "rfx_properties.h"
#include "RfxIntsData.h"
#include "RfxMessageId.h"
#include "RfxMisc.h"
#include "RfxRilUtils.h"
#include <cutils/properties.h>
#include <telephony/mtk_ril.h>

using ::android::String8;

static const char* commOpUrcList[] = {"+EATTEVT:", "+ESMLRSUEVT:"};

RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_URC_ATT_SIM_LOCK_NOTIFICATION);
RFX_REGISTER_DATA_TO_URC_ID(RfxStringsData, RFX_MSG_UNSOL_RSU_EVENT);

/*****************************************************************************
 * Class RfxController
 *****************************************************************************/
RmcCommSimOpUrcHandler::RmcCommSimOpUrcHandler(int slot_id, int channel_id)
    : RmcCommSimUrcHandler(slot_id, channel_id) {
    setTag(String8("RmcCommSimOpUrc"));
}

RmcCommSimOpUrcHandler::~RmcCommSimOpUrcHandler() {}

RmcSimBaseHandler::SIM_HANDLE_RESULT RmcCommSimOpUrcHandler::needHandle(
        const sp<RfxMclMessage>& msg) {
    RmcSimBaseHandler::SIM_HANDLE_RESULT result = RmcSimBaseHandler::RESULT_IGNORE;
    char* ss = msg->getRawUrc()->getLine();

    if (strStartsWith(ss, "+EATTEVT:") || strStartsWith(ss, "+ESMLRSUEVT:")) {
        result = RmcSimBaseHandler::RESULT_NEED;
    } else {
        result = RmcCommSimUrcHandler::needHandle(msg);
    }
    return result;
}

void RmcCommSimOpUrcHandler::handleUrc(const sp<RfxMclMessage>& msg, RfxAtLine* urc) {
    String8 ss(urc->getLine());

    if (strStartsWith(ss, "+EATTEVT:")) {
        handleAttRsuSimLockEvent(msg, ss);
    } else if (strStartsWith(ss, "+ESMLRSUEVT:")) {
        handleRsuEvent(msg, urc);
    } else {
        RmcCommSimUrcHandler::handleUrc(msg, urc);
    }
}

const char** RmcCommSimOpUrcHandler::queryUrcTable(int* record_num) {
    const char** superTable = RmcCommSimUrcHandler::queryUrcTable(record_num);
    int subRecordNumber = 0;
    int supRecordNumber = *record_num;
    char** bufTable = NULL;
    int i = 0, j = 0;

    subRecordNumber = sizeof(commOpUrcList) / sizeof(char*);
    *record_num = subRecordNumber + supRecordNumber;
    bufTable = (char**)calloc(1, *record_num * sizeof(char*));
    RFX_ASSERT(bufTable != NULL);
    for (i = 0; i < supRecordNumber; i++) {
        asprintf(&(bufTable[i]), "%s", superTable[i]);
    }
    for (j = i; j < *record_num; j++) {
        asprintf(&(bufTable[j]), "%s", commOpUrcList[j - supRecordNumber]);
    }
    return (const char**)bufTable;
}

void RmcCommSimOpUrcHandler::handleAttRsuSimLockEvent(const sp<RfxMclMessage>& msg, String8 urc) {
    int eventId = -1;

    RFX_UNUSED(msg);

    if (strStartsWith(urc, "+EATTEVT: 0")) {  // +EATTEVT
        eventId = 0;
    } else {
        logD(mTag, "[RSU-SIMLOCK] handleAttRsuSimLockEvent: unsupport type");
    }

    logD(mTag, "[RSU-SIMLOCK] handleAttRsuSimLockEvent: eventId = %d", eventId);
    sp<RfxMclMessage> unsol = RfxMclMessage::obtainUrc(RFX_MSG_URC_ATT_SIM_LOCK_NOTIFICATION,
                                                       m_slot_id, RfxIntsData(&eventId, 1));
    responseToTelCore(unsol);
}

void RmcCommSimOpUrcHandler::handleRsuEvent(const sp<RfxMclMessage>& msg, RfxAtLine* urc) {
    int err = 0;
    int opId = 0;
    int eventId = -1;
    char* eventString = NULL;
    char* event[2] = {NULL, NULL};
    RfxAtLine* pLine = urc;

    RFX_UNUSED(msg);

    if (pLine == NULL) {
        return;
    }

    pLine->atTokStart(&err);
    if (err < 0) {
        goto error;
    }

    opId = pLine->atTokNextint(&err);
    if (err < 0) {
        goto error;
    }

    if (pLine->atTokHasmore()) {
        eventId = pLine->atTokNextint(&err);
        if (err < 0) {
            goto error;
        }
        asprintf(&event[0], "%d", eventId);
        if (pLine->atTokHasmore()) {
            eventString = pLine->atTokNextstr(&err);
        }
        asprintf(&event[1], "%s", (eventString == NULL ? "" : eventString));
        sp<RfxMclMessage> p_urc = RfxMclMessage::obtainUrc(RFX_MSG_UNSOL_RSU_EVENT, m_slot_id,
                                                           RfxStringsData(event, 2));
        responseToTelCore(p_urc);
        if (event[0] != NULL) {
            free(event[0]);
        }
        if (event[1] != NULL) {
            free(event[1]);
        }
        return;
    }

error:
    logE(mTag, "handleRsuEvent: error");
}
