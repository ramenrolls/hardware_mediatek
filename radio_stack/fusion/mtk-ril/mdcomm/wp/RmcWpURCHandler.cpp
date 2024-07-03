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

#include "RmcWpURCHandler.h"
#include "RmcWpRequestHandler.h"
#include <telephony/mtk_ril.h>
#include "RfxLog.h"
#include "rfx_properties.h"

#define WP_LOG_TAG "RmcWp"
RFX_IMPLEMENT_HANDLER_CLASS(RmcWpURCHandler, RIL_CMD_PROXY_URC);
int RmcWpURCHandler::mSlotId = 0;
int RmcWpURCHandler::ecsraUrcParams[5] = {0};

RmcWpURCHandler::RmcWpURCHandler(int slot_id, int channel_id)
    : RfxBaseHandler(slot_id, channel_id) {
    const char* urc[] = {
            "+ECSRA",
    };
    logD(WP_LOG_TAG, "constructor slot_id = %d, channel_id = %d", slot_id, channel_id);
    registerToHandleURC(urc, sizeof(urc) / sizeof(char*));
}

RmcWpURCHandler::~RmcWpURCHandler() {}

void RmcWpURCHandler::onHandleUrc(const sp<RfxMclMessage>& msg) {
    logD(WP_LOG_TAG, "+ECSRA info = %s", msg->getRawUrc()->getLine());
    if (strStartsWith(msg->getRawUrc()->getLine(), "+ECSRA:")) {
        mSlotId = msg->getSlotId();
        handleWorldModeUrc(msg);
    }
}

void RmcWpURCHandler::handleWorldModeUrc(const sp<RfxMclMessage>& msg) {
    int err;
    // parse urc info
    logD(WP_LOG_TAG, "handleWorldModeUrc begin");
    RfxAtLine* Urcline = msg->getRawUrc();
    Urcline->atTokStart(&err);

    if (err < 0) {
        logD(WP_LOG_TAG, "handleWorldModeUrc err %d", err);
        return;
    }

    for (int i = 0; i <= 4; i++) {
        ecsraUrcParams[i] = Urcline->atTokNextint(&err);
        if (err < 0) {
            logD(WP_LOG_TAG, "handleWorldModeUrc err %d %d %d", err, i, ecsraUrcParams[i]);
            return;
        }
    }

    // set world mode prepare state
    rfx_property_set("persist.vendor.radio.wm_state", "-1");
    // inform telcore to world mode change start
    int WorldModeState[2];
    WorldModeState[0] = 0;
    // send cause to RTC
    WorldModeState[1] = ecsraUrcParams[4];
    sp<RfxMclMessage> urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_WORLD_MODE_CHANGED, mSlotId,
                                                     RfxIntsData(WorldModeState, 2));
    responseToTelCore(urc);
    logD(WP_LOG_TAG, "handleWorldModeUrc state = %d", WorldModeState[0]);
}

int RmcWpURCHandler::getSlotId() { return mSlotId; }

int RmcWpURCHandler::getEcsraUrcParam(int index) {
    assert(index >= 0 && index <= 4);
    return ecsraUrcParams[index];
}

bool RmcWpURCHandler::onCheckIfRejectMessage(const sp<RfxMclMessage>& msg,
                                             RIL_RadioState radioState) {
    bool reject = false;
    if (RADIO_STATE_UNAVAILABLE == radioState) {
        if (strStartsWith(msg->getRawUrc()->getLine(), "+ECSRA:")) {
            reject = false;
        } else {
            reject = true;
        }
    }
    logD(WP_LOG_TAG, "onCheckIfRejectMessage: %d %d", radioState, reject);
    return reject;
}
