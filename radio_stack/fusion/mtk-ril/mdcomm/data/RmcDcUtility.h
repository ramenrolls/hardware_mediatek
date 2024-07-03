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

#ifndef __RMC_DC_UTILITY_H__
#define __RMC_DC_UTILITY_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "RmcDataDefs.h"
#include "RfxLog.h"
#include <string>
#include <stdlib.h>
#include "rfx_properties.h"
#include <telephony/mtk_ril.h>

#define APN_TYPE_INVALID -1

/*****************************************************************************
 * Class RmcDcUtility
 *****************************************************************************/
class RmcDcUtility {
  public:
    static int isOp07Support();
    static int isOp12Support();
    static int isOp16Support();
    static int isOp20Support();
    static int getAddressType(char* addr);
    static const char* getProfileType(const int profileType);
    static const char* getMtkProfileType(const int profileType);
    static int getApnTypeId(const int profileType);
    static int getMtkApnTypeId(const int profileType);
    static const char* getApnType(int apnTypeId);
    static int getProfileID(int apnTypeId);
    static int getProtocolType(const char* protocol);
    static const char* getProtocolName(int protocol);
    static int getProtocolClassBitmap(int protocol);
    static int getAuthType(int authType);
    static int stringToBinaryBase(char* str, int base, int* err);
    static int isImsSupport();
    static const char* addrTypeToString(ADDRESS_TYPE addrType);
    static const char* pdnTypeToString(int pdnType);
    static void getPropertyBySlot(int slotId, const char* pPropertyName, char* pPropertyValue);
    static const char* checkRcsSupportPcscf();
    static int isOp12MccMnc(const char* mccmnc);
    static bool isUserBuild();
    static bool isSupportOverrideModemDefaultApn();
    static bool isSupportDefaultImsApnSettings();
    static bool isWifiApnExplicitlyConfigured();
    static bool isSupportSscMode(int desireMode);
};
#endif /* __RMC_DC_UTILITY_H__ */
