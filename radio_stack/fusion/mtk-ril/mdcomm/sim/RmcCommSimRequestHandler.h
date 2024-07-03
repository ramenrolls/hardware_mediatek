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

#ifndef __RMC_COMM_SIM_REQUEST_HANDLER_H__
#define __RMC_COMM_SIM_REQUEST_HANDLER_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "RmcSimBaseHandler.h"
#include "RmcCommSimDefs.h"
#ifdef __cplusplus
extern "C" {
#endif
#include <vendor/mediatek/ims/radio_stack/platformlib/common/libmtkrilutils/proto/sap-api.pb.h>
#include "pb_decode.h"
#include "pb_encode.h"
#ifdef __cplusplus
}
#endif

/*****************************************************************************
 * Class RpSimController
 *****************************************************************************/

class RmcCommSimRequestHandler : public RmcSimBaseHandler {
    RFX_DECLARE_HANDLER_CLASS(RmcCommSimRequestHandler);

  public:
    RmcCommSimRequestHandler(int slot_id, int channel_id);
    virtual ~RmcCommSimRequestHandler();

    // Check if the handler have to process the Request or not
    virtual RmcSimBaseHandler::SIM_HANDLE_RESULT needHandle(const sp<RfxMclMessage>& msg);

    // Process Request here
    virtual void handleRequest(const sp<RfxMclMessage>& msg);

    virtual void handleEvent(const sp<RfxMclMessage>& msg);

    // Return the list which you want to reqister
    virtual const int* queryTable(int channel_id, int* record_num);
    virtual const int* queryEventTable(int channel_id, int* record_num);

    // BTSAP - START
    void notifyBtSapStatusInd(RIL_SIM_SAP_STATUS_IND_Status message);
    // BTSAP - END

  private:
    static int mOldTestSimValue[MAX_SIM_COUNT];
    // Utility functions
    int openChannel(const sp<RfxMclMessage>& msg, RIL_SIM_IO_Response* sr, int* len);
    int selectAid(RIL_SIM_IO_Response* sr, int* len, int channel, int p2, char* aid);
    int checkRetryFCI(int sw1, int sw2);
    int setChannelToClassByte(int cla, int channelNumber);
    bool getSimAppInfo(int app_id, RIL_AppStatus* p_app_info, UICC_Status uStatus);
    void sleepMsec(long long msec);
    unsigned int htoi(char* szHex);
    void setSimInsertedStatus(int slotId, int isInserted);
    void decodeBtSapPayload(int msgId, void* src, size_t srclen, void* dst);
    void resetBtSapContext();
    char* convertBtSapIntToHexString(uint8_t* data, size_t datalen);
    int toByte(char c);
    int hexStringToByteArrayEx(unsigned char* hexString, int hexStringLen, unsigned char** byte);
    void sendBtSapResponseComplete(const sp<RfxMclMessage>& msg, RIL_Errno ret, int msgId,
                                   void* data);
    bool serviceActivationStatus(int fileId, int appTypeId);
    void querySimOnOffActivation();
    bool isRjioRsuSupport();

