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
#include "RmcSimUrcEntryHandler.h"
#include <telephony/mtk_ril.h>
#include "RfxDefs.h"
#include "RfxOpUtils.h"

/*****************************************************************************
 * Class RfxController
 *****************************************************************************/
RFX_IMPLEMENT_HANDLER_CLASS(RmcSimUrcEntryHandler, RIL_CMD_PROXY_URC);

RmcSimUrcEntryHandler::RmcSimUrcEntryHandler(int slot_id, int channel_id)
    : RmcSimBaseHandler(slot_id, channel_id) {
    setTag(String8("RmcSimUrcEntry"));
    // Create Gsm SIM Controller and C2K SIM Controller
    if (RfxOpUtils::getOpHandler() != NULL) {
        mGsmUrcHandler = (RmcGsmSimUrcHandler*)RfxOpUtils::getSimOpHandler(MTK_RIL_SIM_GSM_URC,
                                                                           slot_id, channel_id);
        mCdmaUrcHandler = (RmcCdmaSimUrcHandler*)RfxOpUtils::getSimOpHandler(MTK_RIL_SIM_CDMA_URC,
                                                                             slot_id, channel_id);
        mCommUrcHandler = (RmcCommSimUrcHandler*)RfxOpUtils::getSimOpHandler(MTK_RIL_SIM_COMM_URC,
                                                                             slot_id, channel_id);
    } else {
        RFX_HANDLER_CREATE(mGsmUrcHandler, RmcGsmSimUrcHandler, (slot_id, channel_id));
        RFX_HANDLER_CREATE(mCdmaUrcHandler, RmcCdmaSimUrcHandler, (slot_id, channel_id));
        RFX_HANDLER_CREATE(mCommUrcHandler, RmcCommSimUrcHandler, (slot_id, channel_id));
    }
    // External SIM [Start]
#ifdef MTK_EXTERNAL_SIM_SUPPORT
    if (RfxRilUtils::isExternalSimSupport()) {
        RFX_HANDLER_CREATE(mVsimUrcHandler, RmcVsimUrcHandler, (slot_id, channel_id));
    } else {
        mVsimUrcHandler = NULL;
    }
#else
    mVsimUrcHandler = NULL;
#endif
    // External SIM [End]
    // register request
    int record_num = 0;
    if (mGsmUrcHandler != NULL) {
        const char** gsm_urc_list = mGsmUrcHandler->queryUrcTable(&record_num);
        if (gsm_urc_list != NULL) {
            registerToHandleURC(gsm_urc_list, record_num);
        }
        if (RfxOpUtils::getOpHandler() != NULL && NULL != gsm_urc_list) {
            for (int i = 0; i < record_num; i++) {
                free((void*)gsm_urc_list[i]);
            }
            free((void*)gsm_urc_list);
        }
    }

    record_num = 0;
    if (mCdmaUrcHandler != NULL) {
        const char** cdma_urc_list = mCdmaUrcHandler->queryUrcTable(&record_num);
        if (cdma_urc_list != NULL) {
            registerToHandleURC(cdma_urc_list, record_num);
        }
    }

    record_num = 0;
    if (mCommUrcHandler != NULL) {
        const char** comm_urc_list = mCommUrcHandler->queryUrcTable(&record_num);
        if (comm_urc_list != NULL) {
            registerToHandleURC(comm_urc_list, record_num);
        }
        if (RfxOpUtils::getOpHandler() != NULL && NULL != comm_urc_list) {
            for (int i = 0; i < record_num; i++) {
                free((void*)comm_urc_list[i]);
            }
            free((void*)comm_urc_list);
        }
    }
    // External SIM [Start]
#ifdef MTK_EXTERNAL_SIM_SUPPORT
    if (mVsimUrcHandler != NULL) {
        record_num = 0;
        const char** vsim_urc_list = mVsimUrcHandler->queryUrcTable(&record_num);
        if (vsim_urc_list != NULL) {
            registerToHandleURC(vsim_urc_list, record_num);
        }
    }
#endif
    // External SIM [End]

    // register request, urc
}

RmcSimUrcEntryHandler::~RmcSimUrcEntryHandler() {}

void RmcSimUrcEntryHandler::onHandleUrc(const sp<RfxMclMessage>& msg) {
    bool isHandled = false;
    RfxAtLine* urc = new RfxAtLine(msg->getRawUrc()->getLine(), NULL);

    if (mCommUrcHandler->needHandle(msg) == RmcSimBaseHandler::RESULT_NEED) {
        isHandled = true;
        mCommUrcHandler->handleUrc(msg, urc);
    }

    if (urc != NULL) {
        delete (urc);
    }

    urc = new RfxAtLine(msg->getRawUrc()->getLine(), NULL);
    if (mGsmUrcHandler->needHandle(msg) == RmcSimBaseHandler::RESULT_NEED) {
        isHandled = true;
        mGsmUrcHandler->handleUrc(msg, urc);
    } else if (mCdmaUrcHandler->needHandle(msg) == RmcSimBaseHandler::RESULT_NEED) {
        isHandled = true;
        mCdmaUrcHandler->handleUrc(msg, urc);
        // External SIM [Start]
#ifdef MTK_EXTERNAL_SIM_SUPPORT
    } else if (mVsimUrcHandler != NULL &&
               mVsimUrcHandler->needHandle(msg) == RmcSimBaseHandler::RESULT_NEED) {
        isHandled = true;
        mVsimUrcHandler->handleUrc(msg, urc);
#endif
        // External SIM [End]
    }
    if (urc != NULL) {
        delete (urc);
    }

    if (!isHandled) {
        // No one want to handle the URC
        logE(mTag, "No one handle the request!");
    }
}
