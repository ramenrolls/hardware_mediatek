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
#include "RfxVoidData.h"
#include "RmcSimRequestEntryHandler.h"
#include "RmcGsmSimRequestHandler.h"
#include "RmcCdmaSimRequestHandler.h"
#include "RmcCommSimRequestHandler.h"
#include <telephony/mtk_ril.h>
#include "RfxOpUtils.h"
#include <stdio.h>

/*****************************************************************************
 * Class RfxController
 *****************************************************************************/
RFX_IMPLEMENT_HANDLER_CLASS(RmcSimRequestEntryHandler, RIL_CMD_PROXY_1);
RFX_REGISTER_HANDLER_CLASS(RmcSimRequestEntryHandler, RIL_CMD_PROXY_3);
// External SIM [Start]
#ifdef MTK_EXTERNAL_SIM_SUPPORT
RFX_REGISTER_HANDLER_CLASS(RmcSimRequestEntryHandler, RIL_CMD_PROXY_11);
#endif
// External SIM [End]
RFX_REGISTER_HANDLER_CLASS(RmcSimRequestEntryHandler, RIL_CMD_PROXY_7);

RmcSimRequestEntryHandler::RmcSimRequestEntryHandler(int slot_id, int channel_id)
    : RmcSimBaseHandler(slot_id, channel_id) {
    setTag(String8("RmcSimBaseRequest"));
    // Create Gsm SIM Controller and C2K SIM Controller
    if (RfxOpUtils::getOpHandler() != NULL) {
        mGsmReqHandler = (RmcGsmSimRequestHandler*)RfxOpUtils::getSimOpHandler(MTK_RIL_SIM_GSM_REQ,
                                                                               slot_id, channel_id);
        mCdmaReqHandler = (RmcCdmaSimRequestHandler*)RfxOpUtils::getSimOpHandler(
                MTK_RIL_SIM_CDMA_REQ, slot_id, channel_id);
        mCommReqHandler = (RmcCommSimRequestHandler*)RfxOpUtils::getSimOpHandler(
                MTK_RIL_SIM_COMM_REQ, slot_id, channel_id);
    } else {
        RFX_HANDLER_CREATE(mGsmReqHandler, RmcGsmSimRequestHandler, (slot_id, channel_id));
        RFX_HANDLER_CREATE(mCdmaReqHandler, RmcCdmaSimRequestHandler, (slot_id, channel_id));
        RFX_HANDLER_CREATE(mCommReqHandler, RmcCommSimRequestHandler, (slot_id, channel_id));
    }
    // External SIM [Start]
#ifdef MTK_EXTERNAL_SIM_SUPPORT
    if (RfxRilUtils::isExternalSimSupport()) {
        RFX_HANDLER_CREATE(mVsimReqHandler, RmcVsimRequestHandler, (slot_id, channel_id));
    } else {
        mVsimReqHandler = NULL;
    }
#else
    mVsimReqHandler = NULL;
#endif
    // External SIM [End]
    // register request
    int record_num = 0;
    if (mGsmReqHandler != NULL) {
        const int* list1 = mGsmReqHandler->queryTable(m_channel_id, &record_num);
        if (list1 != NULL) {
            registerToHandleRequest(list1, record_num);
        }
        if (RfxOpUtils::getOpHandler() != NULL && NULL != list1) {
            free((void*)list1);
        }
    }

    record_num = 0;
    if (mCdmaReqHandler != NULL) {
        const int* list2 = mCdmaReqHandler->queryTable(m_channel_id, &record_num);
        if (list2 != NULL) {
            registerToHandleRequest(list2, record_num);
        }
    }

    record_num = 0;
    if (mCommReqHandler != NULL) {
        const int* list3 = mCommReqHandler->queryTable(m_channel_id, &record_num);
        if (list3 != NULL) {
            registerToHandleRequest(list3, record_num);
        }
        if (RfxOpUtils::getOpHandler() != NULL && NULL != list3) {
            free((void*)list3);
        }
    }
    // External SIM [Start]
#ifdef MTK_EXTERNAL_SIM_SUPPORT
    if (mVsimReqHandler != NULL) {
        record_num = 0;
        const int* list4 = mVsimReqHandler->queryTable(m_channel_id, &record_num);
        if (list4 != NULL) {
            registerToHandleRequest(list4, record_num);
        }
    }
#endif
    // External SIM [End]
    // register event
    record_num = 0;
    if (mGsmReqHandler != NULL) {
        const int* eventList1 = mGsmReqHandler->queryEventTable(m_channel_id, &record_num);
        if (eventList1 != NULL) {
            registerToHandleEvent(eventList1, record_num);
        }
    }

    record_num = 0;
    if (mCdmaReqHandler != NULL) {
        const int* eventList2 = mCdmaReqHandler->queryEventTable(m_channel_id, &record_num);
        if (eventList2 != NULL) {
            registerToHandleEvent(eventList2, record_num);
        }
    }

    record_num = 0;
    if (mCommReqHandler != NULL) {
        const int* eventList3 = mCommReqHandler->queryEventTable(m_channel_id, &record_num);
        if (eventList3 != NULL) {
            registerToHandleEvent(eventList3, record_num);
        }
    }
    // External SIM [Start]
#ifdef MTK_EXTERNAL_SIM_SUPPORT
    if (mVsimReqHandler != NULL) {
        record_num = 0;
        const int* eventList4 = mVsimReqHandler->queryEventTable(m_channel_id, &record_num);
        if (eventList4 != NULL) {
            registerToHandleEvent(eventList4, record_num);
        }
    }
#endif
    // External SIM [End]
}

