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
#include "ares_nowarn.h"
#include "ares_private.h"

int ares_fds(ares_channel channel, fd_set* read_fds, fd_set* write_fds) {
    struct server_state* server;
    ares_socket_t nfds;
    int i;

    /* Are there any active queries? */
    int active_queries = !ares__is_list_empty(&(channel->all_queries));

    nfds = 0;
    for (i = 0; i < channel->nservers; i++) {
        server = &channel->servers[i];
        /* We only need to register interest in UDP sockets if we have
         * outstanding queries.
         */
        // printf("%s
        // active_queries:%d,server->udp_socket:%d,server->tcp_socket:%d\n",__func__,active_queries,server->udp_socket,server->tcp_socket);
        if (active_queries && server->udp_socket != ARES_SOCKET_BAD) {
            FD_SET(server->udp_socket, read_fds);
            if (server->udp_socket >= nfds) nfds = server->udp_socket + 1;
        }
        /* We always register for TCP events, because we want to know
         * when the other side closes the connection, so we don't waste
         * time trying to use a broken connection.
         */
        if (server->tcp_socket != ARES_SOCKET_BAD) {
            FD_SET(server->tcp_socket, read_fds);
            if (server->qhead) FD_SET(server->tcp_socket, write_fds);
            if (server->tcp_socket >= nfds) nfds = server->tcp_socket + 1;
        }
    }
    return (int)nfds;
}
