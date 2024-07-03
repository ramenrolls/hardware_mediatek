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

#ifndef __RMC_GSM_SMS_URC_HANDLER_H__
#define __RMC_GSM_SMS_URC_HANDLER_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "RmcGsmSmsBaseHandler.h"

/*****************************************************************************
 * Class RpSimController
 *****************************************************************************/

class RmcGsmSmsUrcHandler : public RmcGsmSmsBaseHandler {
    RFX_DECLARE_HANDLER_CLASS(RmcGsmSmsUrcHandler);

  public:
    RmcGsmSmsUrcHandler(int slot_id, int channel_id);

  protected:
    // Process URC here
    virtual void onHandleUrc(const sp<RfxMclMessage>& msg);

  private:
    void setTag(String8 tag) { mTag = tag; }

    // Utility functions
    void onNewSms(const sp<RfxMclMessage>& msg);
    void onNewSmsStatusReport(const sp<RfxMclMessage>& msg);

    // URC handler
    void handleNewSms(const sp<RfxMclMessage>& msg);
    // FIXME: Remove handleNewImsSms after modem handle all IMS cases
    void handleNewImsSms(const sp<RfxMclMessage>& msg);
    void handleNewSmsStatusReport(const sp<RfxMclMessage>& msg);
    // FIXME: Remove handleNewImsSmsStatusReport after modem handle all IMS cases
    void handleNewImsSmsStatusReport(const sp<RfxMclMessage>& msg);
    void handleNewSmsOnSim(const sp<RfxMclMessage>& msg);
    void handleSimSmsStorageStatus(const sp<RfxMclMessage>& msg);
    void handleNewBroadcastSms(const sp<RfxMclMessage>& msg);
    void handleNewEtwsNotification(const sp<RfxMclMessage>& msg);

  private:
    String8 mTag;
};
#endif /* __RMC_GSM_SMS_URC_HANDLER_H__ */
