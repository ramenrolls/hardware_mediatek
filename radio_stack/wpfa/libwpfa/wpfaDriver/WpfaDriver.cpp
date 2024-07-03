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

#include "WpfaDriver.h"

#define WPFA_D_LOG_TAG "WpfaDriver"

WpfaDriver* WpfaDriver::sInstance = NULL;
Mutex WpfaDriver::sWpfaDriverInitMutex;

WpfaDriver::WpfaDriver() {
    mtkLogD(WPFA_D_LOG_TAG, "-new()");
    mWpfaDriverAdapter = NULL;
    mWpfaShmSynchronizer = NULL;
    mDriverTid = DRIVER_TID_START;
    init();
}

void WpfaDriver::init() {
    int ret = 0;
    int ccciHandler = -1;
    ret = checkShmControllerState();
    if (ret == 0) {
        ccciHandler = mWpfaShmSynchronizer->getCcciHandler();
        mtkLogD(WPFA_D_LOG_TAG, "init getCcciHandler:%d", ccciHandler);
        mWpfaDriverAdapter->setCcciHandler(ccciHandler);
        ret = checkDriverAdapterState();
    }
    mtkLogD(WPFA_D_LOG_TAG, "-init() ret=%d", ret);
}

WpfaDriver::~WpfaDriver() { mtkLogD(WPFA_D_LOG_TAG, "-del()"); }

WpfaDriver* WpfaDriver::getInstance() {
    if (sInstance != NULL) {
        return sInstance;
    } else {
        sWpfaDriverInitMutex.lock();
        sInstance = new WpfaDriver();
        if (sInstance == NULL) {
            mtkLogE(WPFA_D_LOG_TAG, "new WpfaDriver fail");
        }
        sWpfaDriverInitMutex.unlock();
        return sInstance;
    }
}

int WpfaDriver::registerCallback(event_id_enum eventId, CallbackFunc callbackFunc) {
    std::map<int, CallbackStruc>::iterator iter;

    iter = mapCallbackFunc.find(eventId);
    if (iter != mapCallbackFunc.end()) {
        mtkLogE(WPFA_D_LOG_TAG, "[%s] callback already existed!", __FUNCTION__);
        return 1;
    }
    addCallback(eventId, callbackFunc);
    return 0;
}

int WpfaDriver::unregisterCallback(event_id_enum eventId) {
    std::map<int, CallbackStruc>::iterator iter;

    iter = mapCallbackFunc.find(eventId);
    if (iter != mapCallbackFunc.end()) {
        mapCallbackFunc.erase(iter);
        mtkLogD(WPFA_D_LOG_TAG, "[%s] remove callback", __FUNCTION__);
        return 0;
    } else {
        mtkLogE(WPFA_D_LOG_TAG, "[%s] callback not found!", __FUNCTION__);
        return 1;
    }
}

int WpfaDriver::notifyCallback(event_id_enum eventId, void* notifyArg) {
    int ret = 0;
    std::map<int, CallbackStruc>::iterator iter;

    iter = mapCallbackFunc.find(eventId);
    if (iter != mapCallbackFunc.end()) {
        // mtkLogD(WPFA_D_LOG_TAG, "[%s] invoke callback", __FUNCTION__);
        ret = (mapCallbackFunc[eventId].cb)((void*)notifyArg);
        return ret;
    } else {
        mtkLogE(WPFA_D_LOG_TAG, "[%s] callback not found!", __FUNCTION__);
    }
    return ret;
}

int WpfaDriver::addCallback(event_id_enum eventId, CallbackFunc callbackFunc) {
    struct CallbackStruc callback;
    callback.cb = callbackFunc;
    mapCallbackFunc[eventId] = callback;
    mtkLogD(WPFA_D_LOG_TAG, "[%s] eventId=%d success", __FUNCTION__, eventId);
    return 0;
}

int WpfaDriver::notifyWpfaInit() {
    int retValue = checkDriverAdapterState();
    if (retValue == 0) {
        sp<WpfaDriverMessage> msg = WpfaDriverMessage::obtainMessage(
                MSG_A2M_WPFA_INIT, generateDriverTid(), CCCI_IP_TABLE_MSG, 0);
        retValue = mWpfaDriverAdapter->sendMsgToControlMsgDispatcher(msg);
    }
    return retValue;
}

