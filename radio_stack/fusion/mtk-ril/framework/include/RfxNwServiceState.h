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

#ifndef __RFX_NW_SERVICE_STATE_H
#define __RFX_NW_SERVICE_STATE_H

#define RIL_REG_STATE_NOT_REG_NOT_SEARCHING 0
#define RIL_REG_STATE_HOME 1
#define RIL_REG_STATE_NOT_REG_SEARCHING 2
#define RIL_REG_STATE_DENIED 3
#define RIL_REG_STATE_UNKNOWN 4
#define RIL_REG_STATE_ROAMING 5
#define RIL_REG_STATE_NOT_REG_EMERGENCY_CALL_ENABLED 10
#define RIL_REG_STATE_SEARCHING_EMERGENCY_CALL_ENABLED 12
#define RIL_REG_STATE_DENIED_EMERGENCY_CALL_ENABLED 13
#define RIL_REG_STATE_UNKNOWN_EMERGENCY_CALL_ENABLED 14

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "Rfx.h"
#include "utils/String8.h"

using ::android::String8;

/*****************************************************************************
 * Class RfxController
 *****************************************************************************/
class RfxNwServiceState {
  public:
    RfxNwServiceState();

    RfxNwServiceState(int rilVoiceRegState, int rilDataRegState, int rilVoiceRadioTech,
                      int rilDataRadioTech);

    RfxNwServiceState(int rilVoiceRegState, int rilDataRegState, int rilVoiceRadioTech,
                      int rilDataRadioTech, int rilIwlanRegState);

    ~RfxNwServiceState();

  public:
    int getRilVoiceRegState();
    int getRilDataRegState();
    int getRilVoiceRadioTech();
    int getRilDataRadioTech();
    void setRilVoiceRegState(int rilVoiceRegState);
    void setRilDataRegState(int rilDataRegState);
    void setRilVoiceRadioTech(int rilVoiceRadioTech);
    void setRilDataRadioTech(int rilDataRadioTech);
    // for iwlan
    int getIwlanRegState();
    String8 toString();

  private:
    int mRilVoiceRegState;
    int mRilDataRegState;
    int mRilVoiceRadioTech;
    int mRilDataRadioTech;
    // for iwlan, 0: disabled, 1: enabled.
    int mRilIwlanRegState;

  public:
    static bool isInService(int regState) {
        if (regState == 1 || regState == 5) {
            return true;
        }
        return false;
    }

    static bool isGsmGroup(int radioTech) {
        if (radioTech == RADIO_TECH_GPRS || radioTech == RADIO_TECH_EDGE ||
            radioTech == RADIO_TECH_UMTS || radioTech == RADIO_TECH_HSDPA ||
            radioTech == RADIO_TECH_HSUPA || radioTech == RADIO_TECH_HSPA ||
            radioTech == RADIO_TECH_LTE || radioTech == RADIO_TECH_HSPAP ||
            radioTech == RADIO_TECH_GSM || radioTech == RADIO_TECH_LTE_CA ||
            radioTech == RADIO_TECH_NR || radioTech == RADIO_TECH_TD_SCDMA) {
            return true;
        }
        return false;
    }

    static bool isCdmaGroup(int radioTech) {
        if (radioTech == RADIO_TECH_IS95A || radioTech == RADIO_TECH_IS95B ||
            radioTech == RADIO_TECH_1xRTT || radioTech == RADIO_TECH_EVDO_0 ||
            radioTech == RADIO_TECH_EVDO_A || radioTech == RADIO_TECH_EVDO_B ||
            radioTech == RADIO_TECH_EHRPD) {
            return true;
        }
        return false;
    }

    bool equalTo(const RfxNwServiceState& other) const {
        return (mRilVoiceRegState == other.mRilVoiceRegState) &&
               (mRilDataRegState == other.mRilDataRegState) &&
               (mRilVoiceRadioTech == other.mRilVoiceRadioTech) &&
               (mRilDataRadioTech == other.mRilDataRadioTech) &&
               (mRilIwlanRegState == other.mRilIwlanRegState);
    }

    RfxNwServiceState& operator=(const RfxNwServiceState& other) {
        mRilVoiceRegState = other.mRilVoiceRegState;
        mRilDataRegState = other.mRilDataRegState;
        mRilVoiceRadioTech = other.mRilVoiceRadioTech;
        mRilDataRadioTech = other.mRilDataRadioTech;
        mRilIwlanRegState = other.mRilIwlanRegState;
        return *this;
    }

    bool operator==(const RfxNwServiceState& other) { return equalTo(other); }

    bool operator!=(const RfxNwServiceState& other) { return !equalTo(other); }
};

#endif /* __RFX_NW_SERVICE_STATE_H */
