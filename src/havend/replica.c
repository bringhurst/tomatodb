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
#include "pack/data_msg.h"
#include "routing.h"
#include "settings.h"
#include "task/task.h"

#include "replica.h"
#include "log.h"

/** The stream to send log messages to. */
extern FILE* HVN_debug_stream;

/** The log level to output. */
extern HVN_loglevel HVN_debug_level;

int HVN_replica_follower(HVN_replica_t* replica, char* role)
{
    LOG(HVN_LOG_INFO, "Replica has entered follower state.");

    HVN_INTENTIONALLY_UNUSED_VARIABLE(replica);
    HVN_INTENTIONALLY_UNUSED_VARIABLE(role);

    // TODO: Respond to RPCs from candidates and leaders.

    // TODO: Convert to candidate if election timeout elapses without either
    //           1. Receiving valid AppendEntries RPC, or
    //           2. Granting vote to candidate.

    return HVN_ERROR;
}

int HVN_replica_candidate(HVN_replica_t* replica, char* role)
{
    LOG(HVN_LOG_INFO, "Replica has entered candidate state.");

    HVN_INTENTIONALLY_UNUSED_VARIABLE(replica);
    HVN_INTENTIONALLY_UNUSED_VARIABLE(role);

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

int HVN_replica_leader(HVN_replica_t* replica, char* role)
{
    char* op_packed;
    size_t op_packed_len;
    HVN_msg_client_data_t* client_data_msg;

    HVN_INTENTIONALLY_UNUSED_VARIABLE(role);

    LOG(HVN_LOG_INFO, "Replica has entered leader state.");

    // Cache the local last log index from disk.
    if(HVN_replica_cache_last_log_index(replica) != HVN_SUCCESS) {
        LOG(HVN_LOG_ERR, "Could not determine last log index.");
        return HVN_ERROR;
    }
    else {
        LOG(HVN_LOG_INFO, "Last log index for this leader replica is `%zu'.", replica->last_log_index);
    }

    // TODO: Cache replica list from disk.

    // TODO: Skip until client command accept works -- so followers can join the quorum. Initialize nextIndex for each follower to the local last log index + 1.

    // TODO: Skip until client command accept works. Send initial empty AppendEntries RPCs (heartbeat) to each follower.
    //           1. Repeat during idle periods to prevent election timeouts.

    // Accept data commands from clients.
    LOG(HVN_LOG_DBG, "Leader replica is waiting for client data messages.");
    client_data_msg = chanrecvp(replica->data_chan);

    HVN_proto_print_data_msg(client_data_msg);
    LOG(HVN_LOG_DBG, "Leader replica received a data msg.");

    // Pack and append to local log.
    HVN_clnt_proto_pack_data_msgpack(client_data_msg, &op_packed_len, &op_packed);
    HVN_replica_append_to_log(replica, op_packed, op_packed_len);

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

int HVN_replica_cache_last_log_index(HVN_replica_t* replica)
{
    uint64_t* last_log_index;

    if(HVN_db_unsafe_get_uint64(replica->db, \
                                HVN_CONSENSUS_MD_LAST_LOG_INDEX, strlen(HVN_CONSENSUS_MD_LAST_LOG_INDEX),
                                &last_log_index) != HVN_SUCCESS) {
        LOG(HVN_LOG_ERR, "Could not determine the last log index.");
        return HVN_ERROR;
    }

    replica->last_log_index = *last_log_index;
    return HVN_SUCCESS;
}

int HVN_replica_overwrite_last_log_index(HVN_replica_t* replica, uint64_t last_log_index)
{
    if(HVN_db_unsafe_put_uint64(replica->db, \
                                HVN_CONSENSUS_MD_LAST_LOG_INDEX, strlen(HVN_CONSENSUS_MD_LAST_LOG_INDEX), \
                                last_log_index) != HVN_SUCCESS) {
        LOG(HVN_LOG_ERR, "Could not overwrite the last log index for this replica.");
        return HVN_ERROR;
    }

    return HVN_SUCCESS;
}

int HVN_replica_append_to_log(HVN_replica_t* replica, char* packed_op, size_t packed_op_len)
{
    char* err;
    leveldb_writebatch_t* log_batch;

    char* log_term_key = (char*) malloc(sizeof(char) * _POSIX_PATH_MAX);
    char* log_cmd_key = (char*) malloc(sizeof(char) * _POSIX_PATH_MAX);

    uint64_t* current_term = &(replica->current_term);

    sprintf(log_term_key, HVN_CONSENSUS_MD_LOG_FMT_TERM, replica->current_term);
    sprintf(log_cmd_key, HVN_CONSENSUS_MD_LOG_FMT_CMD, replica->last_log_index + 1);

    log_batch = leveldb_writebatch_create();
    leveldb_writebatch_put(log_batch, log_term_key, strlen(log_term_key), (char*) current_term, sizeof(uint64_t));
    leveldb_writebatch_put(log_batch, log_cmd_key, strlen(log_cmd_key), packed_op, packed_op_len);
    leveldb_write(replica->db->handle, replica->db->write_options, log_batch, &err);

    replica->last_log_index++;

    if(HVN_replica_overwrite_last_log_index(replica, replica->last_log_index) != HVN_SUCCESS) {
        LOG(HVN_LOG_ERR, "Could not update last log index.");
        return HVN_ERROR;
    }

    free(log_term_key);
    free(log_cmd_key);

    return HVN_SUCCESS;
}

void HVN_replica_task(HVN_replica_t* replica)
{
    char* role;
    bool is_active = true;

    LOG(HVN_LOG_DBG, "Entered replica task.");

    while(is_active) {

        if(HVN_db_unsafe_get_char(replica->db, HVN_CONSENSUS_MD_STATE,
                                  strlen(HVN_CONSENSUS_MD_STATE), &role) != HVN_SUCCESS) {
            LOG(HVN_LOG_ERR, "Not loading this replica. Failed to read consensus state.");
            return;
        }

        switch(*role) {
            case HVN_CONSENSUS_MD_STATE_LEADER:

                if(HVN_replica_leader(replica, role) != HVN_SUCCESS) {
                    LOG(HVN_LOG_ERR, "Replica encountered an error while in the leader state.");
                    taskexit(EXIT_FAILURE);
                }

                break;

            case HVN_CONSENSUS_MD_STATE_FOLLOWER:

                if(HVN_replica_follower(replica, role) != HVN_SUCCESS) {
                    LOG(HVN_LOG_ERR, "Replica encountered an error while in the follower state.");
                    taskexit(EXIT_FAILURE);
                }

                break;

            case HVN_CONSENSUS_MD_STATE_CANDIDATE:

                if(HVN_replica_candidate(replica, role) != HVN_SUCCESS) {
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

        free(role);
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

    (*replica)->data_chan = chancreate(\
        sizeof(HVN_msg_client_data_t*), 0);

    return HVN_SUCCESS;
}

void HVN_replica_free(HVN_replica_t* replica)
{
    free(replica);
}

// Bootstrap a new location leader replica in the specified context. Set the
// UUID of the new replica to the uuid pointer.
int HVN_replica_bootstrap_location(HVN_replica_t* replica, HVN_ctx_t* ctx, uuid_t* uuid)
{
    char* key = (char*) malloc(sizeof(char) * HVN_MAX_KEY_SIZE);
    strncpy(key, HVN_REPLICA_KEY_DEFAULT_LOCATION, HVN_MAX_KEY_SIZE);

    return HVN_replica_bootstrap_leader(replica, ctx, uuid, key);
}

// Bootstrap a new leader replica in the specified context. Set the UUID of
// the new replica to the uuid pointer.
int HVN_replica_bootstrap_leader(HVN_replica_t* replica, HVN_ctx_t* ctx, uuid_t* uuid, char* path_key)
{
    char state = HVN_CONSENSUS_MD_STATE_LEADER;

    uint64_t bootstrap_term = 0;
    uint64_t bootstrap_index = 0;

    LOG(HVN_LOG_INFO, "Bootstrapping a replica leader on interface `%s:%d'.", \
        ctx->listen_addr, ctx->listen_port);

    if(HVN_db_validate_key(path_key) != true) {
        LOG(HVN_LOG_ERR, "Invalid leader base key format `%s'.", path_key);
        return HVN_ERROR;
    }
    else {
        LOG(HVN_LOG_DBG, "Using leader base key `%s'.", path_key);
    }

    if(HVN_generate_uuid(&replica->uuid) != HVN_SUCCESS) {
        LOG(HVN_LOG_ERR, "Failed to generate a new replica UUID.");
        return HVN_ERROR;
    }
    else {
        uuid_copy(*uuid, replica->uuid);
    }

    if(HVN_replica_bootstrap_db(replica) != HVN_SUCCESS) {
        LOG(HVN_LOG_ERR, "Failed to bootstrap a new replica database.");
        return HVN_ERROR;
    }

    if(HVN_db_unsafe_put_char(replica->db, HVN_CONSENSUS_MD_STATE,
                              strlen(HVN_CONSENSUS_MD_STATE), state) != HVN_SUCCESS) {
        LOG(HVN_LOG_ERR, "Failed to set the state for bootstrapping a leader.");
        return HVN_ERROR;
    }

    if(HVN_db_unsafe_put_uint64(replica->db, HVN_CONSENSUS_MD_TERM,
                                strlen(HVN_CONSENSUS_MD_TERM), bootstrap_term) != HVN_SUCCESS) {
        LOG(HVN_LOG_ERR, "Failed to set the state for bootstrapping a leader.");
        return HVN_ERROR;
    }

    if(HVN_db_unsafe_put_uint64(replica->db, HVN_CONSENSUS_MD_LAST_LOG_INDEX,
                                strlen(HVN_CONSENSUS_MD_LAST_LOG_INDEX), bootstrap_index) != HVN_SUCCESS) {
        LOG(HVN_LOG_ERR, "Failed to set the initial log index for bootstrapping a leader.");
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

int HVN_load_existing_replicas_from_disk(HVN_ctx_t* ctx)
{
    DIR* dir;
    struct dirent* ent;
    HVN_replica_t* replica;

    char* db_base_path = (char*) malloc(sizeof(char) * _POSIX_PATH_MAX);
    char* db_absolute_path = (char*) malloc(sizeof(char) * _POSIX_PATH_MAX);

    sprintf(db_base_path, "%s%s", \
            HVN_BASE_STATE_DIR, HVN_DATA_DB_PREFIX);

    if((dir = opendir(db_base_path)) != NULL) {
        while((ent = readdir(dir)) != NULL) {
            if(strlen(ent->d_name) == UUID_STR_LEN - 1) {
                sprintf(db_absolute_path, "%s/%s", db_base_path, ent->d_name);
                LOG(HVN_LOG_INFO, "Loading existing replica from `%s'.", db_absolute_path);

                if(HVN_replica_init(&replica) != HVN_SUCCESS) {
                    LOG(HVN_LOG_ERR, "Failed to allocate memory for replica `%s'.", ent->d_name);
                    return HVN_ERROR;
                }

                replica->ctx = ctx;

                if(HVN_db_init(&(replica->db), db_absolute_path) != HVN_SUCCESS) {
                    LOG(HVN_LOG_INFO, "Failed to open replica DB with UUID of `%s'.", ent->d_name);
                    free(replica);
                    return HVN_ERROR;
                }

                if(uuid_parse(ent->d_name, replica->uuid) != 0) {
                    LOG(HVN_LOG_INFO, "Failed to parse replica with UUID of `%s'.", ent->d_name);
                    free(replica);
                    return HVN_ERROR;
                }

                taskcreate((void (*)(void*))HVN_replica_task, replica, HVN_REPLICA_STACK_SIZE);
                HASH_ADD(hh, ctx->replicas, uuid, sizeof(uuid_t), replica);
            }
        }

        closedir(dir);
    }
    else {
        LOG(HVN_LOG_INFO, "Replica state directory can not be read. Server is starting empty.");
    }

    return HVN_SUCCESS;
}

/* EOF */
