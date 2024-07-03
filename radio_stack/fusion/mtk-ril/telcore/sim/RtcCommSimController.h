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

#ifndef __RTC_COMM_SIM_CONTROLLER_H__
#define __RTC_COMM_SIM_CONTROLLER_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "RfxController.h"

/*****************************************************************************
 * typedef
 *****************************************************************************/

#define SIM_APP_COUNT 5
#define SIM_CACHED_FILE_COUNT 10
#define SIM_FILE_PATH_LEN 16

typedef struct {
    bool valid;
    int sw1;
    int sw2;
    char* simResponse;
} RIL_SIM_IO_Response_Data;

typedef struct {
    int fileid;
    char* path;
    RIL_SIM_IO_Response_Data get_rsp;
    RIL_SIM_IO_Response_Data read_binary;
} RIL_SIM_IO_Cache_Response;

typedef struct {
    int fileid;
    char path[SIM_FILE_PATH_LEN];
} RIL_SIM_File_Info;

typedef enum {
    UICC_APP_ISIM = 0,
    UICC_APP_USIM = 1,
    UICC_APP_CSIM = 2,
    UICC_APP_SIM = 3,
    UICC_APP_RUIM = 4,
    UICC_APP_ID_END
} App_Id;

static const RIL_SIM_File_Info RIL_SIM_CACHE_FILES[SIM_APP_COUNT][SIM_CACHED_FILE_COUNT] = {
        // ISIM
        {{0, "\0"},
         {0, "\0"},
         {0, "\0"},
         {0, "\0"},
         {0, "\0"},
         {0, "\0"},
         {0, "\0"},
         {0, "\0"},
         {0, "\0"},
         {0, "\0"}},
        // USIM
        {{0x2FE2, "3F00"},
         {0x6F07, "3F007FFF"},
         {0x6FAD, "3F007FFF"},
         {0x6F3E, "3F007FFF"},
         {0x6F3F, "3F007FFF"},
         {0x6F38, "3F007FFF"},
         {0x6F46, "3F007FFF"},
         {0x6F14, "3F007FFF"},
         {0x6F18, "3F007FFF"},
         {0x6F02, "3F007F43"}},
        // CSIM
        {{0, "\0"},
         {0, "\0"},
         {0, "\0"},
         {0, "\0"},
         {0, "\0"},
         {0, "\0"},
         {0, "\0"},
         {0, "\0"},
         {0, "\0"},
         {0, "\0"}},
        // SIM
        {{0x2FE2, "3F00"},
         {0x6F07, "3F007F20"},
         {0x6FAD, "3F007F20"},
         {0x6F3E, "3F007F20"},
         {0x6F3F, "3F007F20"},
         {0x6F38, "3F007F20"},
         {0x6F46, "3F007F20"},
         {0x6F14, "3F007F20"},
         {0x6F18, "3F007F20"},
         {0x6F02, "3F007F43"}},
        // RUIM
        {{0, "\0"},
         {0, "\0"},
         {0, "\0"},
         {0, "\0"},
         {0, "\0"},
         {0, "\0"},
         {0, "\0"},
         {0, "\0"},
         {0, "\0"},
         {0, "\0"}}};

// SIM on/off state.
#define SIM_POWER_STATE_SIM_OFF 10

/*****************************************************************************
 * Class RtcCommSimController
 *****************************************************************************/

class RtcCommSimController : public RfxController {
    RFX_DECLARE_CLASS(RtcCommSimController);

  public:
    RtcCommSimController();
    virtual ~RtcCommSimController();

    // Override
  protected:
    virtual void onInit();

    virtual bool onCheckIfRejectMessage(const sp<RfxMessage>& message, bool isModemPowerOff,
                                        int radioState);

    virtual bool onHandleRequest(const sp<RfxMessage>& message);
    virtual bool onHandleUrc(const sp<RfxMessage>& message);
    virtual bool onHandleResponse(const sp<RfxMessage>& message);

  private:
    void setTag(String8 tag) { mTag = tag; }

    void onRadioStateChanged(RfxStatusKeyEnum key, RfxVariant old_value, RfxVariant value);

    void onConnectionStateChanged(RfxStatusKeyEnum key, RfxVariant old_value, RfxVariant value);

    void onModeSwitchFinished(RfxStatusKeyEnum key, RfxVariant old_value, RfxVariant value);

    bool onHandleSimIoRequest(const sp<RfxMessage>& msg);

    void handleCallBarring(const sp<RfxMessage>& msg);

    bool isCallBarringUsage(const sp<RfxMessage>& msg);

    void handleGetIccid(const sp<RfxMessage>& msg);

    void handleGetImsi(const sp<RfxMessage>& ms);

    void handleSimIo(const sp<RfxMessage>& ms);

    void onSimFileChanged(RfxStatusKeyEnum key, RfxVariant old_value, RfxVariant value);

    void handleGetCurrentUiccCardProcisioningStatus(const sp<RfxMessage>& msg);

    void onSimResetChanged(RfxStatusKeyEnum key, RfxVariant old_value, RfxVariant value);

    void resetSimParameters();

  private:
    String8 mTag;
    RIL_SIM_IO_Cache_Response mIoResponse[SIM_APP_COUNT][SIM_CACHED_FILE_COUNT];
    sp<RfxMessage> mCacheSmlMsg;
};
#endif /* __RTC_COMM_SIM_CONTROLLER_H__ */
