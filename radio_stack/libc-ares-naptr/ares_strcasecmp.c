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
#include "ares_strcasecmp.h"

#ifndef HAVE_STRCASECMP
int ares_strcasecmp(const char* a, const char* b) {
#if defined(HAVE_STRCMPI)
    return strcmpi(a, b);
#elif defined(HAVE_STRICMP)
    return stricmp(a, b);
#else
    size_t i;

    for (i = 0; i < (size_t)-1; i++) {
        int c1 = ISUPPER(a[i]) ? tolower(a[i]) : a[i];
        int c2 = ISUPPER(b[i]) ? tolower(b[i]) : b[i];
        if (c1 != c2) return c1 - c2;
        if (!c1) break;
    }
    return 0;
#endif
}
#endif

#ifndef HAVE_STRNCASECMP
int ares_strncasecmp(const char* a, const char* b, size_t n) {
#if defined(HAVE_STRNCMPI)
    return strncmpi(a, b, n);
#elif defined(HAVE_STRNICMP)
    return strnicmp(a, b, n);
#else
    size_t i;

    for (i = 0; i < n; i++) {
        int c1 = ISUPPER(a[i]) ? tolower(a[i]) : a[i];
        int c2 = ISUPPER(b[i]) ? tolower(b[i]) : b[i];
        if (c1 != c2) return c1 - c2;
        if (!c1) break;
    }
    return 0;
#endif
}
#endif
