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

#include <stdlib.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#include "ares.h"
#include "ares_private.h"

void ares__close_sockets(ares_channel channel, struct server_state* server) {
    struct send_request* sendreq;

    /* Free all pending output buffers. */
    while (server->qhead) {
        /* Advance server->qhead; pull out query as we go. */
        sendreq = server->qhead;
        server->qhead = sendreq->next;
        if (sendreq->data_storage != NULL) free(sendreq->data_storage);
        free(sendreq);
    }
    server->qtail = NULL;

    /* Reset any existing input buffer. */
    if (server->tcp_buffer) free(server->tcp_buffer);
    server->tcp_buffer = NULL;
    server->tcp_lenbuf_pos = 0;

    /* Reset brokenness */
    server->is_broken = 0;

    /* Close the TCP and UDP sockets. */
    if (server->tcp_socket != ARES_SOCKET_BAD) {
        SOCK_STATE_CALLBACK(channel, server->tcp_socket, 0, 0);
        sclose(server->tcp_socket);
        server->tcp_socket = ARES_SOCKET_BAD;
        server->tcp_connection_generation = ++channel->tcp_connection_generation;
    }
    if (server->udp_socket != ARES_SOCKET_BAD) {
        SOCK_STATE_CALLBACK(channel, server->udp_socket, 0, 0);
        sclose(server->udp_socket);
        server->udp_socket = ARES_SOCKET_BAD;
    }
}
