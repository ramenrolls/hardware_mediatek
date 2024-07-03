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

#ifdef HAVE_LIMITS_H
#include <limits.h>
#endif

#include "ares.h"
#include "ares_private.h"

#ifndef HAVE_WRITEV
ssize_t ares_writev(ares_socket_t s, const struct iovec* iov, int iovcnt) {
    char *buffer, *bp;
    int i;
    size_t bytes = 0;
    ssize_t result;

    /* Validate iovcnt */
    if (iovcnt <= 0) {
        SET_ERRNO(EINVAL);
        return (-1);
    }

    /* Validate and find the sum of the iov_len values in the iov array */
    for (i = 0; i < iovcnt; i++) {
        if (iov[i].iov_len > INT_MAX - bytes) {
            SET_ERRNO(EINVAL);
            return (-1);
        }
        bytes += iov[i].iov_len;
    }

    if (bytes == 0) return (0);

    /* Allocate a temporary buffer to hold the data */
    buffer = malloc(bytes);
    if (!buffer) {
        SET_ERRNO(ENOMEM);
        return (-1);
    }

    /* Copy the data into buffer */
    for (bp = buffer, i = 0; i < iovcnt; ++i) {
        memcpy(bp, iov[i].iov_base, iov[i].iov_len);
        bp += iov[i].iov_len;
    }

    /* Send buffer contents */
    result = swrite(s, buffer, bytes);

    free(buffer);

    return (result);
}
#endif
