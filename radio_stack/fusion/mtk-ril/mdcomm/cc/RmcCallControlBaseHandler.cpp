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

// system include
#include <string>
#include <vector>

// MTK fusion include
#include "RfxRootController.h"
#include "RfxVoidData.h"

// MTK CC local include
#include "RmcCallControlBaseHandler.h"
#include "RmcCallControlInterface.h"

#include "MtkPhoneNumberUtils.h"

#define RFX_LOG_TAG "RmcCCBaseHandler"

#define OPERATOR_VZW "OP12"

RmcCallControlBaseHandler::RmcCallControlBaseHandler(int slot_id, int channel_id)
    : RfxBaseHandler(slot_id, channel_id) {
    // do nothing
}

RmcCallControlBaseHandler::~RmcCallControlBaseHandler() {
    // do nothing
}

char* RmcCallControlBaseHandler::getClirPrefix(int clir) {
    char* clirPrefix = (char*)""; /*subscription default*/
    if (clir == 1) {              /*invocation*/
        clirPrefix = (char*)"#31#";
    } else if (clir == 2) { /*suppression*/
        clirPrefix = (char*)"*31#";
    }
    return clirPrefix;
}

void RmcCallControlBaseHandler::responseVoidDataToTcl(const sp<RfxMclMessage>& msg, RIL_Errno err) {
    // if (err != RIL_E_SUCCESS) {
    //     logE(RFX_LOG_TAG, "%d failed!", msg->getId());
    // }
    sp<RfxMclMessage> mclResponse =
            RfxMclMessage::obtainResponse(msg->getId(), err, RfxVoidData(), msg);
    responseToTelCore(mclResponse);
}

void RmcCallControlBaseHandler::setVDSAuto(bool isAuto, bool isEcc) {
    /*
    AT+EVADSMOD=<mode>,<ecc_preferred_domain>
    <mode>: integer
    0: Automatic mode - LTE(IMS), WG(CS), 1x(C2K)
    1: CS domain only - WG(CS)
    2: 3GPP only - LTE(IMS), WG(CS)
    3: 3GPP2 only - 1x(C2K)
    4: IMS and 1x CS only - LTE(IMS), 1x(C2K)
    5: WG CS and 1x CS only - WG(CS), 1x(C2K)
    <ecc_preferred_domain>: integer
    0: Not preferred
    1: WG CS
    2: 1x CS
    */

    char* atCmd = AT_VDS_MODE;
    int mode = 0;
    int prefMode = 0;

    // prefRat  1:GSM_ONLY, 2:CDMA_ONLY, 3:GSM_PREF, 4:CDMA_PREF
    int prefRat = getMclStatusManager()->getIntValue(RFX_STATUS_KEY_ECC_PREFERRED_RAT, 0);

    if (!isEcc) {
        prefMode = 0;  // no pref for normal call
        if (isAuto) {
            mode = 0;  // auto
            enableVDSReport();
        } else {
            // here it is dialing CS call. No need to check service state.
            mode = 5;
        }
    } else {  // ECC
        if (isAuto) {
            enableVDSReport();
            if (isGsmRat() && prefRat == 3) {  // gsm_pref
                mode = 0;
                prefMode = 1;
            } else if (isGsmRat() || isCdmaRat() || prefRat == 0) {  // ims+gsm+cdma
                mode = 0;
                prefMode = 0;
            } else {                 // no service with pref
                if (prefRat == 1) {  // ims+gsm
                    mode = 2;
                    prefMode = 0;
                } else if (prefRat == 2) {  // ims+cdma
                    mode = 4;
                    prefMode = 0;
                } else if (prefRat == 3) {  // gsm_pref
                    mode = 0;
                    prefMode = 1;
                } else if (prefRat == 4) {  // cdma_pref
                    mode = 0;
                    prefMode = 2;
                }
            }
        } else {
            if (isGsmRat() || isCdmaRat() || prefRat == 0) {  // gsm+cdma
                mode = 5;
                prefMode = 0;
            } else {                 // no service with pref
                if (prefRat == 1) {  // gsm
                    mode = 1;
                    prefMode = 0;
                } else if (prefRat == 2) {  // cdma
                    mode = 3;
                    prefMode = 0;
                } else if (prefRat == 3) {  // gsm_pref
                    mode = 5;
                    prefMode = 1;
                } else if (prefRat == 4) {  // cdma_pref
                    mode = 5;
                    prefMode = 2;
                }
            }
        }
    }

    sp<RfxAtResponse> atResponse =
            atSendCommand(String8::format("%s=%d,%d", atCmd, mode, prefMode));
    // check at cmd result, consider default as success
    if (atResponse->getError() != 0 || atResponse->getSuccess() != 1) {
        logE(RFX_LOG_TAG, "Setup VDS to %d,%d failed", mode, prefMode);
    }
}

void RmcCallControlBaseHandler::enableVDSReport(bool isEnable) {
    /* +EVADSREP=<enable>
     * <enable>: 0 = disable; 1 = enable
     */
    char* atCmd = AT_ENABLE_VDS_REPORT;
    int enableVal = isEnable ? 1 : 0;
    sp<RfxAtResponse> atResponse = atSendCommand(String8::format("%s=%d", atCmd, enableVal));
    // check at cmd result, consider default as success
    if (atResponse->getError()) {
        logE(RFX_LOG_TAG, "Setup VDS Rep to %d failed", enableVal);
    }
}

