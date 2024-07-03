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

#ifndef __RMC_RCS_DATA_THREAD_CONTROLLER_H__
#define __RMC_RCS_DATA_THREAD_CONTROLLER_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "RfxBaseHandler.h"
#include <pthread.h>
#include "volte_core_interface.h"

/*****************************************************************************
 * Define
 *****************************************************************************/
#define RCS_RIL_SOCKET_NAME "@/rcs_rild"
#define UA_NUM (1)                               // rcs
#define MAX_CLIENT (UA_NUM * 2)                  // one for reg, one for sip msg each
#define UA_CHANNEL_HEADER_LEN (3 * sizeof(int))  //(offsetof(Channel_Data_t, data))
#define MD_CHANNEL_HEADER_LEN (5 * sizeof(int))  //(offsetof(VoLTE_Event_IMS_HIF_Data, data))
#define CHANNEL_BUFFER_SIZE 65535  // align SIP_TX_RECV_BUFF_SIZE & SIP_TCP_RECEIVE_BUFFER_SIZE
#define RCS_RIL_SHARE_DATA_STATUS_RECV_NONE (0)
#define RCS_RIL_SHARE_DATA_STATUS_RECV_DATA (1)

/*****************************************************************************
 * typedef
 *****************************************************************************/
typedef struct _Ril_Channel_Obj {
    int fd;
    pthread_mutex_t mutex;
    void* channels;
    int channels_size;
    int type; /* VoLTE_Stack_UA_Type_e */
} Ril_Channel_Obj_t;

typedef struct _Ril_Channel_Client {
    Ril_Channel_Obj_t channel;
    int buf_len;
    char buf[CHANNEL_BUFFER_SIZE];
} Ril_Channel_Client_t;

/*****************************************************************************
 * Class RmcRcsSharedMemory
 *****************************************************************************/
class RmcRcsSharedMemory : virtual public RefBase {
  public:
    RmcRcsSharedMemory(void);
    virtual ~RmcRcsSharedMemory();

    void setState(int state);
    int getState(void);
    bool checkState(int want_state);
    void setSize(int len);
    void setData(char* data, int len);
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
    int mDatalen;  // recv data length from md
    char* mData;   // recv data from md
};

/*****************************************************************************
 * Class RmcRcsDataThreadController
 *****************************************************************************/
class RmcRcsDataThreadController : virtual public RefBase {
  public:
    RmcRcsDataThreadController();
    virtual ~RmcRcsDataThreadController();
    static void init();
    static void* rilServerThread(void* arg);    // process data from UA
    static void* shareMemoryThread(void* arg);  // process data from md
    static void sendMsgToShareMemory(char* outBuffer, int length, const char* user);
    static bool isLogEnable();

  private:
    static void rilServerRead(Channel_Data_t* msgData, Ril_Channel_Obj_t* channel);
    static void startThread();  // only be called once when init
    static int fsend(int fd, char* data, int len);
    static Ril_Channel_Obj_t* getClientObjbyUaType(int uaType);
    static void* convertUAData(Channel_Data_t* _data, int uaType, int acctId);
    static int sendDataToUA(Channel_Data_t* data, int uaType);
    static sp<RmcRcsSharedMemory> getSharedMem(void);

    static pthread_t sRilServerThd;    // process UA data
    static pthread_t sShareMemoryThd;  // process Md data
    static sp<RmcRcsSharedMemory> sShareMemory;
    static pthread_mutex_t sMutex;
    static Ril_Channel_Obj_t* sChannelObj;
    static RmcRcsDataThreadController* sInstance;
};

#endif /* __RMC_RCS_DATA_THREAD_CONTROLLER_H__ */
