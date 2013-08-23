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

#include "connection.h"
#include "ut/uthash.h"

#include "havend.h"

typedef struct HVN_replica_t {
    HVN_db_t* db;
    HVN_ctx_t* ctx;
    HVN_conn_t* connections;
    int fd;
    uuid_t uuid;
    UT_hash_handle hh;
} HVN_replica_t;

int HVN_replica_init(HVN_replica_t** replica);
int HVN_replica_free(HVN_replica_t* replica);

int HVN_replica_follower(HVN_replica_t* replica);
int HVN_replica_candidate(HVN_replica_t* replica);
int HVN_replica_leader(HVN_replica_t* replica);

int HVN_replica_bootstrap_leader(void);
int HVN_replica_bootstrap_follower(void);
int HVN_replica_bootstrap_location(void);

#endif /* __HVN__HAVEND_REPLICA_H */
