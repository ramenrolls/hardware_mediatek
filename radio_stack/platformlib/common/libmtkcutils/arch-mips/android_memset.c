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

/* generic C version for any machine */

#include <cutils/memory.h>

#ifdef __clang__
__attribute__((no_sanitize("integer")))
#endif
void android_memset16(uint16_t* dst, uint16_t value, size_t size)
{
    /* optimized version of
       size >>= 1;
       while (size--)
         *dst++ = value;
    */

    size >>= 1;
    if (((uintptr_t)dst & 2) && size) {
        /* fill unpaired first elem separately */
        *dst++ = value;
        size--;
    }
    /* dst is now 32-bit-aligned */
    /* fill body with 32-bit pairs */
    uint32_t value32 = (((uint32_t)value) << 16) | ((uint32_t)value);
    android_memset32((uint32_t*)dst, value32, size << 1);
    if (size & 1) {
        dst[size - 1] = value; /* fill unpaired last elem */
    }
}

#ifdef __clang__
__attribute__((no_sanitize("integer")))
#endif
void android_memset32(uint32_t* dst, uint32_t value, size_t size)
{
    /* optimized version of
       size >>= 2;
       while (size--)
          *dst++ = value;
    */

    size >>= 2;
    if (((uintptr_t)dst & 4) && size) {
        /* fill unpaired first 32-bit elem separately */
        *dst++ = value;
        size--;
    }
    /* dst is now 64-bit aligned */
    /* fill body with 64-bit pairs */
    uint64_t value64 = (((uint64_t)value) << 32) | ((uint64_t)value);
    uint64_t* dst64 = (uint64_t*)dst;

    while (size >= 12) {
        dst64[0] = value64;
        dst64[1] = value64;
        dst64[2] = value64;
        dst64[3] = value64;
        dst64[4] = value64;
        dst64[5] = value64;
        size -= 12;
        dst64 += 6;
    }

    /* fill remainder with original 32-bit single-elem loop */
    dst = (uint32_t*)dst64;
    while (size != 0) {
        size--;
        *dst++ = value;
    }
}
