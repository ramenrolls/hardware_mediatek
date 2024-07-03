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

#ifndef __WPFA_H
#define __WPFA_H

#include <unistd.h>

#ifdef __cplusplus
extern "C" {
#endif
#define WA_LOG_TAG "wpfa"
#define MNL_LOG_TAG "libmnlUtils"

#define MNL_SOCKET_AUTOPID 0
#define MNL_SOCKET_BUFFER_SIZE (sysconf(_SC_PAGESIZE) < 8192L ? sysconf(_SC_PAGESIZE) : 8192L)
#define MNL_TOTAL_BUFFER_SIZE (0xffff + (MNL_SOCKET_BUFFER_SIZE / 2))

int wpfaInit();

#ifdef __cplusplus
}
#endif
#endif /* __WPFA_H */
