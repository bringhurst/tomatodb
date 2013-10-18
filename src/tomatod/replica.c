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

#include <dirent.h>

#include "candidate.h"
#include "consensus.h"
#include "follower.h"
#include "leader.h"
#include "log.h"
#include "pack/addr_struct.h"
#include "pack/append_msg.h"
#include "pack/data_msg.h"
#include "pack/vote_msg.h"
#include "routing.h"
#include "settings.h"
#include "task/task.h"

#include "replica.h"

/** The stream to send log messages to. */
extern FILE* TDB_debug_stream;

/** The log level to output. */
extern TDB_loglevel TDB_debug_level;

int TDB_replica_persist_state(TDB_replica_t* replica, \
                              UT_array* log, \
                              TDB_addr_t* term_vote, \
                              uint64_t current_term)
{
    TDB_INTENTIONALLY_UNUSED_VARIABLE(replica);
    TDB_INTENTIONALLY_UNUSED_VARIABLE(log);
    TDB_INTENTIONALLY_UNUSED_VARIABLE(term_vote);
    TDB_INTENTIONALLY_UNUSED_VARIABLE(current_term);

    //TODO persist log, vote, and term to leveldb.

    return TDB_ERROR;
}

void TDB_replica_task(TDB_replica_t* replica)
{
    bool is_active = true;

    taskname("replica");
    taskstate("new");
    LOG(TDB_LOG_DBG, "Entered replica task.");

    while(is_active) {

        if(TDB_db_unsafe_get_char(replica->db, TDB_CONSENSUS_MD_STATE,
                                  strlen(TDB_CONSENSUS_MD_STATE), &(replica->target_role)) != TDB_SUCCESS) {
            LOG(TDB_LOG_ERR, "Not loading this replica. Failed to read consensus state.");
            return;
        }

        switch(*(replica->target_role)) {
            case TDB_CONSENSUS_MD_STATE_LEADER:
                taskstate("leader");

                if(TDB_replica_leader(replica) != TDB_SUCCESS) {
                    LOG(TDB_LOG_ERR, "Replica encountered an error while in the leader state.");
                    taskexit(EXIT_FAILURE);
                }

                break;

            case TDB_CONSENSUS_MD_STATE_FOLLOWER:
                taskstate("follower");

                if(TDB_replica_follower(replica) != TDB_SUCCESS) {
                    LOG(TDB_LOG_ERR, "Replica encountered an error while in the follower state.");
                    taskexit(EXIT_FAILURE);
                }

                break;

            case TDB_CONSENSUS_MD_STATE_CANDIDATE:
                taskstate("candidate");

                if(TDB_replica_candidate(replica) != TDB_SUCCESS) {
                    LOG(TDB_LOG_ERR, "Replica encountered an error while in the candidate state.");
                    taskexit(EXIT_FAILURE);
                }

                break;

            case TDB_CONSENSUS_MD_STATE_HALT:
                taskstate("halting");
                LOG(TDB_LOG_ERR, "Replica is preparing to shut down.");
                is_active = false;
                break;

            default:
                taskstate("unknown");
                LOG(TDB_LOG_ERR, "Encountered unknown replica state. Exiting task.");
                taskexit(EXIT_FAILURE);
                break;
        }
    }

    LOG(TDB_LOG_INFO, "Replica task is ending.");
    taskexit(EXIT_SUCCESS);
}

int TDB_replica_init(TDB_replica_t** replica)
{
    *replica = (TDB_replica_t*) malloc(sizeof(TDB_replica_t));

    if(*replica == NULL) {
        LOG(TDB_LOG_ERR, "Failed to allocate memory for a new replica.");
        return TDB_ERROR;
    }

    (*replica)->last_log_index = 0;
    (*replica)->current_term = 0;

    (*replica)->append_chan = chancreate(sizeof(TDB_attach_msg_t*), 0);
    (*replica)->data_chan = chancreate(sizeof(TDB_attach_msg_t*), 0);
    (*replica)->vote_chan = chancreate(sizeof(TDB_attach_msg_t*), 0);

    return TDB_SUCCESS;
}

void TDB_replica_free(TDB_replica_t* replica)
{
    free(replica);
}

/* EOF */
