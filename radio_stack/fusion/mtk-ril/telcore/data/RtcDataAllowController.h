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

#ifndef __RTC_DATA_ALLOW_CONTROLLER_H__
#define __RTC_DATA_ALLOW_CONTROLLER_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include <map>
#include <utils/Vector.h>
#include "RfxController.h"
#include "RfxTimer.h"
#include "RfxMessageId.h"
#include "RfxDataMessageId.h"
#include "RfxIntsData.h"
#include "RfxVoidData.h"

#include <string.h>
#include <string>
#include <vector>

using namespace std;

/*****************************************************************************
 * Class RpDataAllowController
 *****************************************************************************/
// Mapping to err cause: 4112, 4117 (multiple PS allow error)
#define RIL_E_OEM_MULTI_ALLOW_ERR RIL_E_OEM_ERROR_1

#define INVAILD_ID (-1)
#define ALLOW_DATA (1)
#define DISALLOW_DATA (0)

// For dequeueNetworkRequest return value
#define FINISH_ALL_REQUEST true
#define WAIT_NEXT_REQUEST false

class RtcDataAllowController : public RfxController {
    RFX_DECLARE_CLASS(RtcDataAllowController);  // Required: declare this class

    struct onDemandRequest {
        int type;
        int slotId;
    };

  public:
    RtcDataAllowController();
    virtual void enqueueNetworkRequest(int r_id, int slotId);
    virtual bool dequeueNetworkRequest(int r_id, int slotId);
    virtual void resendAllowData();

    virtual ~RtcDataAllowController();

    // Override
  protected:
    virtual void onInit();
    virtual void onDeinit();
    virtual bool onHandleRequest(const sp<RfxMessage>& message);
    virtual bool onHandleResponse(const sp<RfxMessage>& message);
    virtual bool onPreviewMessage(const sp<RfxMessage>& message);
    virtual bool onCheckIfResumeMessage(const sp<RfxMessage>& message);
    virtual void handleSetDataAllowRequest(const sp<RfxMessage>& request);
    virtual void handleSetDataAllowResponse(const sp<RfxMessage>& response);
    virtual void handleMultiAllowError(int activePhoneId);
    virtual bool onCheckIfRejectMessage(const sp<RfxMessage>& message, bool isModemPowerOff,
                                        int radioState);
    virtual void handleDataConnectionAttachRequest(const sp<RfxMessage>& message);
    virtual void handleDataConnectionDetachRequest(const sp<RfxMessage>& message);
    virtual void onAttachOrDetachDone(const sp<RfxMessage> message);
    virtual void onSetDataAllow(int slotId);
    virtual int checkRequestExistInQueue(int type, int slotId);
    virtual int checkTypeExistInQueue(int type);
    virtual void onPreferredChanged(RfxStatusKeyEnum key, RfxVariant old_value, RfxVariant value);
    virtual void onHidlStateChanged(RfxStatusKeyEnum key, RfxVariant old_value, RfxVariant value);
    virtual void onSimMeLockChanged(int slotId, RfxStatusKeyEnum key, RfxVariant old_value,
                                    RfxVariant value);

  private:
    // Private functions
    bool isNeedSuspendRequest(const sp<RfxMessage>& message);
    bool checkDisallowingPeer();
    bool preprocessRequest(const sp<RfxMessage>& request);
    bool isPreferredDataMode();
    void setAllowDataSlot(bool allow, int slot);
    int getAllowDataSlot();

  private:
    // Private members
    bool mDoingDataAllow;
    bool mReqDataAllow;
    int mDisallowingPeer;
    sp<RfxMessage> mLastAllowTrueRequest;
    Vector<int> nonDdsRequestQueue;
    int mIsPreferredDataMode;
    vector<struct onDemandRequest> mOnDemandQueue;
    int isMdSelfEdallow;
};
#endif