RmcSimRequestEntryHandler::~RmcSimRequestEntryHandler() {}

void RmcSimRequestEntryHandler::onHandleRequest(const sp<RfxMclMessage>& msg) {
    int request = msg->getId();
    if (mCommReqHandler->needHandle(msg) == RmcSimBaseHandler::RESULT_NEED) {
        mCommReqHandler->handleRequest(msg);
    } else if (mGsmReqHandler->needHandle(msg) == RmcSimBaseHandler::RESULT_NEED) {
        mGsmReqHandler->handleRequest(msg);
    } else if (mCdmaReqHandler->needHandle(msg) == RmcSimBaseHandler::RESULT_NEED) {
        mCdmaReqHandler->handleRequest(msg);
        // External SIM [Start]
#ifdef MTK_EXTERNAL_SIM_SUPPORT
    } else if (mVsimReqHandler != NULL &&
               mVsimReqHandler->needHandle(msg) == RmcSimBaseHandler::RESULT_NEED) {
        mVsimReqHandler->handleRequest(msg);
#endif
        // External SIM [End]
    } else {
        // Impossible case
        logE(mTag, "Not support the request!");
        sp<RfxMclMessage> response =
                RfxMclMessage::obtainResponse(request, RIL_E_SIM_ERR, RfxVoidData(), msg, false);
        responseToTelCore(response);
    }
}

void RmcSimRequestEntryHandler::onHandleEvent(const sp<RfxMclMessage>& msg) {
    int event = msg->getId();

    if (mCommReqHandler->needHandle(msg) == RmcSimBaseHandler::RESULT_NEED) {
        mCommReqHandler->handleEvent(msg);
    } else if (mGsmReqHandler->needHandle(msg) == RmcSimBaseHandler::RESULT_NEED) {
        mGsmReqHandler->handleEvent(msg);
    } else if (mCdmaReqHandler->needHandle(msg) == RmcSimBaseHandler::RESULT_NEED) {
        mCdmaReqHandler->handleEvent(msg);
        // External SIM [Start]
#ifdef MTK_EXTERNAL_SIM_SUPPORT
    } else if (mVsimReqHandler != NULL &&
               mVsimReqHandler->needHandle(msg) == RmcSimBaseHandler::RESULT_NEED) {
        mVsimReqHandler->handleEvent(msg);
#endif
        // External SIM [End]
    } else {
        logD(mTag, "No one handle the event!");
    }
}
