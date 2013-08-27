#ifndef __HVN__HAVEND_CONSENSUS_H
#define __HVN__HAVEND_CONSENSUS_H

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

#include "attach.h"
#include "database.h"

/* Base directory to hold replicate state. */
#define HVN_CONSENSUS_MD_BASE_KEY          "/.consensus"

/* Consistently replicated keys. */
#define HVN_CONSENSUS_MD_REPLICA_LIST      "replica_list"
#define HVN_CONSENSUS_MD_ELECTION_TIMEOUT  "election_timeout"

/* Non-replicated local keys. */
#define HVN_CONSENSUS_MD_COORDINATOR       "__local_coordinator"
#define HVN_CONSENSUS_MD_STATE             "__local_state"
#define HVN_CONSENSUS_MD_TERM              "__local_term"

typedef struct HVN_consensus_vote_t {
    uint64_t candidate_term;
    uint64_t last_log_index;
    uint64_t last_log_term;
    uuid_t candidate_id;
} HVN_consensus_vote_t;

typedef struct HVN_consensus_append_t {
    uint64_t leader_term;
    uint64_t prev_log_index;
    uint64_t prev_log_term;
    uint64_t commit_index;
    uuid_t leader_id;
    UT_array* log_entries;
} HVN_consensus_append_t;

int HVN_consensus_exec(HVN_attach_t* client, HVN_db_op_t* op);

#endif /* __HVN__HAVEND_CONSENSUS_H */
