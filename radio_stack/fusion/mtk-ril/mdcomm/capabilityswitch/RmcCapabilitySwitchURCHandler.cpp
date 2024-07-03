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

#include "RmcCapabilitySwitchURCHandler.h"
#include "RmcCapabilitySwitchUtil.h"
#include "RmcCapabilitySwitchRequestHandler.h"
#include <telephony/mtk_ril.h>
#include "RfxMessageId.h"
#include "RfxRadioCapabilityData.h"
#include "rfx_properties.h"

#define RFX_LOG_TAG "RmcCapa"

RFX_IMPLEMENT_HANDLER_CLASS(RmcCapabilitySwitchURCHandler, RIL_CMD_PROXY_URC);

Mutex RmcCapabilitySwitchURCHandler::s_switch_urc_channel_mutex;
int RmcCapabilitySwitchURCHandler::s_urc_count = 0;

RmcCapabilitySwitchURCHandler::RmcCapabilitySwitchURCHandler(int slot_id, int channel_id)
    : RfxBaseHandler(slot_id, channel_id) {
    const char* urc[] = {"+ESIMMAP", "+EPSMAP"};
    logI(RFX_LOG_TAG, "constructor slot_id = %d, channel_id = %d", slot_id, channel_id);
    registerToHandleURC(urc, sizeof(urc) / sizeof(char*));
}

RmcCapabilitySwitchURCHandler::~RmcCapabilitySwitchURCHandler() {}

void RmcCapabilitySwitchURCHandler::onHandleUrc(const sp<RfxMclMessage>& msg) {
    if (RmcCapabilitySwitchUtil::isDssNoResetSupport() == false) {
        return;
    }
    if (strStartsWith(msg->getRawUrc()->getLine(), "+ESIMMAP:")) {
        handleSimSwitchUrc(msg, 0);
    } else if (strStartsWith(msg->getRawUrc()->getLine(), "+EPSMAP:")) {
        handleSimSwitchUrc(msg, 1);
    }
}

void RmcCapabilitySwitchURCHandler::handleSimSwitchUrc(const sp<RfxMclMessage>& msg, int version) {
    int err;
    int new_major_slot;
    int old_major_slot =
            getNonSlotMclStatusManager()->getIntValue(RFX_STATUS_KEY_MAIN_CAPABILITY_SLOT, 0);
    // parse urc info
    logI(RFX_LOG_TAG, "handleSimSwitchUrc begin, old_major_slot=%d, urc_count=%d", old_major_slot,
         s_urc_count);
    RfxAtLine* urc_line = msg->getRawUrc();
    urc_line->atTokStart(&err);
    if (err < 0) {
        logE(RFX_LOG_TAG, "handleSimSwitchUrc err %d", err);
        return;
    }
    new_major_slot = urc_line->atTokNextint(&err);
    if (err < 0) {
        logE(RFX_LOG_TAG, "handleSimSwitchUrc first int err %d %d", err, new_major_slot);
        return;
    }
    if (version == 0) {
        int i;
        for (i = 0; new_major_slot > 1; i++) {
            new_major_slot = new_major_slot >> 1;
        }
        new_major_slot = i;
    } else {
        new_major_slot = urc_line->atTokNextint(&err) - 1;
        if (err < 0) {
            logE(RFX_LOG_TAG, "handleSimSwitchUrc second int err %d %d", err, new_major_slot);
            return;
        }
    }
    s_switch_urc_channel_mutex.lock();
    if (checkIfSwitch()) {
        logI(RFX_LOG_TAG, "switch urc channels");
        RmcCapabilitySwitchRequestHandler::setSIMInitState(0);
        for (int i = RIL_URC; i < RfxChannelManager::getSupportChannels();
             i += RIL_CHANNEL_OFFSET) {
            lockReaderMutex(i);
        }
        RmcCapabilitySwitchRequestHandler::switchChannel(RIL_URC, old_major_slot, new_major_slot);
        getNonSlotMclStatusManager()->setBoolValue(RFX_STATUS_KEY_CAPABILITY_SWITCH_URC_CHANNEL,
                                                   false);
        for (int i = RIL_CMD_11; i < RfxChannelManager::getSupportChannels();
             i += RIL_CHANNEL_OFFSET) {
            RmcCapabilitySwitchRequestHandler::lockRestartMutex(i);
        }
        RmcCapabilitySwitchRequestHandler::switchChannel(RIL_CMD_11, old_major_slot,
                                                         new_major_slot);
        for (int i = RIL_CMD_11; i < RfxChannelManager::getSupportChannels();
             i += RIL_CHANNEL_OFFSET) {
            RmcCapabilitySwitchRequestHandler::unlockRestartMutex(i);
        }
        for (int i = RIL_URC; i < RfxChannelManager::getSupportChannels();
             i += RIL_CHANNEL_OFFSET) {
            unlockReaderMutex(i);
        }
    }
    s_switch_urc_channel_mutex.unlock();
}

void RmcCapabilitySwitchURCHandler::lockReaderMutex(int channel_id) {
    RfxChannel* p_channel;
    RfxChannelContext* p_channel_context;

    p_channel = RfxChannelManager::getChannel(channel_id);
    p_channel_context = p_channel->getContext();
    p_channel_context->m_readerMutex.lock();
}

void RmcCapabilitySwitchURCHandler::unlockReaderMutex(int channel_id) {
    RfxChannel* p_channel;
    RfxChannelContext* p_channel_context;

    p_channel = RfxChannelManager::getChannel(channel_id);
    p_channel_context = p_channel->getContext();
    p_channel_context->m_readerMutex.unlock();
}

bool RmcCapabilitySwitchURCHandler::checkIfSwitch() {
    bool ret = getNonSlotMclStatusManager()->getBoolValue(
            RFX_STATUS_KEY_CAPABILITY_SWITCH_URC_CHANNEL, false);
    if (ret) {
        s_urc_count++;
        if (RfxRilUtils::rfxGetSimCount() == s_urc_count) {
            s_urc_count = 0;
            return true;
        }
    }
    return false;
}
