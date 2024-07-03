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

#ifndef HAVE_BITNCMP

#include "ares_setup.h"
#include "bitncmp.h"

/*
 * int
 * bitncmp(l, r, n)
 *	compare bit masks l and r, for n bits.
 * return:
 *	-1, 1, or 0 in the libc tradition.
 * note:
 *	network byte order assumed.  this means 192.5.5.240/28 has
 *	0x11110000 in its fourth octet.
 * author:
 *	Paul Vixie (ISC), June 1996
 */
int ares_bitncmp(const void* l, const void* r, int n) {
    unsigned int lb, rb;
    int x, b;

    b = n / 8;
    x = memcmp(l, r, b);
    if (x || (n % 8) == 0) return (x);

    lb = ((const unsigned char*)l)[b];
    rb = ((const unsigned char*)r)[b];
    for (b = n % 8; b > 0; b--) {
        if ((lb & 0x80) != (rb & 0x80)) {
            if (lb & 0x80) return (1);
            return (-1);
        }
        lb <<= 1;
        rb <<= 1;
    }
    return (0);
}
#endif
