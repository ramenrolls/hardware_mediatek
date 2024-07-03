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

#ifndef WPAF_DRIVER_ADAPTER_H
#define WPAF_DRIVER_ADAPTER_H

#include <mtk_log.h>

#include "Mutex.h"

#include "WpfaDriverMessage.h"
#include "WpfaDriverVersionData.h"
#include "WpfaDriverAcceptData.h"

using ::android::Mutex;

class WpfaDriverAdapter {
  public:
    WpfaDriverAdapter();
    virtual ~WpfaDriverAdapter();
    static WpfaDriverAdapter* getInstance();
    static void setCcciHandler(int fd);
    void init();

    int sendMsgToControlMsgDispatcher(const sp<WpfaDriverMessage>& message);
    int sendMsgToFilterRuleReqHandler(const sp<WpfaDriverMessage>& message);
    int sendMsgToShmReadMsgHandler(const sp<WpfaDriverMessage>& message);
    int sendMsgToShmWriteMsgHandler(const sp<WpfaDriverMessage>& message);

    int getCcciHandler();

  private:
    /**
     * singleton pattern
     */
    static WpfaDriverAdapter* sInstance;
    static Mutex sWpfaDriverAdapterInitMutex;

    static int mCcciHandler;
};

#endif /* end of WPAF_DRIVER_H */
