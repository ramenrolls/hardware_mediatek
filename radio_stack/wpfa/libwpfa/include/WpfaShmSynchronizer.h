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

#ifndef WPAF_SHM_SYNCHRONIZER_H
#define WPAF_SHM_SYNCHRONIZER_H

#include <mtk_log.h>

#include "Mutex.h"
#include <pthread.h>

#include "WpfaDriverMessage.h"
#include "WpfaRingBuffer.h"
#include "WpfaShmAccessController.h"

using ::android::Mutex;

/*
 * =============================================================================
 *                     Defines
 * =============================================================================
 */
#define SHM_WRITER_START_TID (10000)
#define SHM_WRITER_END_TID (50000)

/*
 * =============================================================================
 *                     typedef
 * =============================================================================
 */

typedef int writer_state_enum;
enum { SHM_WRITER_STATE_IDLE = 1, SHM_WRITER_STATE_READY = 2, SHM_WRITER_STATE_MD_READING = 3 };

typedef struct ShmCtrlParam {
    writer_state_enum mState;
    bool isWriting;
    bool isStateUpdating;

    uint32_t writeIdx;
    uint32_t writeSize;

    uint32_t newWriteIdx;
    uint32_t newWriteSize;

    pthread_mutex_t mutex;
    pthread_cond_t cond_can_write;         // signaled when data packets writed to shm
    pthread_cond_t cond_can_update_state;  // signaled when mState updated done
} ShmCtrlParam;

/*
 * =============================================================================
 *                     class
 * =============================================================================
 */
class WpfaShmSynchronizer {
  public:
    WpfaShmSynchronizer();
    virtual ~WpfaShmSynchronizer();
    static WpfaShmSynchronizer* getInstance();
    void init();

    // API of control param
    void lock(const char* user);
    void trylock(const char* user);
    void unlock(const char* user);
    void waitCanWrite(const char* user);
    void signalCanWrite(const char* user);
    void waitCanUpdateState(const char* user);
    void signalCanUpdateState(const char* user);
    void setState(writer_state_enum state);
    writer_state_enum getState();

    uint32_t getMaxDataBufferSize();
    uint32_t getRealDataBufferSize();
    int getCcciHandler();

    int processControlMessage(uint16_t tId, uint16_t msgId);
    int wirteDataToShm(WpfaRingBuffer* ringBuffer);

  protected:
  private:
    void setWritingFlag(bool isWriting);
    bool isWriting();
    void setStateUpdatingFlag(bool isUpdating);
    bool isStateUpdating();
    void dumpCtrlParams();

    // ccci ctrl message handle
    int onRequestData(uint16_t tId);
    int onRequestDataDone(uint16_t tId);

    // write data pkts
    int writeInIdleState(WpfaRingBuffer* ringBuffer);
    int writeInReadyState(WpfaRingBuffer* ringBuffer);
    int writeInMdReadingState(WpfaRingBuffer* ringBuffer);

    int sendMessageToModem(uint16_t tId, uint16_t msgId);

    // generate transaction id for SHM writer
    uint16_t generateShmTid();

    int checkDriverAdapterState();
    /**
     * singleton pattern
     */
    static WpfaShmSynchronizer* sInstance;
    static Mutex sWpfaShmSynchronizerInitMutex;

    static ShmCtrlParam mControlPara;
    uint16_t mTid;

    WpfaShmAccessController* mShmAccessController;
};

#endif /* end of WPAF_SHM_SYNCHRONIZER_H */
