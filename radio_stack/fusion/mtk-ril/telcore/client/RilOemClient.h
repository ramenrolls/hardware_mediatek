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

#ifndef __RIL_OEM_CLIENT_H__
#define __RIL_OEM_CLIENT_H__

#include <stdint.h>
#include "RilClient.h"
#include "RfxMessage.h"
#include "RfxAtLine.h"

class RilOemClient : public RilClient {
  protected:
    typedef enum {
        FAILURE = -1,
        SUCCESS = 0,
        NO_ACTION = 1,
    } Result;
    typedef enum {
        GAME_MODE_NONE = 0,
        GAME_MODE_LOADING,
        GAME_MODE_IN_MATCH,
        GAME_MODE_IDLE,
        GAME_MODE_BATTLE,
        GAME_MODE_BOOST,
    } GameMode;

    typedef enum {
        STATE_PAUSED = 0,
        STATE_RESUMED = 1,
        STATE_DESTORYED = 2,
        STATE_DEAD = 3,
        STATE_STOPPED = 4,
        STATE_NUM = 5,
    } AppStatus;

  public:
    RilOemClient(int identity, char* socketName);
    virtual ~RilOemClient();

    // Override, if you have other imprementation
    /* oem socket would not handle any urc */
    virtual void handleUnsolicited(int slotId, int unsolResponse, void* data, size_t datalen,
                                   UrcDispatchRule rule);
    virtual void handleStateActive();
    virtual void requestComplete(RIL_Token token, RIL_Errno e, void* response, size_t responselen);

    virtual int handleSpecialRequestWithArgs(int argCount, char** args);

  private:
    void freeArgs(char** args, int number);
    void executeThermal(char* arg);
    void executeHangupAll();
    void executeShutDownByThermal(char* arg);
    void executeGameMode(char* arg);
    void executeUpdatePacket(char* arg);
    void executeLowLatencyMode(char* arg);
    void executeWeakSignalOpt(char* arg);
    void executeAppStatus(char* arg);
    void executeQueryCapability(char* arg);
    int mThermalFd = -1;
    int mQueryCapFd = -1;
    int mLowLatencyMode = 0;
    int mGameMode = 0;
    String8 mPhantomPacket;
};
#endif
