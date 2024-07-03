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

#ifndef __RIL_CLIENT_H__
#define __RIL_CLIENT_H__

#include <stddef.h>
#include <utils/Thread.h>
#include "utils/RefBase.h"
#include "utils/Errors.h"
#include "RilClientConstants.h"
#include <telephony/record_stream.h>
#include "RfxDefs.h"

using namespace android;

typedef enum {
    CLIENT_STATE_UNKNOWN,
    CLIENT_INITIALIZING,
    CLIENT_ACTIVE,
    CLIENT_DEACTIVE,
    CLIENT_CLOSED,
} RilClientState;

typedef struct UrcList {
    int id;
    char* data;
    size_t datalen;
    UrcDispatchRule rule;
    UrcList* pNext;
} UrcList;

class RilClient {
  public:
    RilClient(int identity, char* socketName);
    virtual ~RilClient();
    void setClientState(RilClientState state);

    int identity;
    char* socketName;
    int commandFd;
    RecordStream* stream;
    int listenFd;
    RilClientState clientState;

    class StateActivityThread : public Thread {
      public:
        StateActivityThread(RilClient* client);
        virtual ~StateActivityThread();

      protected:
        RilClient* client;
        virtual bool threadLoop();
    };

    sp<StateActivityThread> activityThread;

    static char* clientStateToString(RilClientState state);
    void clientStateCallback();
    void handleStateInitializing();

    // Override, if you have other imprementation
    virtual void handleStateActive();
    virtual void handleStateDeactive();
    virtual void handleStateClosed();
    virtual void processCommands(void* buffer, size_t buflen, int clientId);
    virtual void handleUnsolicited(int slotId, int unsolResponse, void* data, size_t datalen,
                                   UrcDispatchRule rule);
    virtual void addHeaderToResponse(Parcel* p);
    virtual void requestComplete(RIL_Token token, RIL_Errno e, void* response, size_t responselen);
    virtual bool isNeedToCache(int unsolResponse);
    virtual void cacheUrc(int unsolResponse, const void* data, size_t datalen, UrcDispatchRule rule,
                          int slotId);
    virtual void sendUrc(int slotId, UrcList* urcCached);
    virtual void sendPendedUrcs(int fdCommand);

  private:
    UrcList** mPendingUrc = NULL;
};

#endif /* __RIL_CLIENT_H__ */
