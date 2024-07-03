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

#ifdef HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif
#ifdef HAVE_NETINET_IN_H
#include <netinet/in.h>
#endif
#ifdef HAVE_ARPA_INET_H
#include <arpa/inet.h>
#endif
#ifdef HAVE_NETDB_H
#include <netdb.h>
#endif
#ifdef HAVE_ARPA_NAMESER_H
#include <arpa/nameser.h>
#else
#include "nameser.h"
#endif
#ifdef HAVE_ARPA_NAMESER_COMPAT_H
#include <arpa/nameser_compat.h>
#endif

#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef HAVE_STRINGS_H
#include <strings.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "ares.h"
#include "ares_dns.h"
#include "inet_ntop.h"
#include "inet_net_pton.h"
#include "ares_getopt.h"
#include "ares_nowarn.h"
#include "ares_private.h"

int main(int argc, char** argv) {
    struct records_naptr *result_list = NULL, *ptr = NULL;
    struct query_type hints = {0, SOCK_STREAM, 0, 53, 0};
    // aes_getrecords(argv[1], argv[2],&hints,&result_list);
    if (aes_getrecords(argv[1], argv[2], &hints, &result_list) == 1) {
        ptr = result_list;
        do {
            printf("naptr results: %p %d %d %s %p %s %p %s %p %s %p %p\n", ptr, ptr->order,
                   ptr->pref, ptr->flags, ptr->flags, ptr->service, ptr->service, ptr->regexp,
                   ptr->regexp, ptr->fqdn, ptr->fqdn, ptr->next);
            ptr = ptr->next;
        } while (ptr);
    }
    aes_getrecords_free(result_list);
}
