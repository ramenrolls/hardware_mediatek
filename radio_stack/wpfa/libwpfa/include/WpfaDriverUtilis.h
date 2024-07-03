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

#ifndef WPAF_DRIVER_UTILIS_H
#define WPAF_DRIVER_UTILIS_H

#include <termios.h>
#include "Mutex.h"

#include <mtk_log.h>

#include "WpfaDriverMessage.h"
#include "WpfaCcciReader.h"
#include "WpfaCcciSender.h"

using ::android::Mutex;

/*
 * =============================================================================
 *                     Defines
 * =============================================================================
 */
#define WPFA_DRIVER_TEST_MODE_ENABLE (0)

#define TEST_EVENT_NONE (0)
#define TEST_EVENT_PKT_SEND_TO_KERNEL_SUCCESS (1)

#define FAKE_MD_TID_START (100)
#define FAKE_MD_TID_END (500)

class WpfaDriverUtilis {
  public:
    WpfaDriverUtilis();
    virtual ~WpfaDriverUtilis();
    static WpfaDriverUtilis* getInstance();
    void init();

    void setCcciReader(WpfaCcciReader* reader) { mReader = reader; }

    void setCcciSender(WpfaCcciSender* sender) { mSender = sender; }

    void notifyEvent(int eventId) { mEventId = eventId; }

    void testStartNormal(const sp<WpfaDriverMessage>& msg);

    // int sendMsgToControlMsgDispatcher(const sp<WpfaDriverMessage>& message);
    // int sendMsgToFilterRuleReqHandler(const sp<WpfaDriverMessage>& message);
    // int sendMsgToShmReadMsgHandler(const sp<WpfaDriverMessage>& message);
    // int sendMsgToShmWriteMsgHandler(const sp<WpfaDriverMessage>& message);

  private:
    void fake_M2A_WPFA_VERSION(const sp<WpfaDriverMessage>& msg);
    void fake_M2A_REG_DL_FILTER(int testingFilterStrId);
    void fake_M2A_DEREG_DL_FILTER(uint32_t filterId);
    void fake_M2A_UL_IP_PKT(int IpPktId);

    uint16_t fake_MD_Tid();

    /**
     * singleton pattern
     */
    static WpfaDriverUtilis* sInstance;
    static Mutex sWpfaDriverUtilisInitMutex;

    WpfaCcciReader* mReader;
    WpfaCcciSender* mSender;

    uint16_t mFakeMdTid;
    int mEventId;
};

#endif /* end of WPAF_DRIVER_UTILIS_H */
