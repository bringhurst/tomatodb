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

#include "consensus.h"
#include "pack/addr_struct.h"
#include "pack/append_msg.h"
#include "pack/data_msg.h"
#include "pack/vote_msg.h"
#include "routing.h"
#include "settings.h"
#include "task/task.h"

#include "replica.h"
#include "log.h"

/** The stream to send log messages to. */
extern FILE* HVN_debug_stream;

/** The log level to output. */
extern HVN_loglevel HVN_debug_level;

int HVN_replica_follower(HVN_replica_t* replica)
{
    LOG(HVN_LOG_INFO, "Replica has entered follower state.");

    if(HVN_timer_init(&(replica->election_timer), replica->election_timeout, \
                      HVN_TIMER_CHANNEL_BACKLOG, (void (*)(void *)) HVN_replica_follower_handle_timeout, \
                      (void*) replica) != HVN_SUCCESS) {
        LOG(HVN_LOG_ERR, "Could not create an election timer for this follower replica.");
        return HVN_ERROR;
    }

    // TODO: Respond to RPCs from candidates and leaders.

    // TODO: Convert to candidate if election timeout elapses without either
    //           1. Receiving valid AppendEntries RPC, or
    //           2. Granting vote to candidate.

    return HVN_ERROR;
}

int HVN_replica_candidate(HVN_replica_t* replica)
{
    LOG(HVN_LOG_INFO, "Replica has entered candidate state.");

    HVN_INTENTIONALLY_UNUSED_VARIABLE(replica);

    // TODO: Increment current Term, vote for self.

    // TODO: Reset election timeout.

    // TODO: Send RequestVote RPCs to all other servers, wait for either:
    //           1. Votes received from majority of servers: become leader.
    //           2. AppendEntries RPC received from new leader: step down.
    //           3. Election timeout elapses without election resolution:
    //               a. Increment term, state new election.
    //           4. Discover higher term: step down.

    return HVN_ERROR;
}

int HVN_replica_leader(HVN_replica_t* replica)
{
    UT_array* replicas;
    UT_array* log;
    HVN_addr_t* vote;
    uint64_t term;

    LOG(HVN_LOG_INFO, "Replica has entered leader state.");

    // TODO: Skip until client command accept works -- so followers can join the quorum. Initialize nextIndex for each follower to the local last log index + 1.

    // TODO: Skip until client command accept works. Send initial empty AppendEntries RPCs (heartbeat) to each follower.
    //           1. Repeat during idle periods to prevent election timeouts.

    // Accept data commands from clients.
    LOG(HVN_LOG_DBG, "Leader replica is waiting for client data messages.");
    //client_data_msg = chanrecvp(replica->data_chan);
    //HVN_proto_print_data_msg(client_data_msg);
    //LOG(HVN_LOG_DBG, "Leader replica received a data msg.");

    // Pack and append to local log.
    //HVN_clnt_proto_pack_data_msgpack(client_data_msg, &op_packed_len, &op_packed);
    //HVN_replica_append_to_log(replica, op_packed, op_packed_len);

    // TODO: Whenever last log index is greater or equal to nextIndex for a follower:
    //           1. Send AppendEntries RPC with log entries starting at nextIndex.
    //               a. Update nextIndex if successful.
    //               b. If AppendEntries fails because of log inconsistency, decrement
    //                  nextIndex and retry.

    // TODO: Mark entries commited if stored on a majority of servers and some entry
    //       from current term is stored on a majority of servers. Apply newly committed
    //       entries to state machine.

    // TODO: Step down if currentTerm changes.

    return HVN_SUCCESS;
}

void HVN_replica_follower_handle_timeout(HVN_replica_t* replica) {
    *(replica->target_role) = HVN_CONSENSUS_MD_STATE_CANDIDATE;

    //TODO: send messages to replica->*_chan to tell it to shut down?
}

void HVN_replica_candidate_handle_timeout(HVN_replica_t* replica) {
    //TODO: increment term
    *(replica->target_role) = HVN_CONSENSUS_MD_STATE_CANDIDATE;

    //TODO: send messages to replica->*_chan to tell it to shut down?
}

void HVN_replica_leader_handle_timeout(HVN_replica_t* replica) {
    //TODO: send empty append messages to replicas.
    //TODO: step down if current term changes.
}

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

    (*replica)->append_chan_in = chancreate(sizeof(HVN_msg_append_t*), 0);
    (*replica)->data_chan_in = chancreate(sizeof(HVN_msg_client_data_t*), 0);
    (*replica)->vote_chan_in = chancreate(sizeof(HVN_msg_vote_t*), 0);

    (*replica)->append_chan_out = chancreate(sizeof(HVN_msg_append_resp_t*), 0);
    (*replica)->data_chan_out = chancreate(sizeof(HVN_msg_client_data_resp_t*), 0);
    (*replica)->vote_chan_out = chancreate(sizeof(HVN_msg_vote_resp_t*), 0);

    return HVN_SUCCESS;
}

void HVN_replica_free(HVN_replica_t* replica)
{
    free(replica);
}

/* EOF */
