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

#ifndef __RFX_CARRIER_CONFIG_CONTROLLER_H__
#define __RFX_CARRIER_CONFIG_CONTROLLER_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "RfxController.h"
#include "carrierconfig.h"

/*****************************************************************************
 * Class RfxController
 *****************************************************************************/

class RtcCarrierConfigController : public RfxController {
    // Required: declare this class
    RFX_DECLARE_CLASS(RtcCarrierConfigController);

  public:
    RtcCarrierConfigController();
    virtual ~RtcCarrierConfigController();
    bool responseToRilj(const sp<RfxMessage>& message);

    // Override
  protected:
    virtual void onInit();
    virtual void onDeinit();
    virtual bool onHandleRequest(const sp<RfxMessage>& message);
    virtual bool onHandleUrc(const sp<RfxMessage>& message);
    virtual bool onHandleResponse(const sp<RfxMessage>& message);

  private:
    void onUiccGsmMccMncChanged(RfxStatusKeyEnum key, RfxVariant oldValue, RfxVariant value);
    void onUiccCdmaMccMncChanged(RfxStatusKeyEnum key, RfxVariant oldValue, RfxVariant value);
    void onCardTypeChanged(RfxStatusKeyEnum key, RfxVariant oldValue, RfxVariant value);
    void onNwsModeChanged(RfxStatusKeyEnum key, RfxVariant oldValue, RfxVariant value);
    void loadConfiguration(const char* mccmnc);

    /* Native Carrier Config */
    void* dlHandle;
    unsigned int (*fnGetKeyCount)(const char*);
    int (*fnGetValuesByMccMnc)(const char*, CarrierConfigValue*);
    void freeCarrierConfigValue(CarrierConfigValue* data, int count);
};

#endif /* __RFX_CARRIER_CONFIG_CONTROLLER_H__ */
