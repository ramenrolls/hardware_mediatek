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

#ifndef __RMC_CAT_URC_HANDLER_H__
#define __RMC_CAT_URC_HANDLER_H__

#define LOG_TAG "RmcCatUrcHandler"
#define NUM_STK_CALL_CTRL 3
#define EVENT_NOTIFY_BUFFER_LEN 10

#include "RfxBaseHandler.h"
#include "RfxCdmaInfoRecData.h"
#include "RfxStringData.h"
#include "RfxStringsData.h"
#include "RfxIntsData.h"
#include "RfxVoidData.h"
#include "RfxSimRefreshData.h"
#include "RfxMessageId.h"
#include "telephony/ril.h"

typedef enum {
    CMD_REFRESH = 0x01,
    CMD_MORE_TIME = 0x02,
    CMD_POLL_INTERVAL = 0x03,
    CMD_POLLING_OFF = 0x04,
    CMD_SETUP_EVENT_LIST = 0x05,
    CMD_SETUP_CALL = 0x10,
    CMD_SEND_SS = 0x11,
    CMD_SEND_USSD = 0x12,
    CMD_SEND_SMS = 0x13,
    CMD_DTMF = 0x14,
    CMD_LAUNCH_BROWSER = 0x15,
    CMD_PLAY_TONE = 0x20,
    CMD_DSPL_TXT = 0x21,
    CMD_GET_INKEY = 0x22,
    CMD_GET_INPUT = 0x23,
    CMD_SELECT_ITEM = 0x24,
    CMD_SETUP_MENU = 0x25,
    CMD_PROVIDE_LOCAL_INFO = 0x26,
    CMD_TIMER_MANAGER = 0x27,
    CMD_IDLE_MODEL_TXT = 0x28,
    CMD_PERFORM_CARD_APDU = 0x30,
    CMD_POWER_ON_CARD = 0x31,
    CMD_POWER_OFF_CARD = 0x32,
    CMD_GET_READER_STATUS = 0x33,
    CMD_RUN_AT = 0x34,
    CMD_LANGUAGE_NOTIFY = 0x35,
    CMD_OPEN_CHAN = 0x40,
    CMD_CLOSE_CHAN = 0x41,
    CMD_RECEIVE_DATA = 0x42,
    CMD_SEND_DATA = 0x43,
    CMD_GET_CHAN_STATUS = 0x44,
    CMD_RFU = 0x60,
    CMD_END_PROACTIVE_SESSION = 0x81,
    CMD_DETAIL = 0xFF
} sat_proactive_cmd_enum;

typedef enum {
    CMD_TYPE_PROACTIVE = 0x00,
    CMD_TYPE_NOTIFY = 0x01,
    CMD_YPE_SESSIONEND = 0x02
} sat_cmd_type_num;

typedef struct {
    RIL_SimRefreshResponse_v7* simRefreshRsp;
    char* efId;
} SimRefreshResponse;

class RmcCatUrcHandler : public RfxBaseHandler {
    RFX_DECLARE_HANDLER_CLASS(RmcCatUrcHandler);

  public:
    RmcCatUrcHandler(int slot_id, int channel_id);
    virtual ~RmcCatUrcHandler();

  protected:
    virtual void onHandleRequest(const sp<RfxMclMessage>& msg) { RFX_UNUSED(msg); }

    virtual void onHandleUrc(const sp<RfxMclMessage>& msg);

    virtual void onHandleTimer();
    virtual void onHandleEvent(const sp<RfxMclMessage>& msg);

  private:
    void handleBipEventNotify(const sp<RfxMclMessage>& msg);
    void handleReportCpinState(const sp<RfxMclMessage>& msg);
    void handleStkProactiveCommand(const sp<RfxMclMessage>& msg);
    void handleStkEventNotify(const sp<RfxMclMessage>& msg);
    void handleStkSessionEnd(const sp<RfxMclMessage>& msg);
    void handleStkCallControl(const sp<RfxMclMessage>& msg);
    bool onCheckIfRejectMessage(const sp<RfxMclMessage>& msg, RIL_RadioState radioState);
};

#endif
