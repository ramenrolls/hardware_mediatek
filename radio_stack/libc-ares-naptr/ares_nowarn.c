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

#ifdef HAVE_ASSERT_H
#include <assert.h>
#endif

#ifdef HAVE_LIMITS_H
#include <limits.h>
#endif

#if defined(__INTEL_COMPILER) && defined(__unix__)

#ifdef HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif
#ifdef HAVE_NETINET_IN_H
#include <netinet/in.h>
#endif
#ifdef HAVE_ARPA_INET_H
#include <arpa/inet.h>
#endif

#endif /* __INTEL_COMPILER && __unix__ */

#define BUILDING_ARES_NOWARN_C 1

#include "ares_nowarn.h"

#if (SIZEOF_INT == 2)
#define CARES_MASK_SINT 0x7FFF
#define CARES_MASK_UINT 0xFFFF
#elif (SIZEOF_INT == 4)
#define CARES_MASK_SINT 0x7FFFFFFF
#define CARES_MASK_UINT 0xFFFFFFFF
#elif (SIZEOF_INT == 8)
#define CARES_MASK_SINT 0x7FFFFFFFFFFFFFFF
#define CARES_MASK_UINT 0xFFFFFFFFFFFFFFFF
#elif (SIZEOF_INT == 16)
#define CARES_MASK_SINT 0x7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
#define CARES_MASK_UINT 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
#elif defined(HAVE_LIMITS_H)
#define CARES_MASK_SINT INT_MAX
#define CARES_MASK_UINT UINT_MAX
#endif

/*
** unsigned size_t to signed int
*/

int aresx_uztosi(size_t uznum) {
#ifdef __INTEL_COMPILER
#pragma warning(push)
#pragma warning(disable : 810) /* conversion may lose significant bits */
#endif

    return (int)(uznum & (size_t)CARES_MASK_SINT);

#ifdef __INTEL_COMPILER
#pragma warning(pop)
#endif
}

/*
** signed long to signed int
*/

int aresx_sltosi(long slnum) {
#ifdef __INTEL_COMPILER
#pragma warning(push)
#pragma warning(disable : 810) /* conversion may lose significant bits */
#endif

    DEBUGASSERT(slnum >= 0);
    return (int)(slnum & (long)CARES_MASK_SINT);

#ifdef __INTEL_COMPILER
#pragma warning(pop)
#endif
}

/*
** signed ssize_t to signed int
*/

int aresx_sztosi(ssize_t sznum) {
#ifdef __INTEL_COMPILER
#pragma warning(push)
#pragma warning(disable : 810) /* conversion may lose significant bits */
#endif

    DEBUGASSERT(sznum >= 0);
    return (int)(sznum & (ssize_t)CARES_MASK_SINT);

#ifdef __INTEL_COMPILER
#pragma warning(pop)
#endif
}

/*
** signed ssize_t to unsigned int
*/

unsigned int aresx_sztoui(ssize_t sznum) {
#ifdef __INTEL_COMPILER
#pragma warning(push)
#pragma warning(disable : 810) /* conversion may lose significant bits */
#endif

    DEBUGASSERT(sznum >= 0);
    return (unsigned int)(sznum & (ssize_t)CARES_MASK_UINT);

#ifdef __INTEL_COMPILER
#pragma warning(pop)
#endif
}

#if defined(__INTEL_COMPILER) && defined(__unix__)

int aresx_FD_ISSET(int fd, fd_set* fdset) {
#pragma warning(push)
#pragma warning(disable : 1469) /* clobber ignored */
    return FD_ISSET(fd, fdset);
#pragma warning(pop)
}

void aresx_FD_SET(int fd, fd_set* fdset) {
#pragma warning(push)
#pragma warning(disable : 1469) /* clobber ignored */
    FD_SET(fd, fdset);
#pragma warning(pop)
}

void aresx_FD_ZERO(fd_set* fdset) {
#pragma warning(push)
#pragma warning(disable : 593) /* variable was set but never used */
    FD_ZERO(fdset);
#pragma warning(pop)
}

unsigned short aresx_htons(unsigned short usnum) {
#if (__INTEL_COMPILER == 910) && defined(__i386__)
    return (unsigned short)(((usnum << 8) & 0xFF00) | ((usnum >> 8) & 0x00FF));
#else
#pragma warning(push)
#pragma warning(disable : 810) /* conversion may lose significant bits */
    return htons(usnum);
#pragma warning(pop)
#endif
}

unsigned short aresx_ntohs(unsigned short usnum) {
#if (__INTEL_COMPILER == 910) && defined(__i386__)
    return (unsigned short)(((usnum << 8) & 0xFF00) | ((usnum >> 8) & 0x00FF));
#else
#pragma warning(push)
#pragma warning(disable : 810) /* conversion may lose significant bits */
    return ntohs(usnum);
#pragma warning(pop)
#endif
}

#endif /* __INTEL_COMPILER && __unix__ */
