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

#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif

#include "ares.h"
#include "ares_private.h"

int ares_getsock(ares_channel channel, ares_socket_t* socks,
                 int numsocks) /* size of the 'socks' array */
{
    struct server_state* server;
    int i;
    int sockindex = 0;
    int bitmap = 0;
    unsigned int setbits = 0xffffffff;

    /* Are there any active queries? */
    int active_queries = !ares__is_list_empty(&(channel->all_queries));

    for (i = 0; i < channel->nservers; i++) {
        server = &channel->servers[i];
        /* We only need to register interest in UDP sockets if we have
         * outstanding queries.
         */
        if (active_queries && server->udp_socket != ARES_SOCKET_BAD) {
            if (sockindex >= numsocks || sockindex >= ARES_GETSOCK_MAXNUM) break;
            socks[sockindex] = server->udp_socket;
            bitmap |= ARES_GETSOCK_READABLE(setbits, sockindex);
            sockindex++;
        }
        /* We always register for TCP events, because we want to know
         * when the other side closes the connection, so we don't waste
         * time trying to use a broken connection.
         */
        if (server->tcp_socket != ARES_SOCKET_BAD) {
            if (sockindex >= numsocks || sockindex >= ARES_GETSOCK_MAXNUM) break;
            socks[sockindex] = server->tcp_socket;
            bitmap |= ARES_GETSOCK_READABLE(setbits, sockindex);

            if (server->qhead && active_queries) /* then the tcp socket is also writable! */
                bitmap |= ARES_GETSOCK_WRITABLE(setbits, sockindex);

            sockindex++;
        }
    }
    return bitmap;
}
