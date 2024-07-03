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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ares.h"
#include "ares_nowarn.h"
#include "ares_private.h"

/* This is an internal function.  Its contract is to read a line from
 * a file into a dynamically allocated buffer, zeroing the trailing
 * newline if there is one.  The calling routine may call
 * ares__read_line multiple times with the same buf and bufsize
 * pointers; *buf will be reallocated and *bufsize adjusted as
 * appropriate.  The initial value of *buf should be NULL.  After the
 * calling routine is done reading lines, it should free *buf.
 */
int ares__read_line(FILE* fp, char** buf, size_t* bufsize) {
    char* newbuf;
    size_t offset = 0;
    size_t len;

    if (*buf == NULL) {
        *buf = malloc(128);
        if (!*buf) return ARES_ENOMEM;
        *bufsize = 128;
    }

    for (;;) {
        int bytestoread = aresx_uztosi(*bufsize - offset);

        if (!fgets(*buf + offset, bytestoread, fp))
            return (offset != 0) ? 0 : (ferror(fp)) ? ARES_EFILE : ARES_EOF;
        len = offset + strlen(*buf + offset);
        if ((*buf)[len - 1] == '\n') {
            (*buf)[len - 1] = 0;
            break;
        }
        offset = len;
        if (len < *bufsize - 1) continue;

        /* Allocate more space. */
        newbuf = realloc(*buf, *bufsize * 2);
        if (!newbuf) return ARES_ENOMEM;
        *buf = newbuf;
        *bufsize *= 2;
    }
    return ARES_SUCCESS;
}
