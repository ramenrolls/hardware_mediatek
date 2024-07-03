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

#ifndef ARES_IPV6_H
#define ARES_IPV6_H

#ifndef HAVE_PF_INET6
#define PF_INET6 AF_INET6
#endif

#ifndef HAVE_STRUCT_SOCKADDR_IN6
struct sockaddr_in6 {
    unsigned short sin6_family;
    unsigned short sin6_port;
    unsigned long sin6_flowinfo;
    struct ares_in6_addr sin6_addr;
    unsigned int sin6_scope_id;
};
#endif

#ifndef HAVE_STRUCT_ADDRINFO
struct addrinfo {
    int ai_flags;
    int ai_family;
    int ai_socktype;
    int ai_protocol;
    ares_socklen_t ai_addrlen; /* Follow rfc3493 struct addrinfo */
    char* ai_canonname;
    struct sockaddr* ai_addr;
    struct addrinfo* ai_next;
};
#endif

#ifndef NS_IN6ADDRSZ
#if SIZEOF_STRUCT_IN6_ADDR == 0
/* We cannot have it set to zero, so we pick a fixed value here */
#define NS_IN6ADDRSZ 16
#else
#define NS_IN6ADDRSZ SIZEOF_STRUCT_IN6_ADDR
#endif
#endif

#ifndef NS_INADDRSZ
#define NS_INADDRSZ SIZEOF_STRUCT_IN_ADDR
#endif

#ifndef NS_INT16SZ
#define NS_INT16SZ 2
#endif

#ifndef IF_NAMESIZE
#ifdef IFNAMSIZ
#define IF_NAMESIZE IFNAMSIZ
#else
#define IF_NAMESIZE 256
#endif
#endif

/* Defined in ares_net_pton.c for no particular reason. */
extern const struct ares_in6_addr ares_in6addr_any; /* :: */

#endif /* ARES_IPV6_H */