    // Request handler
    void handleGetSimStatus(const sp<RfxMclMessage>& msg);
    void handleSimIo(const sp<RfxMclMessage>& msg);
    void handleIsimAuthentication(const sp<RfxMclMessage>& msg);
    void handleSimAuthentication(const sp<RfxMclMessage>& msg);
    void handleGeneralSimAuthentication(const sp<RfxMclMessage>& msg);
    void handleQueryIccid(const sp<RfxMclMessage>& msg);
    void handleSimOpenChannel(const sp<RfxMclMessage>& msg);
    void handleSimCloseChannel(const sp<RfxMclMessage>& msg);
    void handleSimTransmitBasic(const sp<RfxMclMessage>& msg);
    void handleSimTransmitChannel(const sp<RfxMclMessage>& msg);
    int querySimExtendedFormat();
    int queryEcglaCapability();
    void handleSimTransmitWithExtendedAPDU(const sp<RfxMclMessage>& msg, bool isBasicChannel);
    void handleEsimTransmitChannel(const sp<RfxMclMessage>& msg);
    void handleSimGetAtr(const sp<RfxMclMessage>& msg);
    void handleSimGetIccid(const sp<RfxMclMessage>& msg);
    void handleDetectSim(const sp<RfxMclMessage>& msg);
    void handleSecurityOperation(const sp<RfxMclMessage>& msg, UICC_Security_Operation op);
    void handleEnterNetworkDepersonalization(const sp<RfxMclMessage>& msg);
    void handleEnterDepersonalization(const sp<RfxMclMessage>& msg);
    void handleLocalRequestSimAuthentication(const sp<RfxMclMessage>& msg);
    void handleLocalRequestUsimAuthentication(const sp<RfxMclMessage>& msg);
    // BTSAP - START
    void handleLocalBtSapReset(const sp<RfxMclMessage>& msg);
    void handleBtSapConnect(const sp<RfxMclMessage>& msg);
    void handleBtSapDisconnect(const sp<RfxMclMessage>& msg);
    void handleBtSapTransferApdu(const sp<RfxMclMessage>& msg);
    void handleBtSapTransferAtr(const sp<RfxMclMessage>& msg);
    void handleBtSapPower(const sp<RfxMclMessage>& msg);
    void handleBtSapResetSim(const sp<RfxMclMessage>& msg);
    void handleBtSapSetTransferProtocol(const sp<RfxMclMessage>& msg);
    // BTSAP - END
    bool isOp09Card(const char* iccid);
    int isApplicationIdExist(const char* aid);
    void handleSetSimCardPower(const sp<RfxMclMessage>& msg);
    void handleSetSimPower(const sp<RfxMclMessage>& msg);
    void handleEnterDeviceNetworkDepersonalization(const sp<RfxMclMessage>& msg);
    void handleEventGetSlotStatus(const sp<RfxMclMessage>& msg);
    void handleGetSlotStatus(const sp<RfxMclMessage>& msg);
    void handleSetSimSlotsMapping(const sp<RfxMclMessage>& msg);
    bool getAtr(char* atr);
    bool getEid(char* eid);
    UICC_Status getSIMStatusWithRetry(void);
    void handleSetAllowedCarriers(const sp<RfxMclMessage>& msg);
    void handleGetAllowedCarriers(const sp<RfxMclMessage>& msg);
    // External SIM [Start]
    bool mEvsimSent;
    // External SIM [End]
    void handleActiveUiccCard(const sp<RfxMclMessage>& msg);
    void handleDeactivateUiccCard(const sp<RfxMclMessage>& msg);
    // RSU
    void handleRsuRequest(const sp<RfxMclMessage>& msg);
    void handleVzwRsuRequest(const sp<RfxMclMessage>& msg);
    void handleVzwRsuInitiate(const sp<RfxMclMessage>& msg);
    void handleVzwRsuGetSharedKey(const sp<RfxMclMessage>& msg);
    void handleVzwRsuUpdateLockData(const sp<RfxMclMessage>& msg);
    void handleVzwRsuGetVersion(const sp<RfxMclMessage>& msg);
    void handleVzwRsuGetStatus(const sp<RfxMclMessage>& msg);
    void handleVzwRsuUnlockTimer(const sp<RfxMclMessage>& msg);
    void handleKDDIRsuRequest(const sp<RfxMclMessage>& msg);
    void handleKDDIRsuSetSimLockData(const sp<RfxMclMessage>& msg);
    void handleKDDIRsuGetLockData(const sp<RfxMclMessage>& msg, int requestId);
    void handleTmoRsuRequest(const sp<RfxMclMessage>& msg);
    void handleTmoRsuGetRegisterRequest(const sp<RfxMclMessage>& msg);
    void handleTmoRsuCreateUnlockRequest(const sp<RfxMclMessage>& msg);
    void handleTmoRsuProcessMessage(const sp<RfxMclMessage>& msg);
    void handleTmoRsuGetAdapterConfig(const sp<RfxMclMessage>& msg);
    void handleTmoRsuGetSimlockStatus(const sp<RfxMclMessage>& msg);
};
#endif /* __RMC_COMM_SIM_REQUEST_HANDLER_H__ */
