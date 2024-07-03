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

#ifndef WPFA_RINGBUFFER_H
#define WPFA_RINGBUFFER_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "stdint.h"
#include <mtk_log.h>

using namespace std;
/*
 * =============================================================================
 *                     ref struct
 * =============================================================================
 */

/*
 * =============================================================================
 *                     typedef
 * =============================================================================
 */
typedef struct region_info_t {
    uint32_t read_idx;
    uint16_t data_size;
} region_info_t;

typedef struct RingBufferControlParam {
    int mRbState;
    uint32_t mWriteIdx;
    uint32_t mReadIdx;
    uint32_t mReadDataSize;
    uint32_t mTempReadIdx;
    uint32_t mTempReadDataSize;

    pthread_mutex_t mutex;
    pthread_cond_t cond_can_write;  // signaled when items are raad done (removed)
    pthread_cond_t cond_can_read;   // signaled when items are writed (added)
} RingBufferControlParam;
/*
 * =============================================================================
 *                     class
 * =============================================================================
 */
#define RING_BUFFER_SIZE (10240) /* Size = 10*1024 */

#define RING_BUFFER_STATE_NONE (0)
#define RING_BUFFER_STATE_READING (1)

#define INVALID_INDEX (RING_BUFFER_SIZE + 1)

class WpfaRingBuffer {
  public:
    WpfaRingBuffer();
    virtual ~WpfaRingBuffer();

    void initRingBuffer();

    void lock(const char* user);
    void unlock(const char* user);
    void waitCanWrite(const char* user, uint16_t dataSize);
    void signalCanWrite(const char* user);
    void waitCanRead(const char* user);
    void signalCanRead(const char* user);

    int writeDataToRingBuffer(const void* src, uint16_t dataSize);
    void readDataFromRingBuffer(void* des, uint32_t readIdx, uint16_t dataSize);
    void readDataWithoutRegionCheck(void* des, uint32_t readIdx, uint16_t dataSize);

    // int notifyToReader(uint32_t readIdx, uint16_t dataSize);
    void getRegionInfoForReader(region_info_t* mRegion);
    uint32_t getReadIndex() { return mControlPara.mReadIdx; }
    uint32_t getReadDataSize() { return mControlPara.mReadDataSize; }

    void readDone();

    void setState(int state);
    int getState();
    int dump_hex(unsigned char* data, int len);

  protected:
  private:
    unsigned char mRingBuffer[RING_BUFFER_SIZE];
    RingBufferControlParam mControlPara;

    void updateReadDataSizeByState(uint16_t dataSize);

    void ringBuffer_memset(void* des, uint8_t value, uint16_t size);
    void ringBuffer_memcpy(void* des, const void* src, uint16_t size);

    bool isEmpty();
    bool isFull();

    uint16_t getFreeSize();
    uint16_t getDataCount();

    void toString();
    void dumpParam();
};

#endif /* end of WPFA_RINGBUFFER_H */
