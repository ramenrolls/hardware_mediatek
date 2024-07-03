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

#ifndef WPAF_DRIVER_H
#define WPAF_DRIVER_H

#include <map>
#include <mtk_log.h>

#include "Mutex.h"

#include "WpfaDriverAdapter.h"
#include "WpfaDriverMessage.h"
#include "WpfaRingBuffer.h"
#include "WpfaShmSynchronizer.h"

using ::android::Mutex;

/*
 * =============================================================================
 *                     Defines
 * =============================================================================
 */
#define DRIVER_TID_START (0)
#define DRIVER_TID_END (99)

#ifndef WPFA_D_ASSERT
#define WPFA_D_ASSERT(_expr)                                             \
    do {                                                                 \
        if (!(_expr)) {                                                  \
            mtkLogE("Wpfa", "DRIVER_ASSERT:%s, %d", __FILE__, __LINE__); \
            mtkAssert(NULL);                                             \
            exit(0);                                                     \
        }                                                                \
    } while (0)
#endif

/*
 * =============================================================================
 *                     typedef
 * =============================================================================
 */

/**
 * callback function prototype
 */
typedef int (*CallbackFunc)(void*);

/**
 * callback function structure
 */
struct CallbackStruc {
    CallbackFunc cb;
};

/*
 * =============================================================================
 *                     class
 * =============================================================================
 */
class WpfaDriver {
  public:
    WpfaDriver();
    virtual ~WpfaDriver();
    static WpfaDriver* getInstance();
    void init();

    int registerCallback(event_id_enum eventId, CallbackFunc callbackFunc);
    int unregisterCallback(event_id_enum eventId);
    int notifyCallback(event_id_enum eventId, void* notifyArg);

    int notifyWpfaInit();
    int notifyWpfaVersion(uint16_t apVer, uint16_t mdVer);

    int sendDataPackageToModem(WpfaRingBuffer* ringBuffer);

  protected:
  private:
    int checkDriverAdapterState();
    int checkShmControllerState();
    int addCallback(event_id_enum eventId, CallbackFunc callbackFunc);

    // generate transaction id for SHM writer
    uint16_t generateDriverTid();

    /**
     * singleton pattern
     */
    static WpfaDriver* sInstance;
    static Mutex sWpfaDriverInitMutex;

    std::map<int, CallbackStruc> mapCallbackFunc;

    WpfaDriverAdapter* mWpfaDriverAdapter;
    WpfaShmSynchronizer* mWpfaShmSynchronizer;

    uint16_t mDriverTid;
};

#endif /* end of WPAF_DRIVER_H */
