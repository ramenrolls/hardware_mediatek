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

#ifndef __RMC_CALL_CONTROL_BASE_HANDLER_H__
#define __RMC_CALL_CONTROL_BASE_HANDLER_H__

#include "RfxBaseHandler.h"

class RfxAtLine;

// This class provide the general function for request handler
class RmcCallControlBaseHandler : public RfxBaseHandler {
  public:
    RmcCallControlBaseHandler(int slot_id, int channel_id);
    virtual ~RmcCallControlBaseHandler();

  protected:
    /*
     * Convert clir mode to the clir prefix
     */
    char* getClirPrefix(int clir);
    /*
     * Response a void data with error to the TCL
     */
    void responseVoidDataToTcl(const sp<RfxMclMessage>& msg, RIL_Errno err);

    void setVDSAuto(bool isAuto, bool isEcc);
    void enableVDSReport(bool enable = true);
    bool hasImsCall(int slotId);

    char* extractSipUri(char* num);

    bool isGsmRat();
    bool isCdmaRat();
    String8 handleNumberWithPause(char* number);
    String8 handleNumberWithClirString(String8 number);
    bool isVzWProject();
    int handleClirSpecial(bool isEcc, int clir, char* number);
};

#endif
