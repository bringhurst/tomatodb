#ifndef __HVN__HAVEND_REPLICA_H
#define __HVN__HAVEND_REPLICA_H

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
#include "ut/uthash.h"

#define HVN_REPLICA_STACK_SIZE 32768

#define HVN_REPLICA_KEY_DEFAULT_LOCATION "/location"

typedef struct HVN_replica_t {
    HVN_db_t* db;
    HVN_ctx_t* ctx;
    uuid_t uuid;
    uint64_t current_term;
    uint64_t last_log_index;
    Channel* data_chan;
    Channel* vote_chan;
    Channel* append_chan;
    UT_hash_handle hh;
} HVN_replica_t;

void HVN_replica_task(HVN_replica_t* replica);

int HVN_replica_init(HVN_replica_t** replica);
void HVN_replica_free(HVN_replica_t* replica);

int HVN_replica_follower(HVN_replica_t* replica, char* role);
int HVN_replica_candidate(HVN_replica_t* replica, char* role);
int HVN_replica_leader(HVN_replica_t* replica, char* role);

int HVN_replica_bootstrap_location(HVN_replica_t* replica, HVN_ctx_t* ctx, uuid_t* uuid);
int HVN_replica_bootstrap_leader(HVN_replica_t* replica, HVN_ctx_t* ctx, uuid_t* uuid, char* path_key);
int HVN_replica_bootstrap_follower(void);
int HVN_replica_bootstrap_db(HVN_replica_t* replica);

int HVN_load_existing_replicas_from_disk(HVN_ctx_t* ctx);
int HVN_replica_cache_last_log_index(HVN_replica_t* replica);
int HVN_replica_overwrite_last_log_index(HVN_replica_t* replica, uint64_t last_log_index);
int HVN_replica_append_to_log(HVN_replica_t* replica, char* packed_op, size_t packed_op_len);

#endif /* __HVN__HAVEND_REPLICA_H */
