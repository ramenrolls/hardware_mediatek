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

#ifndef __RMC_SIM_OP_DEFS_H__
#define __RMC_SIM_OP_DEFS_H__

/**
 * Operator Id
 */
typedef enum {        // Operator ID
    RSU_OP_VZW = 1,   // Verizon
    RSU_OP_KDDI = 2,  // KDDI
} RIL_RSU_OPERATOR;

typedef enum {                               // Request function ID
    RSU_REQUEST_INIT_REQUEST = 0,            // VzW only now
    RSU_REQUEST_GET_SHARED_KEY,              // VzW(HMAC), TMO/ATT(SHARED KEY)
    RSU_REQUEST_UPDATE_LOCK_DATA,            // VzW, TMO, ATT, RJIO
    RSU_REQUEST_GET_LOCK_VERSION,            // VzW, TMO, ATT, RJIO
    RSU_REQUEST_RESET_LOCK_DATA,             // TMO, ATT
    RSU_REQUEST_GET_LOCK_STATUS,             // VzW, TMO, ATT, RJIO
    RSU_REQUEST_UNLOCK_TIMER = 50,           // VzW only now for delay timer. Not defined request
                                             // index by customer, extension start from index 50
    RSU_REQUEST_SET_SIM_LOCK_DATA,           // KDDI
    RSU_REQUEST_SET_GET_ALLOWED_LOCK_DATA,   // KDDI
    RSU_REQUEST_SET_GET_EXCLUDED_LOCK_DATA,  // KDDI
} RIL_RSU_REQUEST;

typedef enum {                             // Request function ID
    RSU_MODEM_REQUEST_INIT_REQUEST = 1,    // VzW only now
    RSU_MODEM_REQUEST_UPDATE_LOCK_DATA,    // VzW, TMO, ATT, RJIO
    RSU_MODEM_REQUEST_GET_LOCK_STATUS,     // VzW, TMO, ATT, RJIO
    RSU_MODEM_REQUEST_UNLOCK_TIMER,        // VzW only now for delay timer. Not defined request
                                           // index by customer, extension start from index 50
    RSU_MODEM_REQUEST_GET_SHARED_KEY,      // VzW(HMAC), TMO/ATT(SHARED KEY)
    RSU_MODEM_REQUEST_GET_LOCK_VERSION,    // VzW, TMO, ATT, RJIO
    RSU_MODEM_REQUEST_GET_LOCK_DATA,       // KDDI
    RSU_MODEM_REQUEST_SET_LOCK_DATA_MORE,  // KDDI
    RSU_MODEM_REQUEST_SET_LOCK_DATA_DONE,  // KDDI
} RIL_RSU_MODEM_REQUEST;
#endif /* __RMC_SIM_OP_DEFS_H__ */
