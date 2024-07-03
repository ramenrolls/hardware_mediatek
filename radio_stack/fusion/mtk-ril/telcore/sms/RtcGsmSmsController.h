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

#ifndef __RTC_GSM_SMS_CONTROLLER_H__
#define __RTC_GSM_SMS_CONTROLLER_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "RfxController.h"
#include "RfxTimer.h"
#include "RtcSmsMessage.h"
/*****************************************************************************
 * Class RtcGsmSmsController
 *****************************************************************************/

class RtcGsmSmsController : public RfxController {
    RFX_DECLARE_CLASS(RtcGsmSmsController);

  public:
    RtcGsmSmsController();
    virtual ~RtcGsmSmsController();

    // Override
  protected:
    virtual void onInit();
    virtual bool onHandleResponse(const sp<RfxMessage>& message);
    virtual bool onCheckIfRejectMessage(const sp<RfxMessage>& message, bool isModemPowerOff,
                                        int radioState);
    virtual bool onPreviewMessage(const sp<RfxMessage>& message);
    virtual bool onCheckIfResumeMessage(const sp<RfxMessage>& message);
    virtual bool onHandleUrc(const sp<RfxMessage>& message);
    virtual bool onHandleRequest(const sp<RfxMessage>& message);

  public:
    void handleRequest(const sp<RfxMessage>& message);
    bool previewMessage(const sp<RfxMessage>& message);
    bool checkIfResumeMessage(const sp<RfxMessage>& message);
    void sendGsmSms(RtcGsmSmsMessage* msg);
    void sendGsmSmsAck(int success, int cause, const sp<RfxMessage>& message);

  private:
    void setTag(String8 tag) { mTag = tag; }
    const char* boolToString(bool value);
    void onHidlStateChanged(RfxStatusKeyEnum key, RfxVariant old_value, RfxVariant value);
    void delaySetSmsFwkReady();
    int getReferenceIdFromCDS(char* hex);
    int smsHexCharToDecInt(char* hex, int length);
    void handleNewSms(const sp<RfxMessage>& message);
    void handNewSmsAck(const sp<RfxMessage>& message);
    bool isSupportSmsFormatConvert();
    bool isUnderCryptKeeper();
    bool isCdmaPhoneMode();

  private:
    String8 mTag;
    bool mSmsFwkReady;
    bool mSmsMdReady;
    TimerHandle mSmsTimerHandle;
    bool mSmsSending;
    bool mNeedStatusReport;  // only for ims sms

#define GSM_SMS_MESSAGE_STRS_COUNT 2        // 0: smsc, 1: pdu
#define DELAY_SET_SMS_FWK_READY_TIMER 5000  // 5s
};
#endif /* __RTC_GSM_SMS_CONTROLLER_H__ */
