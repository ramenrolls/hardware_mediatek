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

#ifndef __RTC_PHB_SIMIO_CONTROLLER_H__
#define __RTC_PHB_SIMIO_CONTROLLER_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "RfxController.h"

#define PBR_FILE_LENGTH (50)

/*****************************************************************************
 * Class RfxController
 *****************************************************************************/
class RtcPhbSimIoController : public RfxController {
    // Required: declare this class
    RFX_DECLARE_CLASS(RtcPhbSimIoController);

  public:
    RtcPhbSimIoController();
    virtual ~RtcPhbSimIoController();
    bool onCheckIfPhbRequest(int fileId);

    // Override
  protected:
    virtual void onInit();
    virtual bool onCheckIfRejectMessage(const sp<RfxMessage>& message, bool isModemPowerOff,
                                        int radioState);
    virtual bool onHandleResponse(const sp<RfxMessage>& message);

  private:
    int mIsEngLoad = -1;
    int hexCharToDecInt(char* hex, int length);
    bool addFileId(int fileId);
    int parsePbrFileId(char* hex, int length);
    void onCardTypeChanged(RfxStatusKeyEnum key, RfxVariant oldValue, RfxVariant newValue);
    int pbrFile[50] = {0};
};

#endif /* __RTC_PHB_SIMIO_CONTROLLER_H__ */
