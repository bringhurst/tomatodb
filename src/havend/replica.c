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
extern FILE* HVN_debug_stream;

/** The log level to output. */
extern HVN_loglevel HVN_debug_level;

int HVN_replica_persist_state(HVN_replica_t* replica, \
                              UT_array* log, \
                              HVN_addr_t* term_vote, \
                              uint64_t current_term)
{
    //TODO persist log, vote, and term to leveldb.

    return HVN_ERROR;
}

void HVN_replica_task(HVN_replica_t* replica)
{
    bool is_active = true;

    LOG(HVN_LOG_DBG, "Entered replica task.");

    while(is_active) {

        if(HVN_db_unsafe_get_char(replica->db, HVN_CONSENSUS_MD_STATE,
                                  strlen(HVN_CONSENSUS_MD_STATE), &(replica->target_role)) != HVN_SUCCESS) {
            LOG(HVN_LOG_ERR, "Not loading this replica. Failed to read consensus state.");
            return;
        }

        switch(*(replica->target_role)) {
            case HVN_CONSENSUS_MD_STATE_LEADER:

                if(HVN_replica_leader(replica) != HVN_SUCCESS) {
                    LOG(HVN_LOG_ERR, "Replica encountered an error while in the leader state.");
                    taskexit(EXIT_FAILURE);
                }

                break;

            case HVN_CONSENSUS_MD_STATE_FOLLOWER:

                if(HVN_replica_follower(replica) != HVN_SUCCESS) {
                    LOG(HVN_LOG_ERR, "Replica encountered an error while in the follower state.");
                    taskexit(EXIT_FAILURE);
                }

                break;

            case HVN_CONSENSUS_MD_STATE_CANDIDATE:

                if(HVN_replica_candidate(replica) != HVN_SUCCESS) {
                    LOG(HVN_LOG_ERR, "Replica encountered an error while in the candidate state.");
                    taskexit(EXIT_FAILURE);
                }

                break;

            case HVN_CONSENSUS_MD_STATE_HALT:
                LOG(HVN_LOG_ERR, "Replica is preparing to shut down.");
                is_active = false;
                break;

            default:
                LOG(HVN_LOG_ERR, "Encountered unknown replica state. Exiting task.");
                taskexit(EXIT_FAILURE);
                break;
        }
    }

    LOG(HVN_LOG_INFO, "Replica task is ending.");
    taskexit(EXIT_SUCCESS);
}

int HVN_replica_init(HVN_replica_t** replica)
{
    *replica = (HVN_replica_t*) malloc(sizeof(HVN_replica_t));

    if(*replica == NULL) {
        LOG(HVN_LOG_ERR, "Failed to allocate memory for a new replica.");
        return HVN_ERROR;
    }

    (*replica)->last_log_index = 0;
    (*replica)->current_term = 0;

    (*replica)->append_chan = chancreate(sizeof(HVN_msg_append_t*), 0);
    (*replica)->data_chan = chancreate(sizeof(HVN_msg_data_t*), 0);
    (*replica)->vote_chan = chancreate(sizeof(HVN_msg_vote_t*), 0);

    return HVN_SUCCESS;
}

void HVN_replica_free(HVN_replica_t* replica)
{
    free(replica);
}

/* EOF */
