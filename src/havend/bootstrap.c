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
#include "log.h"
#include "settings.h"

#include "bootstrap.h"

/** The stream to send log messages to. */
extern FILE* HVN_debug_stream;

/** The log level to output. */
extern HVN_loglevel HVN_debug_level;

// Bootstrap a new location leader replica in the specified context. Set the
// UUID of the new replica to the uuid pointer.
int HVN_bootstrap_location(HVN_replica_t* replica, HVN_ctx_t* ctx, uuid_t* uuid)
{
    char* key = (char*) malloc(sizeof(char) * HVN_MAX_KEY_SIZE);
    strncpy(key, HVN_REPLICA_KEY_DEFAULT_LOCATION, HVN_MAX_KEY_SIZE);

    return HVN_bootstrap_leader(replica, ctx, uuid, key);
}

// Bootstrap a new leader replica in the specified context. Set the UUID of
// the new replica to the uuid pointer.
int HVN_bootstrap_leader(HVN_replica_t* replica, HVN_ctx_t* ctx, uuid_t* uuid, char* path_key)
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

    if(HVN_bootstrap_replica_db(replica) != HVN_SUCCESS) {
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

    // TODO: bootstrap the addr list with this node only.

    taskcreate((void (*)(void*))HVN_replica_task, replica, HVN_REPLICA_STACK_SIZE);
    HASH_ADD(hh, ctx->replicas, uuid, sizeof(uuid_t), replica);

    return HVN_SUCCESS;
}

int HVN_bootstrap_follower()
{
    // TODO: query location quorum to find location leader.
    // TODO: create key value store.
    // TODO: create replica task.

    LOG(HVN_LOG_ERR, "Bootstrap follower is not implemented.");
    return HVN_ERROR;
}

int HVN_bootstrap_replica_db(HVN_replica_t* replica)
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

int HVN_bootstrap_replicas_from_disk(HVN_ctx_t* ctx)
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
