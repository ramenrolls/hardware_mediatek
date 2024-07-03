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

#ifndef __RFX_IMS_EVENT_PACKAGE_CONTROLLER_H__
#define __RFX_IMS_EVENT_PACKAGE_CONTROLLER_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include <map>
#include <vector>
#include <string>
#include <memory>

#include "RfxController.h"
#include "RtcImsConferenceHandler.h"
#include "RtcImsDialogHandler.h"
//#include "utils/String8.h"
//#include "utils/Vector.h"

// using ::android::String8;
// using ::android::Vector;
using ::android::sp;

using namespace std;
/*****************************************************************************
 * Class RfxController
 *****************************************************************************/

class RfxStringsData;

class RtcImsConferenceController : public RfxController {
    // Required: declare this class
    RFX_DECLARE_CLASS(RtcImsConferenceController);

  public:
    RtcImsConferenceController();
    virtual ~RtcImsConferenceController();

    // Override
  protected:
    virtual void onInit();
    virtual void onDeinit();
    virtual bool onHandleRequest(const sp<RfxMessage>& message);
    virtual bool onHandleUrc(const sp<RfxMessage>& message);
    virtual bool onHandleResponse(const sp<RfxMessage>& message);
    virtual bool onCheckIfRejectMessage(const sp<RfxMessage>& message, bool isModemPowerOff,
                                        int radioState);
    void handleAddMember(const sp<RfxMessage>& message);
    void handleRemoveMember(const sp<RfxMessage>& message);
    void handleMergeConferenece(const sp<RfxMessage>& message);
    void handleImsCallInfoUpdate(const sp<RfxMessage>& message);
    void handleEconfUpdate(const sp<RfxMessage>& message);
    bool handleAddMemberResponse(const sp<RfxMessage>& response);
    void handleRemoveMemberResponse(const sp<RfxMessage>& response);
    void modifyParticipantComplete();
    void tryhandleCachedCEP();
    bool tryhandleAddMemberByMerge(const sp<RfxMessage>& response);

  public:
    bool handleMessgae(const sp<RfxMessage>& message);
    bool handleResponse(const sp<RfxMessage>& message);
    bool handleUrc(const sp<RfxMessage>& message);
    void handleOneKeyConference(const sp<RfxMessage>& message);
    void handleOneKeyConferenceFail(const sp<RfxMessage>& message);
    void onParticipantsUpdate(bool autoTerminate);
    void handleConferenceStart();

    // Fake disconnect related interface
    bool needProcessFakeDisconnect();
    void processFakeDisconnectDone();
    bool needSkipDueToFakeDisconnect(int callid);

  private:
    void handleImsEventPackageIndication(const sp<RfxMessage>& msg);
    void onTimeout();
    void handleSpecificConferenceMode();

  protected:
    RtcImsConferenceHandler* mRtcImsConferenceHandler = NULL;
    RtcImsDialogHandler* mRtcImsDialogHandler = NULL;

  private:
    bool mNormalCallsMerge;
    bool mInviteByNumber;
    bool mIsMerging;
    int mEconfCount;
    TimerHandle mTimeoutHandle;

    bool mIsAddingMember = false;
    sp<RfxMessage> mOriginalMergeMessage = NULL;
    bool mIsRemovingMember = false;
    sp<RfxMessage> mCachedCEPMessage = NULL;

    int mFakeDisconnectedCallProcessingCount;
    Vector<int> mFakeDisconnectedCallIdList;
    int mActiveCallIdBeforeMerge = 0;
};

#endif /* __RFX_IMS_EVENT_PACKAGE_CONTROLLER_H__ */
