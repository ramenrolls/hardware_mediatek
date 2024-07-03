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

#ifndef __RTST_PARCEL_UTILS_H__
#define __RTST_PARCEL_UTILS_H__
/*****************************************************************************
 * Include
 *****************************************************************************/
#include <utils/Parcel.h>
#include <stddef.h>
#include <utils/Thread.h>
#include "utils/RefBase.h"
#include "utils/Errors.h"
#include <telephony/ril.h>
#include "RilClientConstants.h"
#include "RfxDispatchThread.h"

/*****************************************************************************
 * Name Space
 *****************************************************************************/
using ::android::Parcel;

/*****************************************************************************
 * Parcel utils api
 *****************************************************************************/
extern void dispatchVoid(RIL_RequestFunc onRequest, Parcel& p, RfxRequestInfo* pRI);
extern void dispatchString(RIL_RequestFunc onRequest, Parcel& p, RfxRequestInfo* pRI);
extern void dispatchStrings(RIL_RequestFunc onRequest, Parcel& p, RfxRequestInfo* pRI);
extern void dispatchInts(RIL_RequestFunc onRequest, Parcel& p, RfxRequestInfo* pRI);
extern void dispatchDial(RIL_RequestFunc onRequest, Parcel& p, RfxRequestInfo* pRI);
extern void dispatchEmergencyDial(RIL_RequestFunc onRequest, Parcel& p, RfxRequestInfo* pRI);
extern void dispatchSIM_IO(RIL_RequestFunc onRequest, Parcel& p, RfxRequestInfo* pRI);
extern void dispatchSIM_APDU(RIL_RequestFunc onRequest, Parcel& p, RfxRequestInfo* pRI);
extern void dispatchCallForward(RIL_RequestFunc onRequest, Parcel& p, RfxRequestInfo* pRI);
extern void dispatchCallForwardEx(RIL_RequestFunc onRequest, Parcel& p, RfxRequestInfo* pRI);
extern void dispatchRaw(RIL_RequestFunc onRequest, Parcel& p, RfxRequestInfo* pRI);
extern void dispatchSmsWrite(RIL_RequestFunc onRequest, Parcel& p, RfxRequestInfo* pRI);
extern void dispatchDataCall(RIL_RequestFunc onRequest, Parcel& p, RfxRequestInfo* pRI);
extern void dispatchVoiceRadioTech(RIL_RequestFunc onRequest, Parcel& p, RfxRequestInfo* pRI);
extern void dispatchSetInitialAttachApn(RIL_RequestFunc onRequest, Parcel& p, RfxRequestInfo* pRI);
extern void dispatchCdmaSubscriptionSource(RIL_RequestFunc onRequest, Parcel& p,
                                           RfxRequestInfo* pRI);

extern void dispatchCdmaSms(RIL_RequestFunc onRequest, Parcel& p, RfxRequestInfo* pRI);
extern void dispatchImsSms(RIL_RequestFunc onRequest, Parcel& p, RfxRequestInfo* pRI);
extern void dispatchImsCdmaSms(RIL_RequestFunc onRequest, Parcel& p, RfxRequestInfo* pRI,
                               uint8_t retry, int32_t messageRef);
extern void dispatchImsGsmSms(RIL_RequestFunc onRequest, Parcel& p, RfxRequestInfo* pRI,
                              uint8_t retry, int32_t messageRef);
extern void dispatchCdmaSmsAck(RIL_RequestFunc onRequest, Parcel& p, RfxRequestInfo* pRI);
extern void dispatchGsmBrSmsCnf(RIL_RequestFunc onRequest, Parcel& p, RfxRequestInfo* pRI);
extern void dispatchCdmaBrSmsCnf(RIL_RequestFunc onRequest, Parcel& p, RfxRequestInfo* pRI);
extern void dispatchRilCdmaSmsWriteArgs(RIL_RequestFunc onRequest, Parcel& p, RfxRequestInfo* pRI);
extern void dispatchNVReadItem(RIL_RequestFunc onRequest, Parcel& p, RfxRequestInfo* pRI);
extern void dispatchNVWriteItem(RIL_RequestFunc onRequest, Parcel& p, RfxRequestInfo* pRI);
extern void dispatchUiccSubscripton(RIL_RequestFunc onRequest, Parcel& p, RfxRequestInfo* pRI);
extern void dispatchSimAuthentication(RIL_RequestFunc onRequest, Parcel& p, RfxRequestInfo* pRI);
extern void dispatchDataProfile(RIL_RequestFunc onRequest, Parcel& p, RfxRequestInfo* pRI);
extern void dispatchRadioCapability(RIL_RequestFunc onRequest, Parcel& p, RfxRequestInfo* pRI);
extern void dispatchCarrierRestrictions(RIL_RequestFunc onRequest, Parcel& p, RfxRequestInfo* pRI);
extern void dispatchPhbEntry(RIL_RequestFunc onRequest, Parcel& p, RfxRequestInfo* pRI);
extern void dispatchSetSystemSelectionChannels(RIL_RequestFunc onRequest, Parcel& p,
                                               RfxRequestInfo* pRI);
