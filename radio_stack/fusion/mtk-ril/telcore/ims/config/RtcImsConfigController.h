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

#ifndef __RFX_IMS_CONFIGSTORAGE_H__
#define __RFX_IMS_CONFIGSTORAGE_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include <map>
#include "RfxController.h"
#include <memory>
#include <string>
#include <mtkconfigdef.h>

/*****************************************************************************
 * Import
 *****************************************************************************/
using std::make_shared;
using std::shared_ptr;
using std::string;

/*****************************************************************************
 * Class RtcImsConfigController
 *****************************************************************************/
class RtcImsConfigController : public RfxController {
    RFX_DECLARE_CLASS(RtcImsConfigController);  // Required: declare this class

  public:
    RtcImsConfigController(){};
    virtual ~RtcImsConfigController(){};

    // Override
  protected:
    virtual void onInit();
    virtual void onDeinit();
    virtual bool onHandleRequest(const sp<RfxMessage>& message);
    virtual bool onHandleUrc(const sp<RfxMessage>& message);
    virtual bool onHandleResponse(const sp<RfxMessage>& message);
    virtual bool onCheckIfRejectMessage(const sp<RfxMessage>& message, bool isModemPowerOff,
                                        int radioState);

  private:
    void initProvisionValue();
    void setFeatureValue(const sp<RfxMessage>& message);
    void handleSetFeatureValue(bool success, const sp<RfxMessage>& message);
    void getFeatureValue(const sp<RfxMessage>& message);
    void handleGetFeatureValue(bool success, int value, const sp<RfxMessage>& message);
    void setProvisionValue(const sp<RfxMessage>& messag);
    bool handleImsConfigExt(const sp<RfxMessage>& message);
    void handleSetProvisionValue(bool success, const sp<RfxMessage>& message);
    void handleSetProvisionResponse(const sp<RfxMessage>& message);
    void getProvisionValue(const sp<RfxMessage>& message);
    void handleGetProvisionResponse(const sp<RfxMessage>& message);
    void handleGetProvisionUrc(const sp<RfxMessage>& message);
    void setImsResourceCapability(int feature_id, int value, bool isDynamicImsSwitchLast);
    void getImsResourceCapability(const sp<RfxMessage>& message);
    void handleGetImsResourceCapability(bool success, int value, const sp<RfxMessage>& message);
    void triggerImsCfgCommand(int slot_id);

    void onSimStateChanged(RfxStatusKeyEnum key, RfxVariant old_value, RfxVariant value);
    void onRadioStateChanged(RfxStatusKeyEnum key, RfxVariant old_value, RfxVariant value);
    void onCarrierConfigChanged(RfxStatusKeyEnum key, RfxVariant old_value, RfxVariant value);

    void resetFeatureSendCmd();
    void resetFeatureResource();
    void resetFeatureValue();
    void resetFeatureSendValue();

    string convertToString(int i);
    bool ensureStorageInitStatus(const sp<RfxMessage>& message);
    void processDynamicImsSwitch();
    int getCurrentMccMnc();

    void saveProvisionedValue();
    void loadProvisionedValue();
    void resetProvisionedValue();

    bool isLogEnable();

  private:
    std::map<int, bool> mFeatureSendCmd;
    std::map<int, int> mFeatureResource;           // <Resource Id, Resource Value>
    std::map<int, int> mVoLteFeatureValue;         // <Network Id, Feature Value>
    std::map<int, int> mViLteFeatureValue;         // <Network Id, Feature Value>
    std::map<int, int> mVoWifiFeatureValue;        // <Network Id, Feature Value>
    std::map<int, int> mViWifiFeatureValue;        // <Network Id, Feature Value>
    std::map<int, int> mVoNrFeatureValue;          // <Network Id, Feature Value>
    std::map<int, int> mViNrFeatureValue;          // <Network Id, Feature Value>
    std::map<int, std::string> mProvisionValue;    // <Provision Id, Provision Value>
    std::map<int, std::string> mProvisionedValue;  // <Provisioned Id, Provisioned Value>
    std::map<int, int> mFeatureSendValue;

    bool mECCAllowSendCmd = false;
    bool mECCAllowNotify = false;
    bool mInitDone = false;
    int mMccmnc = -1;
    sp<RfxMessage> mSetProvisionMessage;
    int mGetProvisionId = 0;
    ;
    sp<RfxMessage> mGetProvisionMessage;
    int mSendCfgMccmnc = -1;  // Use to check if call triggerImsCfgCommand() directly.
    String8 mSendCfgIccid;    // Use to check if call triggerImsCfgCommand() directly.

    bool DEBUG = false;

    typedef enum { ISLAST_NULL = -1, ISLAST_FALSE = 0, ISLAST_TRUE = 1 } ENUM_ISLAST;
};

#endif /* __RFX_IMS_CONFIGSTORAGE_H__ */
