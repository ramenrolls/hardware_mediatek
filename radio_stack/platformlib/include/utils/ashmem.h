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

#ifndef _UTILS_ASHMEM_H
#define _UTILS_ASHMEM_H

#include <linux/limits.h>
#include <linux/ioctl.h>

#define ASHMEM_NAME_LEN 256

#define ASHMEM_NAME_DEF "dev/ashmem"

/* Return values from ASHMEM_PIN: Was the mapping purged while unpinned? */
#define ASHMEM_NOT_REAPED 0
#define ASHMEM_WAS_REAPED 1

/* Return values from ASHMEM_UNPIN: Is the mapping now pinned or unpinned? */
#define ASHMEM_NOW_UNPINNED 0
#define ASHMEM_NOW_PINNED 1

#define __ASHMEMIOC 0x77

#define ASHMEM_SET_NAME _IOW(__ASHMEMIOC, 1, char[ASHMEM_NAME_LEN])
#define ASHMEM_GET_NAME _IOR(__ASHMEMIOC, 2, char[ASHMEM_NAME_LEN])
#define ASHMEM_SET_SIZE _IOW(__ASHMEMIOC, 3, size_t)
#define ASHMEM_GET_SIZE _IO(__ASHMEMIOC, 4)
#define ASHMEM_SET_PROT_MASK _IOW(__ASHMEMIOC, 5, unsigned long)
#define ASHMEM_GET_PROT_MASK _IO(__ASHMEMIOC, 6)
#define ASHMEM_PIN _IO(__ASHMEMIOC, 7)
#define ASHMEM_UNPIN _IO(__ASHMEMIOC, 8)
#define ASHMEM_ISPINNED _IO(__ASHMEMIOC, 9)
#define ASHMEM_PURGE_ALL_CACHES _IO(__ASHMEMIOC, 10)

#endif /* _UTILS_ASHMEM_H */
