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

#ifndef _IMSCONFIGUTILS_H_
#define _IMSCONFIGUTILS_H_
/*****************************************************************************
 * Include
 *****************************************************************************/
#include <string>
#include <String8.h>
#include "RfxDefs.h"

using ::android::String8;
/*****************************************************************************
 * Class RtcImsConfigUtils
 *****************************************************************************/

typedef struct {
    int mccmnc;
    int opId;
} ConfigOperatorTable;

class ImsConfigUtils {
  public:
    static int getSystemPropValue(const char* propName);
    static std::string getSystemPropStringValue(const char* propName);
    static int getFeaturePropValue(const char* propName, int slot_id);
    static void setFeaturePropValue(const char* propName, char* enabled, int slot_id);
    static int setBitForPhone(int featureValue, int enabled, int slot_id);

    static bool isMultiImsSupport();
    static bool checkIsPhoneIdValid(int slot_id);

    static bool isTestSim(int slot_id);
    static bool isCtVolteDisabled(int slot_id);
    static bool isPhoneIdSupportIms(int slot_id);
    static int getOperatorId(int mccmnc);
    static bool isAllowForceNotify(int slot_id, int value);
    static void setAllowForceNotify(int slot_id, bool allow, int value);

  private:
    static void printLog(int level, String8 log, int slot_id);

  public:
    static const char* PROPERTY_IMS_SUPPORT;
    static const char* PROPERTY_VOLTE_ENALBE;
    static const char* PROPERTY_WFC_ENALBE;
    static const char* PROPERTY_VILTE_ENALBE;
    static const char* PROPERTY_VIWIFI_ENALBE;
    static const char* PROPERTY_VONR_ENALBE;
    static const char* PROPERTY_VINR_ENALBE;
    static const char* PROPERTY_IMS_VIDEO_ENALBE;
    static const char* PROPERTY_MULTI_IMS_SUPPORT;
    static const char* PROPERTY_DYNAMIC_IMS_SWITCH_SUPPORT;
    static const char* PROPERTY_CTVOLTE_ENABLE;

    static const char PROPERTY_ICCID_SIM[4][25];
    static const char PROPERTY_TEST_SIM[4][30];

  private:
    static const bool DEBUG_ENABLED;
    static bool mForceNotify[4];
    static int mForceValue[4];
};

#endif
