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
#ifdef HAVE_ARPA_NAMESER_H
#include <arpa/nameser.h>
#else
#include "nameser.h"
#endif

#include <string.h>
#include <stdlib.h>
#include "ares.h"
#include "ares_private.h" /* for the memdebug */

/* Simply decodes a length-encoded character string. The first byte of the
 * input is the length of the string to be returned and the bytes thereafter
 * are the characters of the string. The returned result will be NULL
 * terminated.
 */
int ares_expand_string(const unsigned char* encoded, const unsigned char* abuf, int alen,
                       unsigned char** s, long* enclen) {
    unsigned char* q;
    union {
        ssize_t sig;
        size_t uns;
    } elen;

    if (encoded == abuf + alen) return ARES_EBADSTR;

    elen.uns = *encoded;
    if (encoded + elen.sig + 1 > abuf + alen) return ARES_EBADSTR;

    encoded++;
    printf("%s length:%zd\n", __func__, elen.uns);
    if (elen.uns == 0)
        *s = NULL;
    else {
        *s = malloc(elen.uns + 1);
        if (*s == NULL) return ARES_ENOMEM;

        q = *s;
        strncpy((char*)q, (char*)encoded, elen.uns);
        q[elen.uns] = '\0';

        *s = q;
    }
    *enclen = (long)(elen.sig + 1);

    return ARES_SUCCESS;
}
