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

#ifndef _CUTILS_PMEM_H
#define _CUTILS_PMEM_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Allocates a memory block which is continuous in physical memory.
 * @param size  number of bytes to allocate
 * @param pfd   pointer to associate file descriptor
 * @return a void pointer to the allocated space, or NULL if there is insufficient
 *         memory available.
 */
void* pmem_alloc(size_t size, int* pfd);

/**
 * Allocates a sync memory block which is continuous in physical memory.
 * @param size  number of bytes to allocate
 * @param pfd   pointer to associate file descriptor
 * @return a void pointer to the allocated space, or NULL if there is insufficient
 *         memory available.
 */
void* pmem_alloc_sync(size_t size, int* pfd);

/**
 * Deallocates or frees a memory block which is continuous in physical memory.
 * @param ptr  pointer to previously allocated memory block which is allocated by pmem_alloc().
 * @param size size of the memory block which is allocated by pmem_alloc().
 * @param fd   associated file descriptor of the memory block which is allocated by pmem_alloc().
 * @return 0 if success, or nagtive value if there is error
 */
int pmem_free(void* ptr, size_t size, int fd);

/**
 * Convert the address of a memory block from virtual address to physical address.
 * @param fd       associated file descriptor of the memory block which is allocated by
 * pmem_alloc().
 * @return a void pointer contains the physical address, or NULL if virPtr is invalid.
 */
void* pmem_get_phys(int fd);

#ifdef __cplusplus
}
#endif

#endif /* _CUTILS_PMEM_H */
