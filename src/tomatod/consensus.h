#ifndef __TDB__TOMATOD_CONSENSUS_H
#define __TDB__TOMATOD_CONSENSUS_H

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
#define TDB_CONSENSUS_MD_REPLICA_LIST       "/.consensus/replica_list"
#define TDB_CONSENSUS_MD_ELECTION_TIMEOUT   "/.consensus/election_timeout"

/* Non-replicated local persistent keys. */
#define TDB_CONSENSUS_MD_LEADER             "/.consensus/local/leader"
#define TDB_CONSENSUS_MD_LAST_LOG_INDEX     "/.consensus/local/last_log_index"
#define TDB_CONSENSUS_MD_STATE              "/.consensus/local/state"
#define TDB_CONSENSUS_MD_TERM               "/.consensus/local/term"

/* Common log fmt keys. */
#define TDB_CONSENSUS_MD_LOG_FMT_TERM       "/.consensus/local/log/%zu/term"
#define TDB_CONSENSUS_MD_LOG_FMT_CMD        "/.consensus/local/log/%zu/command"

/* Possible values for local state. */
#define TDB_CONSENSUS_MD_STATE_LEADER       'L'
#define TDB_CONSENSUS_MD_STATE_FOLLOWER     'F'
#define TDB_CONSENSUS_MD_STATE_CANDIDATE    'C'
#define TDB_CONSENSUS_MD_STATE_HALT         'H'

int TDB_consensus_exec(TDB_attach_t* client, TDB_db_op_t* op);

#endif /* __TDB__TOMATOD_CONSENSUS_H */