int WpfaDriver::notifyWpfaVersion(uint16_t apVer, uint16_t mdVer) {
    uint32_t maxDataBufferSize = 0;
    uint32_t realDataBufferSize = 0;
    int retValue = checkDriverAdapterState();
    if (retValue == 0) {
        retValue = checkShmControllerState();
        if (retValue == 0) {
            wifiproxy_ap_md_filter_ver_t* pVersionData =
                    (wifiproxy_ap_md_filter_ver_t*)calloc(1, sizeof(wifiproxy_ap_md_filter_ver_t));

            if (pVersionData == NULL) {
                mtkLogD(WPFA_D_LOG_TAG, "[%s] pVersionData is NULL, return failed.", __FUNCTION__);
                return -1;
            }
            pVersionData->ap_filter_ver = apVer;
            pVersionData->md_filter_ver = mdVer;

            // set Total data buffer size for UL and DL
            maxDataBufferSize = mWpfaShmSynchronizer->getMaxDataBufferSize();
            realDataBufferSize = mWpfaShmSynchronizer->getRealDataBufferSize();
            // TODO: handle data buffer size not sync
            switch (CURRENT_SHM_CONFIG_MODE) {
                case SHM_CONFIG_DL_ONLY:
                    pVersionData->dl_buffer_size = maxDataBufferSize;
                    pVersionData->ul_buffer_size = 0;
                    break;

                case SHM_CONFIG_UL_ONLY:
                    /* not supported */
                    pVersionData->dl_buffer_size = 0;
                    pVersionData->ul_buffer_size = maxDataBufferSize;
                    break;

                case SHM_CONFIG_SHARE:
                    pVersionData->dl_buffer_size = maxDataBufferSize / 2;
                    pVersionData->ul_buffer_size = maxDataBufferSize / 2;
                    break;

                default:
                    mtkLogE(WPFA_D_LOG_TAG, "Error: unknown CURRENT_SHM_CONFIG_MODE");
                    WPFA_D_ASSERT(0);
                    return 0;
            }

            mtkLogD(WPFA_D_LOG_TAG, "pVersionData->dl_buffer_size:%d, ul_buffer_size:%d",
                    pVersionData->dl_buffer_size, pVersionData->ul_buffer_size);

            sp<WpfaDriverMessage> msg = WpfaDriverMessage::obtainMessage(
                    MSG_A2M_WPFA_VERSION, generateDriverTid(), CCCI_IP_TABLE_MSG, 0,
                    WpfaDriverVersionData(pVersionData, 1));
            retValue = mWpfaDriverAdapter->sendMsgToControlMsgDispatcher(msg);
            free(pVersionData);
        }
    }
    return retValue;
}

int WpfaDriver::sendDataPackageToModem(WpfaRingBuffer* ringBuffer) {
    int retValue = checkShmControllerState();
    if (retValue == 0) {
        retValue = mWpfaShmSynchronizer->wirteDataToShm(ringBuffer);
    }
    return retValue;
}

int WpfaDriver::checkDriverAdapterState() {
    int retValue = 0;
    if (mWpfaDriverAdapter != NULL) {
        return retValue;
    } else {
        mWpfaDriverAdapter = WpfaDriverAdapter::getInstance();
        if (mWpfaDriverAdapter == NULL) {
            mtkLogE(WPFA_D_LOG_TAG, "getInstance of WpfaDriverAdapter fail");
            retValue = -1;
        }
    }
    return retValue;
}

int WpfaDriver::checkShmControllerState() {
    int retValue = 0;
    if (mWpfaShmSynchronizer != NULL) {
        return retValue;
    } else {
        mWpfaShmSynchronizer = WpfaShmSynchronizer::getInstance();
        if (mWpfaShmSynchronizer == NULL) {
            mtkLogE(WPFA_D_LOG_TAG, "getInstance of checkShmControllerState fail");
            retValue = -1;
        }
    }
    return retValue;
}

uint16_t WpfaDriver::generateDriverTid() {
    uint16_t newTid = mDriverTid;
    newTid = newTid + 1;
    if (newTid >= DRIVER_TID_END) {
        newTid = DRIVER_TID_START;
    }
    mDriverTid = newTid;
    mtkLogD(WPFA_D_LOG_TAG, "generateDriverTid() newTid=%d", newTid);
    return newTid;
}
