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

#include "SSConfig.h"
#include "RfxLog.h"

#include <string.h>

#ifdef RFX_LOG_TAG
#undef RFX_LOG_TAG
#endif
#define RFX_LOG_TAG "SS-Config"

const char* SSConfig::NOT_SHOW_FORWARDED_TOAST[] = {
        /* Verizon */
        "310004", "310005", "310006", "310010", "310012", "310013", "310350", "310590",
        "310820", "310890", "310910", "311012", "311110", "311270", "311271", "311272",
        "311273", "311274", "311275", "311276", "311277", "311278", "311279", "311280",
        "311281", "311282", "311283", "311284", "311285", "311286", "311287", "311288",
        "311289", "311390", "311480", "311481", "311482", "311483", "311484", "311485",
        "311486", "311487", "311488", "311489", "311590", "312770"};

const char* SSConfig::USSI_WITH_NO_LANG[] = {
        /* Smart Cambodia */
        "45606"};

const char* SSConfig::CONVERT_409_TO_NOT_SUPPROT_CALLER_ID[] = {
        /* CMCC */
        "46000", "46002", "46004", "46007", "46008",
        /* CU */
        "46001", "46006", "46009", "45407"};

const char* SSConfig::CONVERT_409_TO_NOT_SUPPROT_CB[] = {
        /* CMCC */
        "46000", "46002", "46004", "46007", "46008",
        /* CT */
        "45502", "46003", "46011", "46012", "45507"};

const char* SSConfig::NOT_SUPPORT_USSI_OVER_CALL[] = {
        /* Mobitel */
        "41301"};

bool SSConfig::isInList(const char* mccmnc, char const* list[], unsigned long size) {
    if (mccmnc == NULL) {
        return false;
    }

    for (unsigned long i = 0; i < size; i++) {
        if (strcmp(mccmnc, list[i]) == 0) {
            return true;
        }
    }
    return false;
}

/**
 * Do not poppup "It's a forwarded call" toast even when ECMCCSS 257 received
 */
bool SSConfig::notShowForwardedToast(const char* mccmnc) {
    bool r = isInList(mccmnc, NOT_SHOW_FORWARDED_TOAST,
                      sizeof(NOT_SHOW_FORWARDED_TOAST) / sizeof(char*));
    RFX_LOG_D(RFX_LOG_TAG, "notShowForwardedToast, mccmnc = %s, r = %s", mccmnc,
              r ? "True" : "False");
    return r;
}

/**
 * Send USSI without language info
 */
bool SSConfig::ussiWithNoLang(const char* mccmnc) {
    bool r = isInList(mccmnc, USSI_WITH_NO_LANG, sizeof(USSI_WITH_NO_LANG) / sizeof(char*));
    RFX_LOG_D(RFX_LOG_TAG, "ussiWithNoLang, mccmnc = %s, r = %s", mccmnc, r ? "True" : "False");
    return r;
}

/**
 * For some operator which not support Caller ID, need to convert the received "409 conflict error"
 * to "request not support"
 */
bool SSConfig::convert409ToNotSupportCallerID(const char* mccmnc) {
    bool r = isInList(mccmnc, CONVERT_409_TO_NOT_SUPPROT_CALLER_ID,
                      sizeof(CONVERT_409_TO_NOT_SUPPROT_CALLER_ID) / sizeof(char*));
    RFX_LOG_D(RFX_LOG_TAG, "convert409ToNotSupportCallerID, mccmnc = %s, r = %s", mccmnc,
              r ? "True" : "False");
    return r;
}

/**
 * For some operator which not support call barring, need to convert the received "409 conflict
 * error" to "request not support"
 */
bool SSConfig::convert409ToNotSupportCB(const char* mccmnc) {
    bool r = isInList(mccmnc, CONVERT_409_TO_NOT_SUPPROT_CB,
                      sizeof(CONVERT_409_TO_NOT_SUPPROT_CB) / sizeof(char*));
    RFX_LOG_D(RFX_LOG_TAG, "convert409ToNotSupportCB, mccmnc = %s, r = %s", mccmnc,
              r ? "True" : "False");
    return r;
}

/**
 * Operator doesn't support send USSI during call
 */
bool SSConfig::isNotSupportUSSIOverCall(const char* mccmnc) {
    bool r = isInList(mccmnc, NOT_SUPPORT_USSI_OVER_CALL,
                      sizeof(NOT_SUPPORT_USSI_OVER_CALL) / sizeof(char*));
    RFX_LOG_D(RFX_LOG_TAG, "isNotSupportUSSIOverCall, mccmnc = %s, r = %s", mccmnc,
              r ? "True" : "False");
    return r;
}
