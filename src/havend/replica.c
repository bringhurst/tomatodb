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

#include "consensus.h"
#include "routing.h"
#include "settings.h"
#include "task/task.h"

#include "replica.h"
#include "log.h"

/** The stream to send log messages to. */
extern FILE* HVN_debug_stream;

/** The log level to output. */
extern HVN_loglevel HVN_debug_level;

void HVN_replica_task(HVN_replica_t* replica)
{
    uint32_t replica_role;

    LOG(HVN_LOG_DBG, "Entered replica task.");

    // TODO: check __state in database and store in replica_role.

    switch(replica_role) {
        case HVN_CONSENSUS_MD_STATE_LEADER:
            
            break;
        case HVN_CONSENSUS_MD_STATE_FOLLOWER:
            break;
        case HVN_CONSENSUS_MD_STATE_CANDIDATE:
            break;
        default:
            LOG(HVN_LOG_ERR, "Encountered unknown replica state. Exiting task.");
            taskexit(EXIT_FAILURE);
            break;
    }

    taskexit(EXIT_SUCCESS);
}

int HVN_replica_follower(HVN_replica_t* replica)
{
    // TODO: Respond to RPCs from candidates and leaders.

    // TODO: Convert to candidate if election timeout elapses without either
    //           1. Receiving valid AppendEntries RPC, or
    //           2. Granting vote to candidate.

    return HVN_ERROR;
}

int HVN_replica_candidate(HVN_replica_t* replica)
{
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
    // TODO: Initialize nextIndex for each to last log index + 1.

    // TODO: Send initial empty AppendEntries RPCs (heartbeat) to each follower.
    //           1. Repeat during idle periods to prevent election timeouts.

    // TODO: Accept commands from clients, append new entries to local log.

    // TODO: Whenever last log index is greater or equal to nextIndex for a follower:
    //           1. Send AppendEntries RPC with log entries starting at nextIndex.
    //               a. Update nextIndex if successful.
    //               b. If AppendEntries fails because of log inconsistency, decrement
    //                  nextIndex and retry.

    // TODO: Mark entries commited if stored on a majority of servers and some entry
    //       from current term is stored on a majority of servers. Apply newly committed
    //       entries to state machine.

    // TODO: Step down if currentTerm changes.

    return HVN_ERROR;
}

int HVN_replica_init(HVN_replica_t** replica)
{
    *replica = (HVN_replica_t*) malloc(sizeof(HVN_replica_t));

    if(*replica == NULL) {
        LOG(HVN_LOG_ERR, "Failed to allocate memory for a new replica.");
        return HVN_ERROR;
    }

    utarray_new((*replica)->quorum_addrs, &ut_str_icd);
    return HVN_SUCCESS;
}

void HVN_replica_free(HVN_replica_t* replica)
{
    utarray_free(replica->quorum_addrs);
    free(replica);
}

// Bootstrap a new location leader replica in the specified context. Set the
// UUID of the new replica to the uuid pointer.
int HVN_replica_bootstrap_location(HVN_replica_t* replica, HVN_ctx_t* ctx, uuid_t* uuid)
{
    char* key = (char*) malloc(sizeof(char) * HVN_MAX_KEY_SIZE);
    strncpy(key, HVN_REPLICA_KEY_DEFAULT_LOCATION, HVN_MAX_KEY_SIZE);

    if(ctx->location_addrs != NULL) {
        LOG(HVN_LOG_ERR, "A location leader already exists.");
        //FIXME: allow override?
        return HVN_ERROR;
    } else {
        ctx->location_addrs = replica->quorum_addrs;
    }

    return HVN_replica_bootstrap_leader(replica, ctx, uuid, key);
}

// Bootstrap a new leader replica in the specified context. Set the UUID of
// the new replica to the uuid pointer.
int HVN_replica_bootstrap_leader(HVN_replica_t* replica, HVN_ctx_t* ctx, uuid_t* uuid, char* path_key)
{
    uint32_t state = HVN_CONSENSUS_MD_STATE_LEADER;

    LOG(HVN_LOG_INFO, "Bootstrapping a replica leader on interface `%s:%d'.", \
            ctx->listen_addr, ctx->listen_port);

    if(HVN_db_validate_key(path_key) != true) {
        LOG(HVN_LOG_ERR, "Invalid leader base key format `%s'.", path_key);
        return HVN_ERROR;
    } else {
        LOG(HVN_LOG_DBG, "Using leader base key `%s'.", path_key);
    }

    if(HVN_generate_uuid(&replica->uuid) != HVN_SUCCESS) {
        LOG(HVN_LOG_ERR, "Failed to generate a new replica UUID.");
        return HVN_ERROR;
    } else {
        uuid_copy(*uuid, replica->uuid);
    }

    if(HVN_replica_bootstrap_db(replica) != HVN_SUCCESS) {
        LOG(HVN_LOG_ERR, "Failed to bootstrap a new replica database.");
        return HVN_ERROR;
    }

    // FIXME: read location addrs from settings db.
    if(ctx->location_addrs == replica->quorum_addrs) {
        LOG(HVN_LOG_DBG, "Not attempting to register with a remote location service " \
                "(this replica is the location service).");
    } else {
        LOG(HVN_LOG_DBG, "Attempting to register with the location service.");
        // TODO: register with location quorum.
    }

    if(HVN_db_unsafe_put(replica->db, HVN_CONSENSUS_MD_STATE, 1, \
                         (char*) &state, 1) != HVN_SUCCESS) {
        LOG(HVN_LOG_ERR, "Failed to set the state for bootstrapping a leader.");
        return HVN_ERROR;
    }

    taskcreate((void (*)(void*))HVN_replica_task, replica, HVN_REPLICA_STACK_SIZE);
    HASH_ADD(hh, ctx->replicas, uuid, sizeof(uuid_t), replica);

    return HVN_SUCCESS;
}

int HVN_replica_bootstrap_follower()
{
    // TODO: query location quorum to find location leader.
    // TODO: create key value store.
    // TODO: create replica task.

    LOG(HVN_LOG_ERR, "Bootstrap follower is not implemented.");
    return HVN_ERROR;
}

int HVN_replica_bootstrap_db(HVN_replica_t* replica)
{
    char* db_path = (char*) malloc(sizeof(char) * _POSIX_PATH_MAX);
    char* uuid_string = (char*) malloc(sizeof(char) * UUID_STR_LEN);
    int offset, result;

    offset = sprintf(db_path, "%s%s", \
                     HVN_BASE_STATE_DIR, HVN_DATA_DB_PREFIX);

    if(HVN_ensure_directory_exists(db_path) != HVN_SUCCESS) {
        LOG(HVN_LOG_ERR, "Failed to create directory structure when preparing a replica DB.");
        return HVN_ERROR;
    }

    uuid_unparse(replica->uuid, uuid_string);
    sprintf(db_path + offset, "/%s", uuid_string);
    result = HVN_db_init(&replica->db, db_path);

    free(db_path);
    free(uuid_string);

    return result;
}

/* EOF */
