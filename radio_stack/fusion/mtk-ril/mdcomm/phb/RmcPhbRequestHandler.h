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

#ifndef __RMC_PHB_REQUEST_HANDLER_H__
#define __RMC_PHB_REQUEST_HANDLER_H__

#include "RfxBaseHandler.h"
#include <telephony/mtk_ril.h>
#include "RfxPhbEntriesData.h"
#include "RfxPhbEntryExtData.h"
#include "RfxPhbMemStorageData.h"

#define RFX_LOG_TAG "RmcPhbReq"

typedef enum {
    CPBW_ENCODE_IRA,
    CPBW_ENCODE_UCS2,
    CPBW_ENCODE_UCS2_81,
    CPBW_ENCODE_UCS2_82,
    CPBW_ENCODE_GSM7BIT,
    CPBW_ENCODE_MAX
} RilPhbCpbwEncode;

#define RIL_PHB_UCS2_81_MASK 0x7f80

#define RIL_MAX_PHB_NAME_LEN 40  // Max # of characters in the NAME
#define RIL_MAX_PHB_EMAIL_LEN 60
#define RIL_MAX_PHB_ENTRY 10

class RmcPhbRequestHandler : public RfxBaseHandler {
    RFX_DECLARE_HANDLER_CLASS(RmcPhbRequestHandler);

  public:
    RmcPhbRequestHandler(int slot_id, int channel_id);
    virtual ~RmcPhbRequestHandler();

  protected:
    virtual void onHandleRequest(const sp<RfxMclMessage>& msg);

    virtual void onHandleUrc(const sp<RfxMclMessage>& msg) { RFX_UNUSED(msg); }

    virtual void onHandleEvent(const sp<RfxMclMessage>& msg);

  private:
    int ril_max_phb_name_len = 40;
    int maxGrpNum = -1;
    int maxAnrNum = -1;
    int maxEmailNum = -1;
    int mIsUserLoad = -1;
    // int current_phb_storage[4] = {-1, -1, -1, -1};
    int current_phb_storage = -1;
    int selectPhbStorage(int type);
    char* getPhbStorageString(int type);
    void resetPhbStorage();
    void requestResetPhbStorage(const sp<RfxMclMessage>& msg);

    int bIsTc1();
    int hexCharToDecInt(char* hex, int length);
    int isMatchGsm7bit(char* src, int maxLen);
    int isGsm7bitExtension(char* src, int maxLen);
    int encodeUCS2_0x81(char* src, char* des, int maxLen);
    int encodeUCS2_0x82(char* src, char* des, int maxLen);
    char* ascii2UCS2(char* input);
    void requestQueryPhbInfo(const sp<RfxMclMessage>& msg);
    void requestClearPhbEntry(int index);
    void requestWritePhbEntry(const sp<RfxMclMessage>& msg);
    void requestReadPhbEntry(const sp<RfxMclMessage>& msg);
    void requestQueryUPBCapability(const sp<RfxMclMessage>& msg);
    void requestEditUPBEntry(const sp<RfxMclMessage>& msg);
    void requestDeleteUPBEntry(const sp<RfxMclMessage>& msg);
    void requestReadGasList(const sp<RfxMclMessage>& msg);
    void requestReadUpbGrpEntry(const sp<RfxMclMessage>& msg);
    void requestWriteUpbGrpEntry(const sp<RfxMclMessage>& msg);
    void requestGetPhoneBookStringsLength(const sp<RfxMclMessage>& msg);
    void requestGetPhoneBookMemStorage(const sp<RfxMclMessage>& msg);
    void requestSetPhoneBookMemStorage(const sp<RfxMclMessage>& msg);
    void loadUPBCapability();
    void requestReadPhoneBookEntryExt(const sp<RfxMclMessage>& msg);
    void requestWritePhoneBookEntryExt(const sp<RfxMclMessage>& msg);
    void requestQueryUPBAvailable(const sp<RfxMclMessage>& msg);
    void requestReadUPBEmail(const sp<RfxMclMessage>& msg);
    void requestReadUPBSne(const sp<RfxMclMessage>& msg);
    void requestReadUPBAnr(const sp<RfxMclMessage>& msg);
    void requestReadAasList(const sp<RfxMclMessage>& msg);
    void requestSetPhonebookReady(const sp<RfxMclMessage>& msg);
};

#endif
