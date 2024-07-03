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

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "RfxNwServiceState.h"

/*****************************************************************************
 * Class RfxController
 *****************************************************************************/

RfxNwServiceState::RfxNwServiceState() {
    mRilVoiceRegState = 0;
    mRilDataRegState = 0;
    mRilVoiceRadioTech = 0;
    mRilDataRadioTech = 0;
    mRilIwlanRegState = 0;
}

RfxNwServiceState::RfxNwServiceState(int rilVoiceRegState, int rilDataRegState,
                                     int rilVoiceRadioTech, int rilDataRadioTech) {
    mRilVoiceRegState = rilVoiceRegState;
    mRilDataRegState = rilDataRegState;
    mRilVoiceRadioTech = rilVoiceRadioTech;
    mRilDataRadioTech = rilDataRadioTech;
    mRilIwlanRegState = 0;
}
// for iwlan
RfxNwServiceState::RfxNwServiceState(int rilVoiceRegState, int rilDataRegState,
                                     int rilVoiceRadioTech, int rilDataRadioTech,
                                     int rilIwlanRegState) {
    mRilVoiceRegState = rilVoiceRegState;
    mRilDataRegState = rilDataRegState;
    mRilVoiceRadioTech = rilVoiceRadioTech;
    mRilDataRadioTech = rilDataRadioTech;
    mRilIwlanRegState = rilIwlanRegState;
}

RfxNwServiceState::~RfxNwServiceState() {}

int RfxNwServiceState::getRilVoiceRegState() { return mRilVoiceRegState; }

void RfxNwServiceState::setRilVoiceRegState(int rilVoiceRegState) {
    mRilVoiceRegState = rilVoiceRegState;
}

int RfxNwServiceState::getRilDataRegState() { return mRilDataRegState; }

void RfxNwServiceState::setRilDataRegState(int rilDataRegState) {
    mRilDataRegState = rilDataRegState;
}

int RfxNwServiceState::getRilVoiceRadioTech() { return mRilVoiceRadioTech; }

void RfxNwServiceState::setRilVoiceRadioTech(int rilVoiceRadioTech) {
    mRilVoiceRadioTech = rilVoiceRadioTech;
}

int RfxNwServiceState::getRilDataRadioTech() { return mRilDataRadioTech; }

void RfxNwServiceState::setRilDataRadioTech(int rilDataRadioTech) {
    mRilDataRadioTech = rilDataRadioTech;
}

int RfxNwServiceState::getIwlanRegState() { return mRilIwlanRegState; }

String8 RfxNwServiceState::toString() {
    return String8::format("cs_reg=%d, cs_rat=%d, ps_reg=%d, ps_rat=%d, iwlan_reg=%d",
                           mRilVoiceRegState, mRilVoiceRadioTech, mRilDataRegState,
                           mRilDataRadioTech, mRilIwlanRegState);
}
