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

#ifndef __CARES_BUILD_H
#define __CARES_BUILD_H

#ifdef CARES_SIZEOF_LONG
#error "CARES_SIZEOF_LONG shall not be defined except in ares_build.h"
Error Compilation_aborted_CARES_SIZEOF_LONG_already_defined
#endif

#ifdef CARES_TYPEOF_ARES_SOCKLEN_T
#error "CARES_TYPEOF_ARES_SOCKLEN_T shall not be defined except in ares_build.h"
        Error Compilation_aborted_CARES_TYPEOF_ARES_SOCKLEN_T_already_defined
#endif

#ifdef CARES_SIZEOF_ARES_SOCKLEN_T
#error "CARES_SIZEOF_ARES_SOCKLEN_T shall not be defined except in ares_build.h"
                Error Compilation_aborted_CARES_SIZEOF_ARES_SOCKLEN_T_already_defined
#endif

/* ================================================================ */
/*    EXTERNAL INTERFACE SETTINGS FOR NON-CONFIGURE SYSTEMS ONLY    */
/* ================================================================ */

#if defined(__DJGPP__) || defined(__GO32__)
#define CARES_SIZEOF_LONG 4
#define CARES_TYPEOF_ARES_SOCKLEN_T int
#define CARES_SIZEOF_ARES_SOCKLEN_T 4

#elif defined(__SALFORDC__)
#define CARES_SIZEOF_LONG 4
#define CARES_TYPEOF_ARES_SOCKLEN_T int
#define CARES_SIZEOF_ARES_SOCKLEN_T 4

#elif defined(__BORLANDC__)
#define CARES_SIZEOF_LONG 4
#define CARES_TYPEOF_ARES_SOCKLEN_T int
#define CARES_SIZEOF_ARES_SOCKLEN_T 4

#elif defined(__TURBOC__)
#define CARES_SIZEOF_LONG 4
#define CARES_TYPEOF_ARES_SOCKLEN_T int
#define CARES_SIZEOF_ARES_SOCKLEN_T 4

#elif defined(__WATCOMC__)
#define CARES_SIZEOF_LONG 4
#define CARES_TYPEOF_ARES_SOCKLEN_T int
#define CARES_SIZEOF_ARES_SOCKLEN_T 4

#elif defined(__POCC__)
#define CARES_SIZEOF_LONG 4
#define CARES_TYPEOF_ARES_SOCKLEN_T int
#define CARES_SIZEOF_ARES_SOCKLEN_T 4

#elif defined(__LCC__)
#define CARES_SIZEOF_LONG 4
#define CARES_TYPEOF_ARES_SOCKLEN_T int
#define CARES_SIZEOF_ARES_SOCKLEN_T 4

#elif defined(__SYMBIAN32__)
#define CARES_SIZEOF_LONG 4
#define CARES_TYPEOF_ARES_SOCKLEN_T unsigned int
#define CARES_SIZEOF_ARES_SOCKLEN_T 4

#elif defined(__MWERKS__)
#define CARES_SIZEOF_LONG 4
#define CARES_TYPEOF_ARES_SOCKLEN_T int
#define CARES_SIZEOF_ARES_SOCKLEN_T 4

#elif defined(_WIN32_WCE)
#define CARES_SIZEOF_LONG 4
#define CARES_TYPEOF_ARES_SOCKLEN_T int
#define CARES_SIZEOF_ARES_SOCKLEN_T 4

#elif defined(__MINGW32__)
#define CARES_SIZEOF_LONG 4
#define CARES_TYPEOF_ARES_SOCKLEN_T int
#define CARES_SIZEOF_ARES_SOCKLEN_T 4

#elif defined(__VMS)
#define CARES_SIZEOF_LONG 4
#define CARES_TYPEOF_ARES_SOCKLEN_T unsigned int
#define CARES_SIZEOF_ARES_SOCKLEN_T 4

#elif defined(__OS400__)
#if defined(__ILEC400__)
#define CARES_SIZEOF_LONG 4
#define CARES_TYPEOF_ARES_SOCKLEN_T socklen_t
#define CARES_SIZEOF_ARES_SOCKLEN_T 4
#define CARES_PULL_SYS_TYPES_H 1
#define CARES_PULL_SYS_SOCKET_H 1
#endif

#elif defined(__MVS__)
#if defined(__IBMC__) || defined(__IBMCPP__)
#if defined(_ILP32)
#define CARES_SIZEOF_LONG 4
#elif defined(_LP64)
#define CARES_SIZEOF_LONG 8
#endif
#define CARES_TYPEOF_ARES_SOCKLEN_T socklen_t
#define CARES_SIZEOF_ARES_SOCKLEN_T 4
#define CARES_PULL_SYS_TYPES_H 1
#define CARES_PULL_SYS_SOCKET_H 1
#endif

#elif defined(__370__)
#if defined(__IBMC__) || defined(__IBMCPP__)
#if defined(_ILP32)
#define CARES_SIZEOF_LONG 4
#elif defined(_LP64)
#define CARES_SIZEOF_LONG 8
#endif
#define CARES_TYPEOF_ARES_SOCKLEN_T socklen_t
#define CARES_SIZEOF_ARES_SOCKLEN_T 4
#define CARES_PULL_SYS_TYPES_H 1
#define CARES_PULL_SYS_SOCKET_H 1
#endif

#elif defined(TPF)
#define CARES_SIZEOF_LONG 8
#define CARES_TYPEOF_ARES_SOCKLEN_T int
#define CARES_SIZEOF_ARES_SOCKLEN_T 4

/* ===================================== */
/*    KEEP MSVC THE PENULTIMATE ENTRY    */
/* ===================================== */

#elif defined(_MSC_VER)
#define CARES_SIZEOF_LONG 4
#define CARES_TYPEOF_ARES_SOCKLEN_T int
#define CARES_SIZEOF_ARES_SOCKLEN_T 4

/* ===================================== */
/*    KEEP GENERIC GCC THE LAST ENTRY    */
/* ===================================== */

#elif defined(__GNUC__)
#if defined(__i386__) || defined(__ppc__)
#define CARES_SIZEOF_LONG 4
#elif defined(__x86_64__) || defined(__ppc64__)
#define CARES_SIZEOF_LONG 8
#else
#define CARES_SIZEOF_LONG sizeof(long)
#endif
#define CARES_TYPEOF_ARES_SOCKLEN_T socklen_t
#define CARES_SIZEOF_ARES_SOCKLEN_T 4
#define CARES_PULL_SYS_TYPES_H 1
#define CARES_PULL_SYS_SOCKET_H 1

#else
#error "Unknown non-configure build target!"
Error Compilation_aborted_Unknown_non_configure_build_target
#endif

/* CARES_PULL_SYS_TYPES_H is defined above when inclusion of header file  */
/* sys/types.h is required here to properly make type definitions below.  */
#ifdef CARES_PULL_SYS_TYPES_H
#include <sys/types.h>
#endif

/* CARES_PULL_SYS_SOCKET_H is defined above when inclusion of header file  */
/* sys/socket.h is required here to properly make type definitions below.  */
#ifdef CARES_PULL_SYS_SOCKET_H
#include <sys/socket.h>
#endif

/* Data type definition of ares_socklen_t. */

#ifdef CARES_TYPEOF_ARES_SOCKLEN_T
        typedef CARES_TYPEOF_ARES_SOCKLEN_T ares_socklen_t;
#endif

#endif /* __CARES_BUILD_H */
