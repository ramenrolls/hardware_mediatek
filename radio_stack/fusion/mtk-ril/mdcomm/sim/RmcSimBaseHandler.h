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

#ifndef __RMC_SIM_BASE_HANDLER_H__
#define __RMC_SIM_BASE_HANDLER_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "RfxBaseHandler.h"
#include "RmcCommSimDefs.h"
#include "ratconfig.h"

#define SINGLE_MODE_SIM_CARD 10
#define SINGLE_MODE_USIM_CARD 20
#define SINGLE_MODE_RUIM_CARD 30
#define DUAL_MODE_CG_CARD 40
#define DUAL_MODE_UG_CARD 50
#define CT_NATIONAL_ROAMING_CARD 41
#define CU_DUAL_MODE_CARD 42
#define DUAL_MODE_TELECOM_LTE_CARD 43
#define UNKNOWN_CARD -1

#define MAX_RETRY_COUNT 30

/*****************************************************************************
 * Class RpSimController
 *****************************************************************************/

class RmcSimBaseHandler : public RfxBaseHandler {
    RFX_DECLARE_HANDLER_CLASS(RmcSimBaseHandler);

  protected:
    enum SIM_HANDLE_RESULT {
        RESULT_NEED,
        RESULT_IGNORE,

        RESULT_END
    };

    String8 mTag;

  public:
    RmcSimBaseHandler(int slot_id, int channel_id);
    virtual ~RmcSimBaseHandler();

    // Check if the handler have to process the Request or not
    virtual SIM_HANDLE_RESULT needHandle(const sp<RfxMclMessage>& msg);

    // Process URC here
    virtual void handleUrc(const sp<RfxMclMessage>& msg, RfxAtLine* urc);

    // Process Request here
    virtual void handleRequest(const sp<RfxMclMessage>& msg);

    virtual void handleEvent(const sp<RfxMclMessage>& msg) { RFX_UNUSED(msg); }

    // Return the string of request or event
    virtual char* handleIdToString(int id) {
        RFX_UNUSED(id);
        return NULL;
    }

    // Return the request list which you want to reqister
    virtual const int* queryTable(int channel_id, int* record_num);

    virtual const char** queryUrcTable(int* record_num);

    // Return the event list which you want to reqister
    virtual const int* queryEventTable(int channel_id, int* record_num) {
        RFX_UNUSED(channel_id);
        RFX_UNUSED(record_num);
        return NULL;
    }

    // Override
  protected:
    pthread_mutex_t simStatusMutex = PTHREAD_MUTEX_INITIALIZER;

    int queryAppTypeId(String8 aid);

    void setTag(String8 s);
    bool bIsTc1();
    bool isCommontSlotSupport();
    bool isSimInserted();
    bool isSimSlotLockSupport();
    bool getIccId(char* value);
    RmcSimPinPukCount* getPinPukRetryCount(void);
    void setPinPukRetryCountProp(RmcSimPinPukCount* retry);
    RmcSimSlotLockDeviceLockInfo* getSimSlotLockDeviceLockInfo(void);
    void setSimSlotLockDeviceLockInfoProp(RmcSimSlotLockDeviceLockInfo* retry);
    UICC_Status getSimStatus();
    bool isOP09AProject();
    // Convert all the lower case characters in string to upper case
    char* stringToUpper(char* str);
    void sendSimStatusChanged();
    bool isAOSPPropSupport();
    bool isSimIoFcp(char* response);
    void handleCdmaCardType(const char* iccid);
    bool isOp09Card(const char* iccid);
    int isApplicationIdExist(const char* aid);
    void makeSimRspFromUsimFcp(unsigned char** simResponse);
    void resetSimPropertyAndStatusKey();
    RIL_SimSlotStatus** decodeEslotsinfo(RfxAtLine* line, int* decodeerr, int* slotnum);
};
#endif /* __RMC_SIM_BASE_HANDLER_H__ */
