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

#ifndef __SUPL_MSG_DISPATCHER_H__
#define __SUPL_MSG_DISPATCHER_H__
#define HAL_MNL_INTERFACE_VERSION 1

/*****************************************************************************
 * Include
 *****************************************************************************/
#include <string>
#include <list>
#include "RfxObject.h"
#include "SmsParserUtils.h"

#define SUPL_EXISTENCE_UNKNOWN 2
#define SUPL_EXISTENCE_YES 1
#define SUPL_EXISTENCE_NO 0

#define SUPL_EXISTENCE_MAX_DETECTION 3

#define HAL_MNL_BUFF_SIZE (16 * 1024)
#define HAL2MNL_NI_MESSAGE 401
#define HAL_MNL_INTERFACE_VERSION 1
#define MTK_HAL2MNL "mtk_hal2mnl"

class ConcentratedSms;
class SmsMessage;
class WappushMessage;

/*****************************************************************************
 * Class SuplMsgDispatcher
 *****************************************************************************/
class SuplMsgDispatcher : public RfxObject {
    // Required: declare this class
    RFX_DECLARE_CLASS(SuplMsgDispatcher);

  public:
    void dispatchSuplMsg(string content);
    bool doesSuplExist(void);

    // Override
  protected:
    virtual void onDeinit();

  private:
    bool addToExistingConcSms(SmsMessage* msg);
    ConcentratedSms* findConcSms(int ref);
    void onConcSmsTimeout(int ref);
    void hal2mnl_ni_message(char* msg, int len);
    void put_byte(char* buff, int* offset, const char input);
    void put_short(char* buff, int* offset, const short input);
    void put_int(char* buff, int* offset, const int input);
    void put_binary(char* buff, int* offset, const char* input, const int len);
    int safe_sendto(const char* path, const char* buff, int len);
    bool notifyConcMsg2Mnl(ConcentratedSms* msg);
    bool notifySms2Mnl(SmsMessage* msg);
    bool notifyWappush2Mnl(WappushMessage* msg);

  private:
    list<ConcentratedSms*> mConcSmsList;
    static int sSuplExistenceState;
    static int sSuplExistenceDetectionCount;
};

#endif /* __SUPL_MSG_DISPATCHER_H__ */
