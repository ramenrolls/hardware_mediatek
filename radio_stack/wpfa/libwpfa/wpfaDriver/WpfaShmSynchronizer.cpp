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

#include "WpfaShmSynchronizer.h"
#include "WpfaShmWriteMsgHandler.h"

#define WPFA_D_LOG_TAG "WpfaShmSynchronizer"

WpfaShmSynchronizer* WpfaShmSynchronizer::sInstance = NULL;
Mutex WpfaShmSynchronizer::sWpfaShmSynchronizerInitMutex;
ShmCtrlParam WpfaShmSynchronizer::mControlPara;

WpfaShmSynchronizer::WpfaShmSynchronizer() {
    mtkLogD(WPFA_D_LOG_TAG, "-new()");
    init();
}

WpfaShmSynchronizer::~WpfaShmSynchronizer() { mtkLogD(WPFA_D_LOG_TAG, "-del()"); }

WpfaShmSynchronizer* WpfaShmSynchronizer::getInstance() {
    if (sInstance != NULL) {
        return sInstance;
    } else {
        sWpfaShmSynchronizerInitMutex.lock();
        sInstance = new WpfaShmSynchronizer();
        if (sInstance == NULL) {
            mtkLogE(WPFA_D_LOG_TAG, "new WpfaShmSynchronizer fail");
        }
        sWpfaShmSynchronizerInitMutex.unlock();
        return sInstance;
    }
}

void WpfaShmSynchronizer::init() {
    // control parameter init
    setState(SHM_WRITER_STATE_IDLE);
    mControlPara.isWriting = false;
    mControlPara.isStateUpdating = false;

    mControlPara.writeIdx = 0;
    mControlPara.writeSize = 0;
    mControlPara.newWriteIdx = 0;
    mControlPara.newWriteSize = 0;

    mControlPara.mutex = PTHREAD_MUTEX_INITIALIZER;
    mControlPara.cond_can_write = PTHREAD_COND_INITIALIZER;
    mControlPara.cond_can_update_state = PTHREAD_COND_INITIALIZER;

    // transaction id init
    mTid = SHM_WRITER_START_TID;

    mShmAccessController = new WpfaShmAccessController();

    mtkLogD(WPFA_D_LOG_TAG, "-init(), mShmAccessController: [%p]", (void*)&mShmAccessController);
}

void WpfaShmSynchronizer::lock(const char* user) {
    mtkLogD(WPFA_D_LOG_TAG, "[ShmSync] get lock ... (%s)(%p)", user, (void*)&mControlPara.mutex);
    pthread_mutex_lock(&mControlPara.mutex);
    mtkLogD(WPFA_D_LOG_TAG, "[ShmSync] lock success (%s)", user);
}

void WpfaShmSynchronizer::trylock(const char* user) {
    mtkLogD(WPFA_D_LOG_TAG, "[ShmSync][trylock] get lock ... (%s)", user);
    for (int i = 0; i < 200; i++) {
        if (pthread_mutex_trylock(&mControlPara.mutex) == 0) {
            mtkLogD(WPFA_D_LOG_TAG, "[ShmSync][trylock] lock success");
            return;
        } else {
            mtkLogD(WPFA_D_LOG_TAG, "try lock Count=%d", i);
            usleep(10 * 1000); /* 10 ms */
        }
    }
    mtkLogE("Wpfa", "DRIVER_ASSERT:%s, %d", __FILE__, __LINE__);
    mtkAssert(NULL);
    exit(0);
}
void WpfaShmSynchronizer::unlock(const char* user) {
    pthread_mutex_unlock(&mControlPara.mutex);
    mtkLogD(WPFA_D_LOG_TAG, "[ShmSync] unlock success (%s)", user);
}

void WpfaShmSynchronizer::waitCanWrite(const char* user) {
    mtkLogD(WPFA_D_LOG_TAG, "[ShmSync] wait can write... (%s)", user);
    while (isStateUpdating()) {
        // wait until some state update done
        pthread_cond_wait(&mControlPara.cond_can_write, &mControlPara.mutex);
    }
    mtkLogD(WPFA_D_LOG_TAG, "[ShmSync] wait can write success (%s)", user);
}

void WpfaShmSynchronizer::signalCanWrite(const char* user) {
    pthread_cond_signal(&mControlPara.cond_can_write);
    mtkLogD(WPFA_D_LOG_TAG, "[ShmSync] signal can write success (%s)", user);
}

