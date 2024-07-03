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

#ifndef __RFX_H__
#define __RFX_H__

#include <telephony/mtk_ril.h>

#ifdef __cplusplus
extern "C" {
#endif

extern void rfx_init(void);

extern void rfx_enqueue_request_message(int request, void* data, size_t datalen, RIL_Token t,
                                        RIL_SOCKET_ID socketId);

extern void rfx_enqueue_request_message_client(int request, void* data, size_t datalen, RIL_Token t,
                                               RIL_SOCKET_ID socketId);

extern void rfx_enqueue_sap_request_message(int request, void* data, size_t datalen, RIL_Token t,
                                            RIL_SOCKET_ID socketId);

extern void rfx_update_connection_state(RIL_SOCKET_ID socketId, int isConnected);
#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // __RFX_H__
