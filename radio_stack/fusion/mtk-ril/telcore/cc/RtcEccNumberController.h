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

#ifndef __RTC_ECC_NUMBER_CONTROLLER_H__
#define __RTC_ECC_NUMBER_CONTROLLER_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "RfxController.h"
#include "RfxTimer.h"
#include "RfxVoidData.h"
#include "RfxStringData.h"
#include "RfxStringsData.h"
#include "RfxIntsData.h"
#include "RfxMessageId.h"
#include "RfxAtLine.h"
#include "rfx_properties.h"
#include <string>
#include <vector>
#include "EccNumberSource.h"

using namespace std;

/*****************************************************************************
 * Class RfxController
 *****************************************************************************/
class RtcEccNumberController : public RfxController {
    // Required: declare this class
    RFX_DECLARE_CLASS(RtcEccNumberController);

  public:
    RtcEccNumberController();
    virtual ~RtcEccNumberController();

    // Override
  protected:
    virtual bool onHandleUrc(const sp<RfxMessage>& message);
    virtual void onInit();
    virtual bool onHandleRequest(const sp<RfxMessage>& message);
    // virtual bool onHandleResponse(const sp<RfxMessage>& message);
    virtual bool onCheckIfRejectMessage(const sp<RfxMessage>& message, bool isModemPowerOff,
                                        int radioState);

    // External Method
  public:
    // Get the emergency call routing for the corresponding emergency number
    //
    // RETURNS:
    //     UNKNOWN if the number is not recognized as emergency emergency number in RILD
    //     EMERGENCY if the number should dial using emergency call routing
    //     NORMAL if the number should dial using normal call routing
    EmergencyCallRouting getEmergencyCallRouting(String8 number);

    // Check if given number is a emergency number or not based on SIM/Network/XML/Default
    // emergency number list.
    // NOTE: For speical emergency number which should dial using normal call
    //       This API will return true
    //
    // RETURNS: true if number is emergency number
    bool isEmergencyNumber(String8 number);

    // Get service category of emergency number
    //
    // RETURNS: service category value defined 3gpp 22.101, Section 10 - Emergency Calls
    int getServiceCategory(String8 number);
    string getPlmn(int slot);

    bool isSimInserted();
    void updateEmergencySourcesForAllSimStateChanges(bool isSimInserted);
    void updateEmergencyNumber();

  private:
    void onCardTypeChanged(RfxStatusKeyEnum key, RfxVariant oldValue, RfxVariant newValue);
    void onPlmnChanged(RfxStatusKeyEnum key, RfxVariant old_value, RfxVariant value);
    void onSimRecovery(RfxStatusKeyEnum key, RfxVariant old_value, RfxVariant value);
    void onSimStateChanged(RfxStatusKeyEnum key, RfxVariant old_value, RfxVariant value);
    void onConnectionStateChanged(RfxStatusKeyEnum key, RfxVariant old_value, RfxVariant value);
    void onGsmUiccMccMncChanged(RfxStatusKeyEnum key, RfxVariant old_value, RfxVariant value);
    void onCdmaUiccMccMncChanged(RfxStatusKeyEnum key, RfxVariant old_value, RfxVariant value);
    void handleGsmSimEcc(const sp<RfxMessage>& message);
    void handleC2kSimEcc(const sp<RfxMessage>& message);
    void parseSimEcc(RfxAtLine* line, bool isGsm);
    bool isCdmaCard(int cardType);
    void handleSetEccNum(const sp<RfxMessage>& message);
    void handleGetEccNum(const sp<RfxMessage>& message);
    void handleUpdateNetworkEmergencyNumber(const sp<RfxMessage>& message);
    void initEmergencyNumberSource();
    bool isEccMatchInList(String8 number, String8 eccList);
    void createEmergencyNumberListResponse(RIL_EmergencyNumber* data);
    void freeEmergencyNumberListResponse(RIL_EmergencyNumber* data);
    string getSourcesString(int sources);
    void updateSpecialEmergencyNumber();
    void delaySetFwkReady();
    string convertPlmnForRoaming(string plmn);
    bool isCtCard();
    bool isNeedCtaEcc();
    void dumpEccList();
    void testEcc();
    bool updateEmergencySourcesForPlmnChange(bool isSimInsert);
    bool updateEmergencySourcesFromPeer(string plmn);
    void removeNonEmergencyNumbers();
    bool hasSimInAnySlot();
    void updateForSimStateChanges(bool isSimInserted);

  private:
    String8 mGsmEcc;
    String8 mC2kEcc;
    RfxAtLine* mCachedGsmUrc;
    RfxAtLine* mCachedC2kUrc;
    bool mIsSimInsert;
    vector<EccNumberSource*> mEccNumberSourceList;
    SimEccNumberSource* mSimEccSource;
    NetworkEccNumberSource* mNetworkEccSource;
    DefaultEccNumberSource* mDefaultEccSource;
    XmlEccNumberSource* mXmlEccSource;
    FrameworkEccNumberSource* mFrameworkEccSource;
    OemPropertyEccNumberSource* mPropertyEccSource;
    TestEccNumberSource* mTestEccSource;
    CtaEccNumberSource* mCtaEccSource;
    vector<EmergencyNumber> mEccList;
    bool mIsFwkReady;  // Indicate if framework is ready to receive emergency number
    bool mIsPendingUpdate;
    TimerHandle mTimerHandle;
};

#endif /* __RTC_ECC_NUMBER_CONTROLLER_H__ */