void WpfaShmSynchronizer::waitCanUpdateState(const char* user) {
    mtkLogD(WPFA_D_LOG_TAG, "[ShmSync] wait can update state .... (%s)", user);
    while (isWriting()) {
        // wait for data pkts write to SHM
        pthread_cond_wait(&mControlPara.cond_can_update_state, &mControlPara.mutex);
    }
    mtkLogD(WPFA_D_LOG_TAG, "[ShmSync] wait can update state (%s)", user);
}

void WpfaShmSynchronizer::signalCanUpdateState(const char* user) {
    pthread_cond_signal(&mControlPara.cond_can_update_state);
    mtkLogD(WPFA_D_LOG_TAG, "[ShmSync] signal can update state (%s)", user);
}

int WpfaShmSynchronizer::processControlMessage(uint16_t tId, uint16_t msgId) {
    int retValue = 0;
    mtkLogD(WPFA_D_LOG_TAG, "processControlMessage()+ tId=%d msgId=%d", tId, msgId);
    // get lock and wait for signel if needed
    trylock("processControlMessage");
    // waitCanUpdateState("processControlMessage");

    // set state updating flag
    // setStateUpdatingFlag(true);

    switch (msgId) {
        case MSG_M2A_REQUEST_DATA:
            onRequestData(tId);
            break;

        case MSG_M2A_REQUEST_DATA_DONE:
            onRequestDataDone(tId);
            break;

        default:
            mtkLogE(WPFA_D_LOG_TAG, "processControlMessage, type=%d not support", msgId);
    }

    // reset state updating flag
    // setStateUpdatingFlag(false);

    // signal writer thread and release mutex
    // signalCanWrite("processControlMessage");
    unlock("processControlMessage");
    mtkLogD(WPFA_D_LOG_TAG, "processControlMessage()-");
    return retValue;
}

// ccci ctrl message handle
int WpfaShmSynchronizer::onRequestData(uint16_t tId) {
    if (mShmAccessController == NULL) {
        mtkLogD(WPFA_D_LOG_TAG, "onRequestData(): mShmAccessController is NULL");
    }
    mtkLogD(WPFA_D_LOG_TAG, "onRequestData()+ tId:%d, mShmAccessController: [%p]", tId,
            (void*)&mShmAccessController);
    dumpCtrlParams();
    mShmAccessController->dumpShmDLCtrParm();
    int retValue = 0;
    if (tId == mTid) {
        // 1. set newBeginIdx = WrideIdx+size and newSize=0
        mControlPara.newWriteIdx =
                (mControlPara.writeIdx + mControlPara.writeSize) % WPFA_SHM_MAX_DATA_BUFFER_SIZE;
        mControlPara.newWriteSize = 0;

        mtkLogD(WPFA_D_LOG_TAG, "onRequestData, newWriteIdx=%d newWriteSize=%d",
                mControlPara.newWriteIdx, mControlPara.newWriteSize);

        // 2. send message to ccci
        sendMessageToModem(tId, MSG_A2M_REQUEST_DATA_ACK);
        // 3. change mState
        setState(SHM_WRITER_STATE_MD_READING);
    } else {
        mtkLogE(WPFA_D_LOG_TAG, "onRequestData id not match, tId:%d", tId);
        retValue = -1;
    }
    mShmAccessController->dumpShmDLCtrParm();
    dumpCtrlParams();
    mtkLogD(WPFA_D_LOG_TAG, "onRequestData()-");
    return retValue;
}

int WpfaShmSynchronizer::onRequestDataDone(uint16_t tId) {
    if (mShmAccessController == NULL) {
        mtkLogD(WPFA_D_LOG_TAG, "onRequestData(): mShmAccessController is NULL");
    }
    mtkLogD(WPFA_D_LOG_TAG, "onRequestDataDone() tId:%d, mShmAccessController: [%p]", tId,
            (void*)&mShmAccessController);
    int retValue = 0;
    if (tId == mTid) {
        mtkLogD(WPFA_D_LOG_TAG, "onRequestDataDone +");
        dumpCtrlParams();
        mShmAccessController->dumpShmDLCtrParm();
        // 1. set newBeginIdx and newSize to SHM
        if (mControlPara.newWriteSize > 0) {
            // 2. update new param to SHM
            mControlPara.writeIdx = mControlPara.newWriteIdx;
            mControlPara.writeSize = mControlPara.newWriteSize;

            // update shm control param
            mShmAccessController->setMdReadIdx(mControlPara.writeIdx);
            mShmAccessController->setMdReadSize(mControlPara.writeSize);

            mControlPara.newWriteIdx = 0;
            mControlPara.newWriteSize = 0;
            mShmAccessController->setTempReadSize(0);

            mtkLogD(WPFA_D_LOG_TAG, "onRequestDataDone, writeIdx=%d writeSize=%d",
                    mControlPara.writeIdx, mControlPara.writeSize);

            // 3. send event to MD
            sendMessageToModem(generateShmTid(), MSG_A2M_DATA_READY);
            // 4. change mState
            setState(SHM_WRITER_STATE_READY);
        } else {
            mControlPara.writeIdx = mControlPara.newWriteIdx;
            mShmAccessController->setMdReadIdx(mControlPara.writeIdx);
            mShmAccessController->setMdReadSize(0);

            mControlPara.newWriteIdx = 0;
            mControlPara.newWriteSize = 0;
            mShmAccessController->setTempReadSize(0);

            // change mState to IDLE
            setState(SHM_WRITER_STATE_IDLE);
        }
        mShmAccessController->dumpShmDLCtrParm();
        dumpCtrlParams();
        mtkLogD(WPFA_D_LOG_TAG, "onRequestDataDone -");
    } else {
        mtkLogE(WPFA_D_LOG_TAG, "onRequestDataDone id not match, tId:%d", tId);
        retValue = -1;
    }

    return retValue;
}

