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

#ifndef __RTC_IMS_SMS_CONTROLLER_H__
#define __RTC_IMS_SMS_CONTROLLER_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "RfxController.h"
#include "RtcGsmSmsController.h"
#include "RtcCdmaSmsController.h"
#include "RtcConcatSms.h"
#include "RtcSmsMessage.h"
#include "utils/Vector.h"

/*****************************************************************************
 * Class RpSimController
 *****************************************************************************/

class RtcImsSmsController : public RfxController {
    RFX_DECLARE_CLASS(RtcImsSmsController);

  public:
    RtcImsSmsController();
    virtual ~RtcImsSmsController();

  public:
    bool removeReferenceIdCached(int ref);
    void addReferenceId(int ref);
    int getCacheSize();
    RtcConCatSmsRoot* getConCatSmsRoot() { return mConCatSmsRoot; }
    void sendCdmaSms(RtcCdmaSmsMessage* msg);
    void sendGsmSms(RtcGsmSmsMessage* msg);
    void sendCdmaSmsAck(const sp<RfxMessage>& message);
    void sendGsmSmsAck(int success, int cause, const sp<RfxMessage>& message);
    bool isSupportSmsFormatConvert();

    // Override
  protected:
    virtual void onInit();
    virtual bool onHandleRequest(const sp<RfxMessage>& message);
    virtual bool onCheckIfRejectMessage(const sp<RfxMessage>& message, bool isModemPowerOff,
                                        int radioState);
    virtual bool onPreviewMessage(const sp<RfxMessage>& message);
    virtual bool onCheckIfResumeMessage(const sp<RfxMessage>& message);

  private:
    void setTag(String8 tag) { mTag = tag; }
    bool hasImsCall();
    bool isCdmaRat();
    bool isInCall();
    bool isInCdmaCall();

  private:
    String8 mTag;
    RtcGsmSmsController* mGsmCtrl;
    RtcCdmaSmsController* mCdmaCtrl;
    RtcConCatSmsRoot* mConCatSmsRoot;
    Vector<int> mImsSmsRefIdVector;  // only store ims sms reference id
};
#endif /* __RTC_IMS_SMS_CONTROLLER_H__ */
