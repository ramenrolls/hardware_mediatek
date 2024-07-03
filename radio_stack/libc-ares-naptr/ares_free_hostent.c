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

#include "ares_setup.h"
#include <stdlib.h>

#ifdef HAVE_NETDB_H
#include <netdb.h>
#endif

#include "ares.h"
#include "ares_private.h" /* for memdebug */

void ares_free_hostent(struct hostent* host) {
    char** p;

    if (!host) return;

    free((char*)(host->h_name));
    for (p = host->h_aliases; *p; p++) free(*p);
    free(host->h_aliases);
    free(host->h_addr_list[0]); /* no matter if there is one or many entries,
                                   there is only one malloc for all of them */
    free(host->h_addr_list);
    free(host);
}
