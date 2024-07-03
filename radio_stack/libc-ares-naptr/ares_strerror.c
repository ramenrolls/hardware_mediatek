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
#include <assert.h>
#include "ares.h"

const char* ares_strerror(int code) {
    /* Return a string literal from a table. */
    const char* errtext[] = {"Successful completion",
                             "DNS server returned answer with no data",
                             "DNS server claims query was misformatted",
                             "DNS server returned general failure",
                             "Domain name not found",
                             "DNS server does not implement requested operation",
                             "DNS server refused query",
                             "Misformatted DNS query",
                             "Misformatted domain name",
                             "Unsupported address family",
                             "Misformatted DNS reply",
                             "Could not contact DNS servers",
                             "Timeout while contacting DNS servers",
                             "End of file",
                             "Error reading file",
                             "Out of memory",
                             "Channel is being destroyed",
                             "Misformatted string",
                             "Illegal flags specified",
                             "Given hostname is not numeric",
                             "Illegal hints flags specified",
                             "c-ares library initialization not yet performed",
                             "Error loading iphlpapi.dll",
                             "Could not find GetNetworkParams function",
                             "DNS query cancelled"};

    if (code >= 0 && code < (int)(sizeof(errtext) / sizeof(*errtext)))
        return errtext[code];
    else
        return "unknown";
}
