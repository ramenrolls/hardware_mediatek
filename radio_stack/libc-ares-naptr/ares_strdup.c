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
#include "ares_strdup.h"

#ifndef HAVE_STRDUP
char* ares_strdup(const char* s1) {
    size_t sz;
    char* s2;

    if (s1) {
        sz = strlen(s1);
        if (sz < (size_t)-1) {
            sz++;
            if (sz < ((size_t)-1) / sizeof(char)) {
                s2 = malloc(sz * sizeof(char));
                if (s2) {
                    memcpy(s2, s1, sz * sizeof(char));
                    return s2;
                }
            }
        }
    }
    return (char*)NULL;
}
#endif
