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

#ifndef WPAF_SHM_ACCESS_CONTROLLER_H
#define WPAF_SHM_ACCESS_CONTROLLER_H

#include <map>
#include <mtk_log.h>

#include "Mutex.h"

#include "WpfaRingBuffer.h"
#include "FilterStructure.h"

using ::android::Mutex;

/*
 * =============================================================================
 *                     Defines
 * =============================================================================
 */

#define WPFA_SHM_MAX_SIZE (32 * 1024)          // SHM total size: 32 KB = 32768 bytes
#define WPFA_SHM_GUARD_REGION_SIZE (4)         // 4 bytes
#define WPFA_SHM_CONTROL_PARAM_SIZE (8)        // 8 bytes
#define WPFA_SHM_MAX_DATA_BUFFER_SIZE (32744)  // ((32*1024)-((4+8)*2)) = 32744

/* UT only*/
// #define WPFA_SHM_MAX_SIZE (28 * 1024)           // SHM total size: 28 KB = 28672 bytes
// #define WPFA_SHM_GUARD_REGION_SIZE (4)          // 4 bytes
// #define WPFA_SHM_CONTROL_PARAM_SIZE (8)         // 8 bytes
// #define WPFA_SHM_MAX_DATA_BUFFER_SIZE (28648)   // ((28*1024)-((4+8)*2)) = 28648

/*
 * =============================================================================
 *                     typedef
 * =============================================================================
 */

/* ######################################################################### */
#pragma pack()
/* ######################################################################### */

typedef struct ApRingBufferControlParam {
    uint32_t offset;          // count from the address of wpfa_shm_t
    uint32_t ap_write_index;  // AP start to write
    uint32_t md_read_index;   // MD start to read (mapping to SHM control param for AP)
    uint32_t md_read_size;    // MD read data size (mapping to SHM control param for AP)
    uint32_t temp_read_size;
} ApRingBufferControlParam;

typedef struct wpfa_shm_control_region_t {
    uint32_t index;
    uint32_t size;
} wpfa_shm_control_region_t;

typedef struct wpfa_shm_data_region_ul_only_t {
    uint8_t ul_data[WPFA_SHM_MAX_DATA_BUFFER_SIZE];
} wpfa_shm_data_region_ul_only_t;

typedef struct wpfa_shm_data_region_dl_only_t {
    uint8_t dl_data[WPFA_SHM_MAX_DATA_BUFFER_SIZE];
} wpfa_shm_data_region_dl_only_t;

typedef struct wpfa_shm_data_region_share_t {
    uint8_t ul_data[WPFA_SHM_MAX_DATA_BUFFER_SIZE / 2];
    uint8_t dl_data[WPFA_SHM_MAX_DATA_BUFFER_SIZE / 2];
} wpfa_shm_data_region_share_t;

typedef struct wpfa_shm_data_region_t {
    union {
        wpfa_shm_data_region_ul_only_t ulOnlyDataBuffer;
        wpfa_shm_data_region_dl_only_t dlOnlyDataBuffer;
        wpfa_shm_data_region_share_t shareDataBuffer;
    } u;
} wpfa_shm_data_region_t;

typedef struct wpfa_shm_t {
    /* 4 bytes gurard region */
    uint8_t guard_region_header[WPFA_SHM_GUARD_REGION_SIZE];

    /* 8 bytes UL control param */
    wpfa_shm_control_region_t ul_control_param;

    /* 8 bytes DL control param */
    wpfa_shm_control_region_t dl_control_param;

    /* 32744 bytes data region*/
    wpfa_shm_data_region_t data_region;

    /* 4 bytes gurard region */
    uint8_t guard_region_footer[WPFA_SHM_GUARD_REGION_SIZE];
} wpfa_shm_t;

/* ######################################################################### */
#pragma pack()
/* ######################################################################### */

/*
 * =============================================================================
 *                     class
 * =============================================================================
 */
class WpfaShmAccessController {
  public:
    WpfaShmAccessController();
    virtual ~WpfaShmAccessController();
    void init();

    int resetShareMemoryIndex();
    uint32_t writeApDataToShareMemory(WpfaRingBuffer* ringBuffer);
    void dumpShmDLCtrParm();
    void dumpShmWriteDataInShm(uint32_t index, uint32_t size);

    uint32_t getReadIdx() {
        // TODO: need to confrim with modem of initial value(=begin address of ap data region or =0)
        return mApRingBufferCtrlParam.md_read_index;
    }

    void setMdReadIdx(uint32_t index) {
        mApRingBufferCtrlParam.md_read_index = index;
        // update to SHM
        pShareMemory->dl_control_param.index = mApRingBufferCtrlParam.offset + index;
    }

    uint32_t getMdReadSize() { return mApRingBufferCtrlParam.md_read_size; }

    void setMdReadSize(uint32_t size) {
        mApRingBufferCtrlParam.md_read_size = size;
        pShareMemory->dl_control_param.size = size;
    }

    void setTempReadSize(uint32_t size) { mApRingBufferCtrlParam.temp_read_size = size; }

    uint32_t getMaxDataBufferSize() { return WPFA_SHM_MAX_DATA_BUFFER_SIZE; }

    uint32_t getRealDataBufferSize() {
        return mShareMemoryLength -
               ((WPFA_SHM_GUARD_REGION_SIZE + WPFA_SHM_CONTROL_PARAM_SIZE) * 2);
    }

    int getCcciShareMemoryHandler() { return mCcciShareMemoryHandler; }

  protected:
  private:
    void lock(const char* user);
    void unlock(const char* user);

    int openShareMemory();
    int closeShareMemory();
    int formatShareMemory();

    void shm_memset(void* des, uint8_t value, uint16_t size);
    void shm_memcpy(void* des, const void* src, uint16_t size);
    uint32_t getFreeSizeOfApToMd();

    void dumpShmLayout();
    void dumpApRingBufferCtrlParam();
    int dump_hex(unsigned char* data, int len);

    int mCcciShareMemoryHandler;

    unsigned char* pShareMemoryBase;
    unsigned int mShareMemoryLength;
    wpfa_shm_t* pShareMemory;

    uint32_t mDlBufferSize;
    uint32_t mUlBufferSize;

    ApRingBufferControlParam mApRingBufferCtrlParam;

    pthread_mutex_t mShmAccessMutex;
};

#endif /* end of WPAF_SHM_WRITER_H */
