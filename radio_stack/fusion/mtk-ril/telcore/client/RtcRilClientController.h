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

#ifndef __RTC_RIL_CLIENT_CONTROLLER_H__
#define __RTC_RIL_CLIENT_CONTROLLER_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "RfxController.h"
#include "RfxTimer.h"
#include "RilClient.h"
#include "RilClientConstants.h"
#include "RilClientQueue.h"
/*****************************************************************************
 * Class RfxController
 *****************************************************************************/

using namespace std;

typedef enum {
    CLIENT_TYPE_DEFAULT,
    CLIENT_TYPE_OEM,
    CLIENT_TYPE_MTTS1,
    CLIENT_TYPE_MTTS2,
    CLIENT_TYPE_MTTS3,
    CLIENT_TYPE_MTTS4,
    CLIENT_TYPE_ATCI,
} RilClientType;

struct ClientInformation {
    int identity;
    char* socketName;
    RilClientType type;
};

class RtcRilClientController : public RfxController {
    RFX_DECLARE_CLASS(RtcRilClientController);

  public:
    RtcRilClientController();
    virtual ~RtcRilClientController();
    static int queryFileDescriptor(int clientId);
    static RilClient* findClientWithId(int clientId);
    static bool onClientRequestComplete(RIL_Token token, RIL_Errno e, void* response,
                                        size_t responselen, int clientId);
    static bool onClientUnsolicitedResponse(int slotId, int urcId, void* response,
                                            size_t responselen, UrcDispatchRule rule);
    static int sendResponse(Parcel& parcel, int fd);
    static int blockingWrite(int fd, const void* buffer, size_t len);
    static RilClientQueue* clientHead;
    static int getCdmaSlotId();

  protected:
    virtual void onInit();
    virtual void initRilClient();
    void registerRilClient(RilClient* client);
    void deregisterRilClient(RilClient* client);
    void startListenSocket(ClientInformation information);
};

#endif /* __RP_RADIO_CONTROLLER_H__ */