extern void dispatchSetSignalStrengthReportingCriteria(RIL_RequestFunc onRequest, Parcel& p,
                                                       RfxRequestInfo* pRI);

extern int responseInts(Parcel& p, void* response, size_t responselen);
extern int responseFailCause(Parcel& p, void* response, size_t responselen);
extern int responseStrings(Parcel& p, void* response, size_t responselen);
extern int responseString(Parcel& p, void* response, size_t responselen);
extern int responseVoid(Parcel& p, void* response, size_t responselen);
extern int responseCallList(Parcel& p, void* response, size_t responselen);
extern int responseSMS(Parcel& p, void* response, size_t responselen);
extern int responseSIM_IO(Parcel& p, void* response, size_t responselen);
extern int responseCallForwards(Parcel& p, void* response, size_t responselen);
extern int responseCallForwardsEx(Parcel& p, void* response, size_t responselen);
extern int responseDataCallList(Parcel& p, void* response, size_t responselen);
extern int responseSetupDataCall(Parcel& p, void* response, size_t responselen);
extern int responseRaw(Parcel& p, void* response, size_t responselen);
extern int responseSsn(Parcel& p, void* response, size_t responselen);
extern int responseSimStatus(Parcel& p, void* response, size_t responselen);
extern int responseGsmBrSmsCnf(Parcel& p, void* response, size_t responselen);
extern int responseCdmaBrSmsCnf(Parcel& p, void* response, size_t responselen);
extern int responseCdmaSms(Parcel& p, void* response, size_t responselen);
extern int responseCellList(Parcel& p, void* response, size_t responselen);
extern int responseCdmaInformationRecords(Parcel& p, void* response, size_t responselen);
extern int responseRilSignalStrength(Parcel& p, void* response, size_t responselen);
extern int responseCallRing(Parcel& p, void* response, size_t responselen);
extern int responseCdmaSignalInfoRecord(Parcel& p, void* response, size_t responselen);
extern int responseCdmaCallWaiting(Parcel& p, void* response, size_t responselen);
extern int responseSimRefresh(Parcel& p, void* response, size_t responselen);
extern int responseCellInfoList(Parcel& p, void* response, size_t responselen);
extern int responseGetSmsSimMemStatusCnf(Parcel& p, void* response, size_t responselen);
extern int responseEtwsNotification(Parcel& p, void* response, size_t responselen);
extern int responseHardwareConfig(Parcel& p, void* response, size_t responselen);
extern int responseDcRtInfo(Parcel& p, void* response, size_t responselen);
extern int responseRadioCapability(Parcel& p, void* response, size_t responselen);
extern int responseSSData(Parcel& p, void* response, size_t responselen);
extern int responseLceStatus(Parcel& p, void* response, size_t responselen);
extern int responseLceData(Parcel& p, void* response, size_t responselen);
extern int responseActivityData(Parcel& p, void* response, size_t responselen);
extern int responseCarrierRestrictions(Parcel& p, void* response, size_t responselen);
extern int responsePcoData(Parcel& p, void* response, size_t responselen);
extern int responsePcoIaData(Parcel& p, void* response, size_t responselen);
extern int responseCrssN(Parcel& p, void* response, size_t responselen);
extern int responseVoiceRegistrationState(Parcel& p, void* response, size_t responselen);
extern int responseDataRegistrationState(Parcel& p, void* response, size_t responselen);
extern int responseEmergencyList(Parcel& p, void* response, size_t responselen);
extern int responsePhbEntries(Parcel& p, void* response, size_t responselen);
#endif /* __RTST_PARCEL_UTILS_H__ */
