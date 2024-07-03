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

#ifndef __RFX_RIL_UTILS__
#define __RFX_RIL_UTILS__

#include <rfx_properties.h>
#include <stdlib.h>
#include <string.h>
#include <RfxLog.h>
#include <RfxDefs.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include "RfxStatusDefs.h"
#include "RfxVariant.h"

#if !defined(PURE_AP_USE_EXTERNAL_MODEM)
#include "hardware/ccci_intf.h"
#endif

#define NUM_ELEMS(x) (sizeof(x) / sizeof(x[0]))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
typedef void (*STATUSCALLBACK)(int slotId, const RfxStatusKeyEnum key, const RfxVariant& value);

#define OPERATOR_KDDI 129
#define MNGMT_PACKET_LENGTH 192

class RfxRilUtils {
  public:
    static int rfxGetSimCount();
    static int isEngLoad();
    static int isUserLoad();
    static int isLteSupport();
    static int isImsSupport();
    static int isMultipleImsSupport();
    static int triggerCCCIIoctlEx(int request, int* param);
    static int triggerCCCIIoctl(int request);
    static int triggerPhantomPacket(String8 s);
    static RilRunMode getRilRunMode();
    static void setRilRunMode(RilRunMode mode);
    static void setStatusValueForGT(int slotId, const RfxStatusKeyEnum key,
                                    const RfxVariant& value);
    static void updateStatusToGT(int slotId, const RfxStatusKeyEnum key, const RfxVariant& value);
    static void setStatusCallbackForGT(STATUSCALLBACK cb);
    /// M: add for op09 volte setting @{
    static bool isOp09();
    static bool isCtVolteSupport();
    static bool isCtMixVolteSupport();
    /// @}
    static int getMajorSim();
    static void printLog(int level, String8 tag, String8 log, int slot);
    static bool isInLogReductionList(int reqId);
    static int handleAee(const char* modem_warning, const char* modem_version);

    // External SIM [Start]
    static int isExternalSimSupport();
    static int isExternalSimOnlySlot(int slot);
    static int isNonDsdaRemoteSupport();
    static int isSwitchVsimWithHotSwap();
    static int isPersistExternalSimDisabled();
    static int isVsimEnabledBySlot(int slot);
    static int isVsimEnabledNotInserted(int slot);
    static bool isVsimEnabled();
    static bool isVsimClientConnected();
    static bool isExternalSimManagerExisted();
    static void setRsimAuthOngoing(int slot, int ongoing);
    static int getRsimAuthOngoing(int slot);
    static int getRemoteSimSlot();
    // External SIM [End]

    static bool isTplusWSupport();
    static int getKeep3GMode();
    static bool isEmciSupport();
    static bool isWfcSupport();
    static bool isVilteSupport();
    static bool isVolteSupport();
    static bool isViwifiSupport();
    static bool isWfcEnable(int slotId);
    static bool isVilteEnable(int slotId);
    static bool isDigitsSupport();
    static bool isSmsSupport();
    static bool isSimSwitchUrc(const char* line);
    // Get the protocol stack id by slot id, the slot id is 0-based, protocol stack id is 1-based
    static int getProtocolStackId(int slotId);

    /// M: GSMA TS.27 13.3.7 @{
    static bool isUiccCLFSupport();
    /// M: GSMA TS.27 13.3.7 @}
    static bool hideStatusLog(const RfxStatusKeyEnum key);
    static void getLogicalModemId(char* modemId, int size, int slotId);

    static bool isGwsdSupport();

    static const char* pii(const char* tag, const char* hiddenData);

  private:
    static const char* boolToString(bool value);
    static char convertCharToHex(char ch);

    static int m_isEngLoad;
    static int m_isChipTest;
    static int m_isInternalLoad;
    static int m_isUserLoad;
    static int mIsC2kSupport;
    static int mIsLteSupport;
    static int mIsImsSupport;
    static int mIsMultiIms;
    /// M: add for op09 volte setting @{
    static int mIsOp09;
    static int mIsCtVolteSupport;
    static int mIsCtMixVolteSupport;
    /// @}
    static RilRunMode m_rilRunMode;
    static STATUSCALLBACK s_statusCallback;
    // External SIM [Start]
    static int mIsRsimAuthOngoing[MAX_SIM_COUNT];
    // External SIM [End]
    static const int MAX_RETRY_COUNT = 20;
};

#endif
