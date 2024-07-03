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

#ifndef __RFX_REDIAL_DATA__H__
#define __RFX_REDIAL_DATA__H__

#include "RfxBaseData.h"
#include "RfxDialData.h"
#include <telephony/mtk_ril.h>

typedef struct {
    RIL_Dial* dial_data;
    int call_id;
} RFX_Redial;

class RfxRedialData : public RfxBaseData {
    RFX_DECLARE_DATA_CLASS(RfxRedialData);

  public:
    RfxRedialData(RIL_Dial* data, int callId);

  public:
    int getCallId() const { return mCallId; }

    int getClir();
    char* getNumber();
    void dump();

  private:
    RfxDialData* mDialData;
    int mCallId;
};

#endif