bool RmcCallControlBaseHandler::hasImsCall(int slotId) {
    RfxRootController* rootController = RFX_OBJ_GET_INSTANCE(RfxRootController);
    RfxStatusManager* slotStatusMgr = rootController->getStatusManager(slotId);
    return slotStatusMgr->getBoolValue(RFX_STATUS_KEY_IMS_CALL_EXIST, false);
}

char* RmcCallControlBaseHandler::extractSipUri(char* num) {
    // sip:0123456;234,4@xxx.com
    char* result;
    result = strsep(&num, ":");
    if (NULL != num) {
        result = num;
    }
    result = strsep(&result, "@");
    result = strsep(&result, ";");
    result = strsep(&result, ",");
    return result;
}

/// C2K specific start
bool RmcCallControlBaseHandler::isGsmRat() {
    bool bGsmRat;
    // RAT can be unknown, neither Gsm or Cdma
    RfxNwServiceState defaultServiceState(0, 0, 0, 0);
    RfxNwServiceState ss = getMclStatusManager()->getServiceStateValue(RFX_STATUS_KEY_SERVICE_STATE,
                                                                       defaultServiceState);
    int voiceRadioTech = ss.getRilVoiceRadioTech();
    bGsmRat = RfxNwServiceState::isGsmGroup(voiceRadioTech);
    if (!bGsmRat) {
        logD(RFX_LOG_TAG, "%s, voiceRadioTech:%d", __FUNCTION__, voiceRadioTech);
    }
    return bGsmRat;
}

// Check if this slot is in CDMA RAT
bool RmcCallControlBaseHandler::isCdmaRat() {
    RfxNwServiceState defaultServiceState(0, 0, 0, 0);
    RfxNwServiceState ss = getMclStatusManager()->getServiceStateValue(RFX_STATUS_KEY_SERVICE_STATE,
                                                                       defaultServiceState);
    int voiceRadioTech = ss.getRilVoiceRadioTech();
    logD(RFX_LOG_TAG, "%s, voiceRadioTech:%d", __FUNCTION__, voiceRadioTech);
    return RfxNwServiceState::isCdmaGroup(voiceRadioTech);
}
/// C2K specific end

String8 RmcCallControlBaseHandler::handleNumberWithPause(char* number) {
    if (number == NULL) {
        return String8::format("");
    }
    char* formatNumber = NULL;
    char* origNumber = NULL;
    int len = strlen(number);
    int pos = 0;
    char* tmp = strchr(number, ',');
    if (tmp != NULL) {
        pos = (tmp - number) / sizeof(char);
        formatNumber = (char*)alloca(pos + 1);
        origNumber = (char*)alloca(len - pos);
        memset(formatNumber, 0, pos + 1);
        memset(origNumber, 0, len - pos);
        strncpy(formatNumber, number, pos);
        strncpy(origNumber, tmp + 1, len - pos - 1);
        if (RfxRilUtils::isUserLoad() != 1) {
            logD(RFX_LOG_TAG, "%s, number:%s, formatNum:%s, origNum:%s", __FUNCTION__,
                 RfxRilUtils::pii(RFX_LOG_TAG, number), RfxRilUtils::pii(RFX_LOG_TAG, formatNumber),
                 RfxRilUtils::pii(RFX_LOG_TAG, origNumber));
        }
        return String8::format("%s", origNumber);
    } else {
        return String8::format("%s", number);
    }
}

// Check if Clir should be modify
int RmcCallControlBaseHandler::handleClirSpecial(bool isEcc, int clir, char* number) {
    String8 priorityString = getMclStatusManager()->getString8Value(
            RFX_STATUS_KEY_HIGH_PRIORITY_CLIR_PREFIX_SUPPORTED);
    String8 ignoreClirWhenEcc =
            getMclStatusManager()->getString8Value(RFX_STATUS_KEY_CARRIER_IGNORE_CLIR_WHEN_ECC);
    logD(RFX_LOG_TAG, "ignoreClirWhenEcc %s", ignoreClirWhenEcc.string());

    bool needSetDefaultWhenPriorityString = strcmp(priorityString.string(), "true") == 0 &&
                                            clir == 1 /*invocation*/ &&
                                            strncmp(number, "*82", 3) == 0;
    bool needSetDefaultWhenEcc = isEcc && (ignoreClirWhenEcc == "1");

    if (needSetDefaultWhenPriorityString || needSetDefaultWhenEcc) {
        clir = 0;  // use subscription default value
    }
    return clir;
}

String8 RmcCallControlBaseHandler::handleNumberWithClirString(String8 number) {
    RfxNwServiceState defaultServiceState(0, 0, 0, 0);
    RfxNwServiceState ss = getMclStatusManager()->getServiceStateValue(RFX_STATUS_KEY_SERVICE_STATE,
                                                                       defaultServiceState);
    if (isVzWProject() && (ss.getRilVoiceRegState() == 5)) {
        if (number.find("*67") == 0) {
            logD(RFX_LOG_TAG, "is *67, covert to #31#");
            const char* tempString = number.string();
            return String8::format("#31#%s", &tempString[3]);
        } else if (number.find("*82") == 0) {
            logD(RFX_LOG_TAG, "is *82, covert to *31#");
            const char* tempString = number.string();
            return String8::format("*31#%s", &tempString[3]);
        }
    }
    return number;
}

bool RmcCallControlBaseHandler::isVzWProject() {
    char optr_value[RFX_PROPERTY_VALUE_MAX] = {0};

    rfx_property_get("persist.vendor.operator.optr", optr_value, "0");

    return (strncmp(optr_value, OPERATOR_VZW, strlen(OPERATOR_VZW)) == 0);
}
