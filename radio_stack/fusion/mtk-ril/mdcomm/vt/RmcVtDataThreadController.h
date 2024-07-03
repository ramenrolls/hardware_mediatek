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

#ifndef __RMC_VT_DATA_THREAD_CONTROLLER_H__
#define __RMC_VT_DATA_THREAD_CONTROLLER_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "RfxBaseHandler.h"
#include <pthread.h>

/*****************************************************************************
 * Define
 *****************************************************************************/
#define VT_RIL_SHARE_DATA_STATUS_RECV_NONE (0)
#define VT_RIL_SHARE_DATA_STATUS_RECV_DATA (1)

/*****************************************************************************
 * Class RmcVtReqHandlerSharedMemory
 *****************************************************************************/
class RmcVtSharedMemory : virtual public RefBase {
  public:
    RmcVtSharedMemory(void);
    virtual ~RmcVtSharedMemory();

    void setState(int state);
    int getState(void);
    bool checkState(int want_state);
    void setSlotId(int id);
    void setSize(int size);
    void setData(char* data, int len);
    int getSlotId(void);
    int getSize(void);
    void getData(char** data);
    void clearData();
    void lock(const char* user);
    void unlock(const char* user);
    void wait(const char* user, int stay_state);
    void signal(const char* user);

  private:
    pthread_mutex_t mLock;
    pthread_mutex_t* mPLock;
    pthread_cond_t mCond;
    int mDataReadyCount;
    RIL_VT_SERVICE_MSG mSharedMsg;
};

/*****************************************************************************
 * Class RmcVtDataThreadController
 *****************************************************************************/
class RmcVtDataThreadController : virtual public RefBase {
  public:
    RmcVtDataThreadController(void);
    virtual ~RmcVtDataThreadController();

    void start();

    static void* RIL_IMCB_THREAD(void* arg);
    static void* VT_RIL_THREAD(void* arg);
    static sp<RmcVtSharedMemory> getSharedMem(void);

  private:
    static int ril_vt_looper();
    static int ril_vt_recv(int fd, void* buffer, int size);

    static bool isImsMessage(int msgId);
    static void handleMessage(int msgId, int length, void* data, int slotId);
    static void handleImsMessage(int size, char* outBuffer, int slotId);

    static sp<RmcVtSharedMemory> sShareMemmory;

    static pthread_t sVtRilThd;
    static pthread_t sImcbRilThd;

    static int sVtRilFd;
    static int sVtsFd;

    static int sIsVtConnected;
};

#endif
