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

#include <string.h>
#include <asm/types.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <linux/if_link.h>
#include <linux/if_addr.h>
#include <linux/neighbour.h>
#include <asm/types.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <fcntl.h>
#include <dirent.h>
#include <time.h>

typedef int64_t nsecs_t;

enum {
    SYSTEM_TIME_REALTIME,
    SYSTEM_TIME_MONOTONIC,
    SYSTEM_TIME_PROCESS,
    SYSTEM_TIME_THREAD,
    SYSTEM_TIME_BOOTTIME
};

struct wlan0_hw_addr {
    unsigned char wlan0_mac_addr[6];
};

struct rtnl_handle {
    int fd;
    int seq;
    int dump;
    struct sockaddr_nl local;
};

#define INET_ADDRSTRLEN 16
#define INET6_ADDRSTRLEN 46

#define MACSIZE 6
#define IN6ADDR_ANY_INIT                                       \
    {                                                          \
        {                                                      \
            { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } \
        }                                                      \
    }

#define NOT_MATCH 2
#define TIME_EXCEED 998
#define NO_REPLY 999

int rtnl_open(struct rtnl_handle* rth);
void rtnl_close(struct rtnl_handle* rth);
static int Ipv4GetMacByIoctl(const char* ipStr, const char* ifname,
                             struct sockaddr* WlanSocketAddr);
char* SocketAddr_ntop(const struct sockaddr* SockAddr, char* bufp);
static void WlanMacAddr_ntop(const struct wlan0_hw_addr* WlanMacAddr, char* buf);
static int SendNewNeighborMessage(const char* ipStr, int ifIndex, bool is_ipv6,
                                  struct sockaddr* WlanSocketAddr);
static int inet_get_addr(const char* ip_str, bool is_ipv6, struct in_addr* ip,
                         struct in6_addr* ip6);
int addattr_l(struct nlmsghdr* n, int maxlen, int type, const void* data, int alen);
void parse_rtattr(struct rtattr** tb, int max, struct rtattr* rta, int len);
const char* addr_to_mac(unsigned char* addr, int alen, char* buf, ssize_t blen);
static int read_neighbor_nlmsg(int fd, const char* ipStr, struct sockaddr* WlanSocketAddr);
static int neigh_mac_process(struct nlmsghdr* nh, const char* ipStr,
                             struct sockaddr* WlanSocketAddr);
static const inline nsecs_t seconds_to_nanoseconds(nsecs_t secs);
static const inline nsecs_t nanoseconds_to_milliseconds(nsecs_t secs);
static nsecs_t elapsedRealtime(int clock);
static int SendIpv6_DumpRequest(const char* ipStr, int ifIndex, struct sockaddr* WlanSocketAddr);
int Get_Mac_Addr(const char* ipStr, const char* ifname, struct sockaddr* WlanSocketAddr);
