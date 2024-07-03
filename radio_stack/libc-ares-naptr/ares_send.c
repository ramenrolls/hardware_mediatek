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

#ifdef HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif
#ifdef HAVE_NETINET_IN_H
#include <netinet/in.h>
#endif
#ifdef HAVE_ARPA_NAMESER_H
#include <arpa/nameser.h>
#else
#include "nameser.h"
#endif
#ifdef HAVE_ARPA_NAMESER_COMPAT_H
#include <arpa/nameser_compat.h>
#endif

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "ares.h"
#include "ares_dns.h"
#include "ares_private.h"

int ares_send(ares_channel channel, const unsigned char* qbuf, int qlen, ares_callback callback,
              void* arg) {
    struct query* query;
    int i;
    struct timeval now;

    /* Verify that the query is at least long enough to hold the header. */
    if (qlen < HFIXEDSZ || qlen >= (1 << 16)) {
        return ARES_EBADQUERY;
    }

    /* Allocate space for query and allocated fields. */
    query = malloc(sizeof(struct query));
    if (!query) {
        return ARES_ENOMEM;
    }
    query->tcpbuf = malloc(qlen + 2);
    if (!query->tcpbuf) {
        free(query);
        return ARES_ENOMEM;
    }
    query->server_info = malloc(channel->nservers * sizeof(query->server_info[0]));
    if (!query->server_info) {
        free(query->tcpbuf);
        free(query);
        return ARES_ENOMEM;
    }

    /* Compute the query ID.  Start with no timeout. */
    query->qid = (unsigned short)DNS_HEADER_QID(qbuf);
    query->timeout.tv_sec = 0;
    query->timeout.tv_usec = 0;

    /* Form the TCP query buffer by prepending qlen (as two
     * network-order bytes) to qbuf.
     */
    query->tcpbuf[0] = (unsigned char)((qlen >> 8) & 0xff);
    query->tcpbuf[1] = (unsigned char)(qlen & 0xff);
    memcpy(query->tcpbuf + 2, qbuf, qlen);
    query->tcplen = qlen + 2;

    /* Fill in query arguments. */
    query->qbuf = query->tcpbuf + 2;
    query->qlen = qlen;
    query->callback = callback;
    query->arg = arg;

    /* Initialize query status. */
    query->try_count = 0;

    /* Choose the server to send the query to. If rotation is enabled, keep track
     * of the next server we want to use. */
    query->server = channel->last_server;
    if (channel->rotate == 1) channel->last_server = (channel->last_server + 1) % channel->nservers;

    for (i = 0; i < channel->nservers; i++) {
        query->server_info[i].skip_server = 0;
        query->server_info[i].tcp_connection_generation = 0;
    }
    query->using_tcp = (channel->flags & ARES_FLAG_USEVC) || qlen > PACKETSZ;
    query->error_status = ARES_ECONNREFUSED;
    query->timeouts = 0;

    /* Initialize our list nodes. */
    ares__init_list_node(&(query->queries_by_qid), query);
    ares__init_list_node(&(query->queries_by_timeout), query);
    ares__init_list_node(&(query->queries_to_server), query);
    ares__init_list_node(&(query->all_queries), query);

    /* Chain the query into the list of all queries. */
    ares__insert_in_list(&(query->all_queries), &(channel->all_queries));
    /* Keep track of queries bucketed by qid, so we can process DNS
     * responses quickly.
     */
    ares__insert_in_list(&(query->queries_by_qid),
                         &(channel->queries_by_qid[query->qid % ARES_QID_TABLE_SIZE]));

    /* Perform the first query action. */
    now = ares__tvnow();
    printf("%s the first query\n", __func__);
    ares__send_query(channel, query, &now);
    return ARES_SUCCESS;
}