int WpfaShmSynchronizer::wirteDataToShm(WpfaRingBuffer* ringBuffer) {
    int retValue = 0;

    // acquire mutex and wait
    lock("wirteDataToShm");
    // waitCanWrite("wirteDataToShm");

    // set Writing flag
    // setWritingFlag(true);

    switch (getState()) {
        case SHM_WRITER_STATE_IDLE:
            mtkLogD(WPFA_D_LOG_TAG, "call writeInIdleState");
            retValue = writeInIdleState(ringBuffer);
            break;

        case SHM_WRITER_STATE_READY:
            mtkLogD(WPFA_D_LOG_TAG, "call writeInReadyState");
            retValue = writeInReadyState(ringBuffer);
            break;

        case SHM_WRITER_STATE_MD_READING:
            mtkLogD(WPFA_D_LOG_TAG, "call writeInMdReadingState");
            retValue = writeInMdReadingState(ringBuffer);
            break;

        default:
            mtkLogE(WPFA_D_LOG_TAG, "not handle state, mState=%d", getState());
    }

    // reset Writing flag
    // setWritingFlag(false);

    // signal for state update and release mutex
    // signalCanUpdateState("wirteDataToShm");
    unlock("wirteDataToShm");

    return retValue;
}

// write data pkts
int WpfaShmSynchronizer::writeInIdleState(WpfaRingBuffer* ringBuffer) {
    mtkLogD(WPFA_D_LOG_TAG, "writeInIdleState +, mShmAccessController: [%p]",
            (void*)&mShmAccessController);

    int retValue = 0;
    uint32_t newIdx = 0;
    uint32_t shmReadIdx = mShmAccessController->getReadIdx();

    dumpCtrlParams();
    mShmAccessController->dumpShmDLCtrParm();

    // 1. copy data to SHM
    newIdx = mShmAccessController->writeApDataToShareMemory(ringBuffer);
    if (newIdx >= 0) {
        // 2. update all control paramaters(write_idx and size) in SHM
        mControlPara.writeIdx = shmReadIdx;
        mControlPara.writeSize = ringBuffer->getReadDataSize();

        mShmAccessController->setMdReadIdx(mControlPara.writeIdx);
        mShmAccessController->setMdReadSize(mControlPara.writeSize);

        mtkLogD(WPFA_D_LOG_TAG, "writeInIdleState, writeIdx=%d writeSize=%d", mControlPara.writeIdx,
                mControlPara.writeSize);

        // 3. send A2M_DATA_READY to modem
        sendMessageToModem(generateShmTid(), MSG_A2M_DATA_READY);

        // 4. update mState first
        setState(SHM_WRITER_STATE_READY);
    } else {
        mtkLogE(WPFA_D_LOG_TAG, "writeInIdleState, fail to write");
        retValue = -1;
    }
    mShmAccessController->dumpShmDLCtrParm();
    dumpCtrlParams();
    mtkLogD(WPFA_D_LOG_TAG, "writeInIdleState -");

    return retValue;
}

