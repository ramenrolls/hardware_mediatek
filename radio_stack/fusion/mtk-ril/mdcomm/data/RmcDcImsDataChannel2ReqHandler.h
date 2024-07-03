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

#ifndef __RMC_DC_IMS_DATA_CHANNEL2_REQ_HANDLER_H__
#define __RMC_DC_IMS_DATA_CHANNEL2_REQ_HANDLER_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "RfxBaseHandler.h"
#include "RfxController.h"
#include "RfxStatusDefs.h"
#include "rfx_properties.h"

/*****************************************************************************
 * Class RmcDcImsDataChannel2ReqHandler
 *****************************************************************************/
class RmcDcImsDataChannel2ReqHandler : public RfxBaseHandler {
    RFX_DECLARE_HANDLER_CLASS(RmcDcImsDataChannel2ReqHandler);

  public:
    RmcDcImsDataChannel2ReqHandler(int slot_id, int channel_id);
    virtual ~RmcDcImsDataChannel2ReqHandler();

  protected:
    virtual void onHandleRequest(const sp<RfxMclMessage>& msg);
    virtual void onHandleEvent(const sp<RfxMclMessage>& msg);

    // URC +EIMSPDN "notify" URC
    virtual void handleImsBearerNotify(const sp<RfxMclMessage>& msg);
    // AT+EIMSPDN activation/deactivation confirm to DDM
    virtual void handleImsBearerConfirm(const sp<RfxMclMessage>& msg);
    // Set flag to control "+EIMSPDN " notification to ap
    virtual void handleImsBearerNotificationSet(const sp<RfxMclMessage>& msg);
    // P-CSCF discovery, not support now
    virtual void handlePcscfAddressDiscovery(const sp<RfxMclMessage>& msg);
    // Callback function when RmcDcUrcHandler URC registration ready
    virtual void handleRegisterUrcDone();
    virtual void notifyImsBearerRequest(int requestId, RIL_IMS_BearerNotification* notification,
                                        bool isUpdateState);
    virtual void notifyImsDataInfo(RIL_IMS_DataInfoNotify* notification);

    // handle unsynchronized scenario after phone process/ims service has been kill
    virtual void notifyUnsyncImsBearerRequest();
    // handle unsynchronized scenario after phone/ims service has been kill deu to phone decrypt
    virtual void checkImsServiceRestartAfterSystemCrash();

  private:
    int isEnableBearerNotification = 0;
    int isEncrypted = 0;
};
#endif /* __RMC_DC_IMS_DATA_CHANNEL2_REQ_HANDLER_H__ */
