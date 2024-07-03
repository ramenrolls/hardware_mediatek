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

#ifndef RIL_CALLBACKS_H
#define RIL_CALLBACKS_H 1

#include <telephony/mtk_ril.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef RIL_SHLIB

extern void RFX_onRequestComplete(RIL_Token t, RIL_Errno e, void* response, size_t responselen);
#if defined(ANDROID_MULTI_SIM)
extern void RFX_onUnsolicitedResponse(int unsolResponse, const void* data, size_t datalen,
                                      RIL_SOCKET_ID socket_id);
#else
extern void RFX_onUnsolicitedResponse(int unsolResponse, const void* data, size_t datalen);
#endif

extern void RFX_requestTimedCallback(RIL_TimedCallback callback, void* param,
                                     const struct timeval* relativeTime);

extern void RFX_onRequestAck(RIL_Token t);
#endif

extern void RFX_SAP_onRequestComplete(RIL_Token t, RIL_Errno e, void* response, size_t responselen);
#if defined(ANDROID_MULTI_SIM)
extern void RFX_SAP_onUnsolicitedResponse(int unsolResponse, void* data, size_t datalen,
                                          int socket_id);
#else
extern void RFX_SAP_onUnsolicitedResponse(int unsolResponse, void* data, size_t datalen);
#endif

#ifdef __cplusplus
}  // extern "C"
#endif

#endif /* RIL_CALLBACKS_H */
