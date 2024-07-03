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

#ifndef __RIL_CLIENT_CONSTANTS_H
#define __RIL_CLIENT_CONSTANTS_H

#include <utils/Parcel.h>
#include <telephony/mtk_ril.h>

using ::android::Parcel;

#ifdef __cplusplus
extern "C" {
#endif

/* Negative values for private RIL errno's */
#define RIL_ERRNO_INVALID_RESPONSE -1
#define RIL_ERRNO_NO_MEMORY -12

typedef enum {
    CLIENT_ID_START = 1000,
    CLIENT_ID_OEM,
    CLIENT_ID_MTTS1,
    CLIENT_ID_MTTS2,
    CLIENT_ID_MTTS3,
    CLIENT_ID_MTTS4,
    CLIENT_ID_ATCI,
} ClientId;

#ifdef __cplusplus
}
#endif
#endif /*RADIO_CONSTANT_H*/
