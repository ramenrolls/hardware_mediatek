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

#ifndef _NET_CAP_H_
#define _NET_CAP_H_

#include <sys/cdefs.h>
#include <arpa/inet.h>

#define WPFA_IPTABLE_EXEC_OVER 1
#define WPFA_IPTABLE_POLL_WAIT_ERR -1
#define WPFA_IPTABLE_EXEC_FALED -3

__BEGIN_DECLS

#ifdef __cplusplus
extern "C" {
#endif

/* MTK start */
int wpfaExecIptable(char** args);
/* MTK end */

#ifdef __cplusplus
}
#endif
__END_DECLS
#endif /* _NET_CAP_H_ */
