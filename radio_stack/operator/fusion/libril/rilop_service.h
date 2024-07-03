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

#ifndef RILOP_SERVICE_H
#define RILOP_SERVICE_H

#include <telephony/mtk_ril.h>
#include <telephony/mtk_rilop.h>
#include <rilop_internal.h>
#include <ril_internal.h>

namespace radio {

extern "C" void registerOpService(RIL_RadioFunctions* callbacks, android::CommandInfo* commands);
/*********************************************************************************/
/*  Vendor request                                                               */
/*********************************************************************************/
int setDigitsLineResponse(int slotId, android::ClientId clientId, int responseType, int serial,
                          RIL_Errno e, void* response, size_t responselen);

int setTrnResponse(int slotId, android::ClientId clientId, int responseType, int serial,
                   RIL_Errno e, void* response, size_t responselen);

int setIncomingVirtualLineResponse(int slotId, android::ClientId clientId, int responseType,
                                   int serial, RIL_Errno e, void* response, size_t responselen);

int setRxTestConfigResponse(int slotId, android::ClientId clientId, int responseType, int serial,
                            RIL_Errno e, void* response, size_t responseLen);

int getRxTestResultResponse(int slotId, android::ClientId clientId, int responseType, int serial,
                            RIL_Errno e, void* response, size_t responseLen);

int dialFromResponse(int slotId, android::ClientId clientId, int responseType, int serial,
                     RIL_Errno e, void* response, size_t responselen);

int sendUssiFromResponse(int slotId, android::ClientId clientId, int responseType, int serial,
                         RIL_Errno e, void* response, size_t responselen);

int cancelUssiFromResponse(int slotId, android::ClientId clientId, int responseType, int serial,
                           RIL_Errno e, void* response, size_t responselen);

int setEmergencyCallConfigResponse(int slotId, android::ClientId clientId, int responseType,
                                   int serial, RIL_Errno e, void* response, size_t responselen);

int setDisable2GResponse(int slotId, android::ClientId clientId, int responseType, int serial,
                         RIL_Errno e, void* response, size_t responseLen);

int getDisable2GResponse(int slotId, android::ClientId clientId, int responseType, int serial,
                         RIL_Errno e, void* response, size_t responseLen);

int sendRsuRequestResponse(int slotId, android::ClientId clientId, int responseType, int serial,
                           RIL_Errno e, void* response, size_t responselen);

int deviceSwitchResponse(int slotId, android::ClientId clientId, int responseType, int serial,
                         RIL_Errno e, void* response, size_t responselen);

int cancelDeviceSwitchResponse(int slotId, android::ClientId clientId, int responseType, int serial,
                               RIL_Errno e, void* response, size_t responselen);

int setDigitsRegStatuseResponse(int slotId, android::ClientId clientId, int responseType,
                                int serial, RIL_Errno e, void* response, size_t responselen);

int exitSCBMResponse(int slotId, android::ClientId clientId, int responseType, int serial,
                     RIL_Errno e, void* response, size_t responselen);
int switchRcsRoiStatusResponse(int slotId, android::ClientId clientId, int responseType, int serial,
                               RIL_Errno e, void* response, size_t responselen);
int updateRcsCapabilitiesResponse(int slotId, android::ClientId clientId, int responseType,
                                  int serial, RIL_Errno e, void* response, size_t responselen);
int updateRcsSessionInfoResponse(int slotId, android::ClientId clientId, int responseType,
                                 int serial, RIL_Errno e, void* response, size_t responselen);

/*********************************************************************************/
/*  Vendor unsol                                                                 */
/*********************************************************************************/
int digitsLineIndicationInd(int slotId, int indicationType, int token, RIL_Errno e, void* response,
                            size_t responselen);

int getTrnIndicationInd(int slotId, int indicationType, int token, RIL_Errno e, void* response,
                        size_t responselen);

// MTK-START: SIM TMO RSU
int onSimMeLockEvent(int slotId, int indicationType, int token, RIL_Errno e, void* response,
                     size_t responseLen);
// MTK-END

int onRsuEvent(int slotId, int indicationType, int token, RIL_Errno e, void* response,
               size_t responselen);

int responseModulationInfoInd(int slotId, int indicationType, int token, RIL_Errno e,
                              void* response, size_t responseLen);

int digitsLineIndication(int slotId, int indicationType, int token, RIL_Errno e, void* response,
                         size_t responselen);

int enterSCBMInd(int slotId, int indicationType, int token,

                 RIL_Errno e, void* response, size_t responselen);

int exitSCBMInd(int slotId, int indicationType, int token,

                RIL_Errno e, void* response, size_t responselen);
}  // namespace radio
#endif  // RILOP_SERVICE_H
