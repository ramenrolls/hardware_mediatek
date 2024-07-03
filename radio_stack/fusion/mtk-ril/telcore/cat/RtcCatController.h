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

#ifndef __RFX_CAT_CONTROLLER_H__
#define __RFX_CAT_CONTROLLER_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "RfxController.h"
#include "RfxTimer.h"
#include "RfxVoidData.h"
#include "RfxIntsData.h"
#include "RfxStringData.h"
#include "RfxStringsData.h"
#include "RfxSimIoRspData.h"
#include "RfxSimRefreshData.h"
#include "RfxMessageId.h"
#include "rfx_properties.h"
#include "RfxRawData.h"
/*****************************************************************************
 * Enum
 *****************************************************************************/
// Proactive Command Type
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
} CatProactiveCmdEnum;
typedef enum {
    CMD_TYPE_PROACTIVE = 0x00,
    CMD_TYPE_NOTIFY = 0x01,
    CMD_YPE_SESSIONEND = 0x02
} sat_cmd_type_num;

// Event Download Command Type
typedef enum {
    EVENT_MT_CALL = 0x00,
    EVENT_CALL_CONNECTED = 0x01,
    EVENT_CALL_DISCONNECTED = 0x02,
    EVENT_LOCATION_STATUS = 0x03,
    EVENT_ACCESS_TECHNOLOGY_CHANGE = 0x0B,
    EVENT_NETWORK_SEARCH_MODE_CHANGE = 0x0E,
    EVENT_NETWORK_REJECTION = 0x12
} sat_event_download_cmd_enum;
/*****************************************************************************
 * Define
 *****************************************************************************/
// Invalid value
#define INVALID_VALUE -1

// Envelpoe Command Type
#define MENU_SELECTION_TAG 0xD3
#define EVENT_DOWNLOAD_TAG 0xD6

// Event notify buffer size
#define EVENT_NOTIFY_BUFFER_LEN 10
#define STK_REFRESH_DELAY_TIME 200
/*****************************************************************************
 * typedef
 *****************************************************************************/

typedef struct {
    int cmdType;
    int cmdNum;
    int cmdQualifier;
    // int source;
    bool needResponse;
} ProactiveCmdRecord;

typedef struct {
    RIL_SimRefreshResponse_v7* simRefreshRsp;
    char* efId;
} SimRefreshResponse;
/*****************************************************************************
 * Class RfxController
 *****************************************************************************/

class RtcCatController : public RfxController {
    // Required: declare this class
    RFX_DECLARE_CLASS(RtcCatController);

  public:
    RtcCatController();
    virtual ~RtcCatController();

    // Override
  protected:
    virtual void onInit();
    virtual bool onHandleRequest(const sp<RfxMessage>& message);
    virtual bool onHandleUrc(const sp<RfxMessage>& message);
    virtual bool onHandleResponse(const sp<RfxMessage>& message);
    virtual void onDeinit();
    virtual bool onCheckIfRejectMessage(const sp<RfxMessage>& message, bool isModemPowerOff,
                                        int radioState);

  public:
  private:
    void onCardTypeChanged(RfxStatusKeyEnum key, RfxVariant oldValue, RfxVariant newValue);
    void onCdma3GSwitchCard(RfxStatusKeyEnum key, RfxVariant oldValue, RfxVariant newValue);
    int checkEventNotifyFreeBuffer();
    int checkStkCmdDisplay(char* cmd_str);
    int decodeStkRefreshAid(char* cmd, char** paid);
    char* decodeStkRefreshFileChange(char* str, int** cmd, int* cmd_length);
    unsigned int findStkCallDuration(char* str);
    void freeAllStkQueuedEventNotify();
    void freeStkQueuedEventNotify(int index);
    void freeStkQueuedProactivCmd();
    void freeCachedMenu();
    int getStkCommandType(char* cmd);
    int getStkCommandNumber(char* cmd);
    char* getStkQueuedEventNotify(int index);
    int getStkResultCode(char* cmd);
    void handleSimRefresh(char* urc);
    void handleStkCommand(char* cmd, int cmdClass);
    void parseStkCmdQualifier(char* cmd, int* cmdQual);
    void parseStkCmdType(char* cmd, int* cmdType);
    void parseStkCmdType(const sp<RfxMessage>& message, int* cmdType);
    void parseStkCmdNum(char* cmd, int* cmdNum);
    void parseStkResultCode(const sp<RfxMessage>& message, int* resCode);
    void onRadioStateChanged(RfxStatusKeyEnum key, RfxVariant old_value, RfxVariant value);
    void handleEventNotify(const sp<RfxMessage>& message);
    void handleProactiveCommand(const sp<RfxMessage>& message);
    void handleQueryCpinResponse(const sp<RfxMessage>& message);
    void handleSimRefreshAfterQueryCpin();
    void requestHandleStkServiceIsRunning();
    int rild_sms_hexCharToDecInt(char* hex, int length);
    void setStkCachedEventNotify(char* cmd);
    void sendEventWithDelay(int id, int delayTime);
    void retryQueryCpinState();
    // flag:0-Stk Service running, 1-refresh, 2-others
    void sendStkQueuedCmd(int flag);
    void sendStkUssdConfirmInfo(const sp<RfxMessage>& message);
    void sendResponseForCardMissing(int cmdType, int cmdQual);
    void sendResponseForSpecial(int cmdType, int cmdQual);
    void resetProCmd();
    void parseStkEnvelopeCommandType(char* cmd, int* cmdType);
    bool isIgnoredEnvelopeCommand(const sp<RfxMessage>& message);

  private:
    bool isEventNotifyQueued;
    bool isProaCmdQueued;
    bool isStkServiceRunning;
    char* pEvent_notify[EVENT_NOTIFY_BUFFER_LEN];
    char* pProactive_cmd;
    SimRefreshResponse* pSimRefreshRspWithType;
    TimerHandle mTimeoutHandle;
    int mCPinRetry;
    int mCardType;
    ProactiveCmdRecord mProCmdRec;
    char* pCachedMenu;
};

#endif /* __RFX_CAT_CONTROLLER_H__ */
