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
#include <assert.h>
#include <stdlib.h>
#include "ares.h"
#include "ares_private.h"

/*
 * ares_cancel() cancels all ongoing requests/resolves that might be going on
 * on the given channel. It does NOT kill the channel, use ares_destroy() for
 * that.
 */
void ares_cancel(ares_channel channel) {
    struct query* query;
    struct list_node* list_head;
    struct list_node* list_node;
    int i;

    list_head = &(channel->all_queries);
    for (list_node = list_head->next; list_node != list_head;) {
        query = list_node->data;
        list_node = list_node->next; /* since we're deleting the query */
        query->callback(query->arg, ARES_ECANCELLED, 0, NULL, 0);
        ares__free_query(query);
    }
#ifndef NDEBUG
    /* Freeing the query should remove it from all the lists in which it sits,
     * so all query lists should be empty now.
     */
    assert(ares__is_list_empty(&(channel->all_queries)));
    for (i = 0; i < ARES_QID_TABLE_SIZE; i++) {
        assert(ares__is_list_empty(&(channel->queries_by_qid[i])));
    }
    for (i = 0; i < ARES_TIMEOUT_TABLE_SIZE; i++) {
        assert(ares__is_list_empty(&(channel->queries_by_timeout[i])));
    }
#endif
    if (!(channel->flags & ARES_FLAG_STAYOPEN)) {
        if (channel->servers) {
            for (i = 0; i < channel->nservers; i++)
                ares__close_sockets(channel, &channel->servers[i]);
        }
    }
}
