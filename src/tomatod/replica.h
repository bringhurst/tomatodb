#ifndef __TDB__TOMATOD_REPLICA_H
#define __TDB__TOMATOD_REPLICA_H

/*
 * Copyright 2013 Los Alamos National Security, LLC.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Author: Jon Bringhurst <jon@bringhurst.org>
 */

#include "context.h"
#include "task/task.h"
#include "timer.h"
#include "ut/uthash.h"

#define TDB_REPLICA_STACK_SIZE 32768

#define TDB_REPLICA_KEY_DEFAULT_LOCATION "/location"

typedef struct TDB_replica_t {
    TDB_db_t* db;
    TDB_ctx_t* ctx;

    TDB_timer_t* election_timer;
    struct timeval* election_timeout;

    Channel* data_chan;
    Channel* vote_chan;
    Channel* append_chan;

    char* target_role;
    UT_array* foreign_replicas;
    uuid_t uuid;

    uint64_t current_term;
    uint64_t last_log_index;

    UT_hash_handle hh;
} TDB_replica_t;

void TDB_replica_task(TDB_replica_t* replica);

int TDB_replica_init(TDB_replica_t** replica);
void TDB_replica_free(TDB_replica_t* replica);

int TDB_replica_persist_state(TDB_replica_t* replica, \
                              UT_array* log, \
                              TDB_addr_t* term_vote, \
                              uint64_t current_term);

#endif /* __TDB__TOMATOD_REPLICA_H */