int WpfaShmSynchronizer::writeInReadyState(WpfaRingBuffer* ringBuffer) {
    int retValue = 0;
    uint32_t newIdx = 0;

    mtkLogD(WPFA_D_LOG_TAG, "writeInReadyState +, mShmAccessController: [%p]",
            (void*)&mShmAccessController);
    dumpCtrlParams();
    mShmAccessController->dumpShmDLCtrParm();

    // 1. copy data to SHM
    newIdx = mShmAccessController->writeApDataToShareMemory(ringBuffer);
    if (newIdx >= 0) {
        // 2. only update data size in shm
        mControlPara.writeSize += ringBuffer->getReadDataSize();
        mShmAccessController->setMdReadSize(mControlPara.writeSize);

        mtkLogD(WPFA_D_LOG_TAG, "writeInReadyState, writeIdx=%d writeSize=%d",
                mControlPara.writeIdx, mControlPara.writeSize);

    } else {
        mtkLogE(WPFA_D_LOG_TAG, "writeInReadyState, fail to write");
        retValue = -1;
    }
    mShmAccessController->dumpShmDLCtrParm();
    dumpCtrlParams();
    mtkLogD(WPFA_D_LOG_TAG, "writeInReadyState -");
    return retValue;
}

int WpfaShmSynchronizer::writeInMdReadingState(WpfaRingBuffer* ringBuffer) {
    int retValue = 0;
    uint32_t newIdx = 0;

    mtkLogD(WPFA_D_LOG_TAG, "writeInMdReadingState +, mShmAccessController: [%p]",
            (void*)&mShmAccessController);
    dumpCtrlParams();
    mShmAccessController->dumpShmDLCtrParm();

    // 1. copy data to SHM
    newIdx = mShmAccessController->writeApDataToShareMemory(ringBuffer);
    if (newIdx >= 0) {
        // 2. only update new data size in local
        mControlPara.newWriteSize += ringBuffer->getReadDataSize();
        mShmAccessController->setTempReadSize(mControlPara.newWriteSize);

        mtkLogD(WPFA_D_LOG_TAG, "writeInMdReadingState, newWriteIdx=%d newWriteSize=%d",
                mControlPara.newWriteIdx, mControlPara.newWriteSize);

    } else {
        mtkLogE(WPFA_D_LOG_TAG, "writeInMdReadingState, fail to write");
        retValue = -1;
    }
    mShmAccessController->dumpShmDLCtrParm();
    dumpCtrlParams();
    mtkLogD(WPFA_D_LOG_TAG, "writeInMdReadingState -");

    return retValue;
}

int WpfaShmSynchronizer::sendMessageToModem(uint16_t tId, uint16_t msgId) {
    int retValue = 0;

    sp<WpfaDriverMessage> msg = WpfaDriverMessage::obtainMessage(msgId, tId, CCCI_CTRL_MSG, 0);
    WpfaShmWriteMsgHandler::enqueueShmWriteMessage(msg);
    return retValue;
}

void WpfaShmSynchronizer::setState(writer_state_enum state) {
    mtkLogD(WPFA_D_LOG_TAG, "setState() state=%d", state);
    mControlPara.mState = state;
}

writer_state_enum WpfaShmSynchronizer::getState() { return mControlPara.mState; }

void WpfaShmSynchronizer::setWritingFlag(bool isWriting) {
    mtkLogD(WPFA_D_LOG_TAG, "setWritingFlag() isWriting=%d", isWriting);
    mControlPara.isWriting = isWriting;
}

bool WpfaShmSynchronizer::isWriting() { return mControlPara.isWriting; }

void WpfaShmSynchronizer::setStateUpdatingFlag(bool isUpdating) {
    mtkLogD(WPFA_D_LOG_TAG, "setStateUpdatingFlag() isUpdating=%d", isUpdating);
    mControlPara.isStateUpdating = isUpdating;
}

bool WpfaShmSynchronizer::isStateUpdating() { return mControlPara.isStateUpdating; }

void WpfaShmSynchronizer::dumpCtrlParams() {
    mtkLogD(WPFA_D_LOG_TAG,
            "dumpCtrlParams writeIdx=%d writeSize=%d newWriteIdx=%d,"
            "newWriteSize=%d",
            mControlPara.writeIdx, mControlPara.writeSize, mControlPara.newWriteIdx,
            mControlPara.newWriteSize);
}

uint32_t WpfaShmSynchronizer::getMaxDataBufferSize() {
    return mShmAccessController->getMaxDataBufferSize();
}

uint32_t WpfaShmSynchronizer::getRealDataBufferSize() {
    return mShmAccessController->getRealDataBufferSize();
}

int WpfaShmSynchronizer::getCcciHandler() {
    return mShmAccessController->getCcciShareMemoryHandler();
}

uint16_t WpfaShmSynchronizer::generateShmTid() {
    uint16_t newTid = mTid;
    newTid = newTid + 1;
    if (newTid >= SHM_WRITER_END_TID) {
        newTid = SHM_WRITER_START_TID;
    }
    mTid = newTid;
    mtkLogD(WPFA_D_LOG_TAG, "generateShmTid() newTid=%d", newTid);
    return newTid;
}
