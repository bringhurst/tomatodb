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

/* Consistently replicated keys. */
#define HVN_CONSENSUS_MD_REPLICA_LIST       "/.consensus/replica_list"
#define HVN_CONSENSUS_MD_ELECTION_TIMEOUT   "/.consensus/election_timeout"

/* Non-replicated local persistent keys. */
#define HVN_CONSENSUS_MD_LEADER             "/.consensus/local/leader"
#define HVN_CONSENSUS_MD_LOG                "/.consensus/local/log"
#define HVN_CONSENSUS_MD_STATE              "/.consensus/local/state"
#define HVN_CONSENSUS_MD_TERM               "/.consensus/local/term"

/* Common log fmt keys. */
#define HVN_CONSENSUS_MD_LOG_FMT_IDX        "/.consensus/local/log/%zu"
#define HVN_CONSENSUS_MD_LOG_FMT_TERM       "/.consensus/local/log/%zu/term"
#define HVN_CONSENSUS_MD_LOG_FMT_CMD        "/.consensus/local/log/%zu/command"

/* Possible values for local state. */
#define HVN_CONSENSUS_MD_STATE_LEADER       'L'
#define HVN_CONSENSUS_MD_STATE_FOLLOWER     'F'
#define HVN_CONSENSUS_MD_STATE_CANDIDATE    'C'
#define HVN_CONSENSUS_MD_STATE_HALT         'H'

typedef struct HVN_consensus_vote_t {
    uint64_t candidate_term;
    uint64_t last_log_index;
    uint64_t last_log_term;
    uuid_t candidate_id;
} HVN_consensus_vote_t;

typedef struct HVN_consensus_vote_resp_t {
    uint64_t term;
    bool vote_granted;
} HVN_consensus_vote_resp_t;

typedef struct HVN_consensus_append_t {
    uint64_t leader_term;
    uint64_t prev_log_index;
    uint64_t prev_log_term;
    uint64_t commit_index;
    uuid_t leader_id;
    UT_array* log_entries;
} HVN_consensus_append_t;

typedef struct HVN_consensus_append_resp_t {
    uint64_t term;
    uint64_t conflict_term;
    bool success;
} HVN_consensus_append_resp_t;

int HVN_consensus_exec(HVN_attach_t* client, HVN_db_op_t* op);

#endif /* __HVN__HAVEND_CONSENSUS_H */
