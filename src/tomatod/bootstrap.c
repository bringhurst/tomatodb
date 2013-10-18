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
extern FILE* TDB_debug_stream;

/** The log level to output. */
extern TDB_loglevel TDB_debug_level;

// Bootstrap a new follower replica in the specified context.
int TDB_bootstrap_follower(TDB_replica_t* replica, TDB_ctx_t* ctx, uuid_t* uuid)
{
    char state = TDB_CONSENSUS_MD_STATE_FOLLOWER;

    uint64_t bootstrap_term = 0;
    uint64_t bootstrap_index = 0;

    LOG(TDB_LOG_INFO, "Bootstrapping a replica follower on interface `%s:%d'.", \
        ctx->listen_addr, ctx->listen_port);

    if(TDB_generate_uuid(&replica->uuid) != TDB_SUCCESS) {
        LOG(TDB_LOG_ERR, "Failed to generate a new follower replica UUID.");
        return TDB_ERROR;
    }
    else {
        uuid_copy(*uuid, replica->uuid);
    }

    if(TDB_bootstrap_replica_db(replica) != TDB_SUCCESS) {
        LOG(TDB_LOG_ERR, "Failed to bootstrap a new follower replica database.");
        return TDB_ERROR;
    }

    if(TDB_db_unsafe_put_char(replica->db, TDB_CONSENSUS_MD_STATE,
                              strlen(TDB_CONSENSUS_MD_STATE), state) != TDB_SUCCESS) {
        LOG(TDB_LOG_ERR, "Failed to set the state for bootstrapping a follower.");
        return TDB_ERROR;
    }

    if(TDB_db_unsafe_put_uint64(replica->db, TDB_CONSENSUS_MD_TERM,
                                strlen(TDB_CONSENSUS_MD_TERM), bootstrap_term) != TDB_SUCCESS) {
        LOG(TDB_LOG_ERR, "Failed to set the state for bootstrapping a follower.");
        return TDB_ERROR;
    }

    if(TDB_db_unsafe_put_uint64(replica->db, TDB_CONSENSUS_MD_LAST_LOG_INDEX,
                                strlen(TDB_CONSENSUS_MD_LAST_LOG_INDEX), bootstrap_index) != TDB_SUCCESS) {
        LOG(TDB_LOG_ERR, "Failed to set the initial log index for bootstrapping a follower.");
        return TDB_ERROR;
    }

    // TODO: bootstrap the addr list with this node only.

    taskcreate((void (*)(void*))TDB_replica_task, replica, TDB_REPLICA_STACK_SIZE);
    HASH_ADD(hh, ctx->replicas, uuid, sizeof(uuid_t), replica);

    return TDB_SUCCESS;
}

int TDB_bootstrap_replica_db(TDB_replica_t* replica)
{
    char* db_path = (char*) malloc(sizeof(char) * _POSIX_PATH_MAX);
    char* uuid_string = (char*) malloc(sizeof(char) * UUID_STR_LEN);
    int offset, result;

    offset = sprintf(db_path, "%s%s", \
                     TDB_BASE_STATE_DIR, TDB_DATA_DB_PREFIX);

    if(TDB_ensure_directory_exists(db_path) != TDB_SUCCESS) {
        LOG(TDB_LOG_ERR, "Failed to create directory structure when preparing a replica DB.");
        return TDB_ERROR;
    }

    uuid_unparse(replica->uuid, uuid_string);
    sprintf(db_path + offset, "/%s", uuid_string);
    result = TDB_db_init(&replica->db, db_path);

    free(db_path);
    free(uuid_string);

    return result;
}

int TDB_bootstrap_replicas_from_disk(TDB_ctx_t* ctx)
{
    DIR* dir;
    struct dirent* ent;
    TDB_replica_t* replica;

    char* db_base_path = (char*) malloc(sizeof(char) * _POSIX_PATH_MAX);
    char* db_absolute_path = (char*) malloc(sizeof(char) * _POSIX_PATH_MAX);

    sprintf(db_base_path, "%s%s", \
            TDB_BASE_STATE_DIR, TDB_DATA_DB_PREFIX);

    if((dir = opendir(db_base_path)) != NULL) {
        while((ent = readdir(dir)) != NULL) {
            if(strlen(ent->d_name) == UUID_STR_LEN - 1) {
                sprintf(db_absolute_path, "%s/%s", db_base_path, ent->d_name);
                LOG(TDB_LOG_INFO, "Loading existing replica from `%s'.", db_absolute_path);

                if(TDB_replica_init(&replica) != TDB_SUCCESS) {
                    LOG(TDB_LOG_ERR, "Failed to allocate memory for replica `%s'.", ent->d_name);
                    return TDB_ERROR;
                }

                replica->ctx = ctx;

                if(TDB_db_init(&(replica->db), db_absolute_path) != TDB_SUCCESS) {
                    LOG(TDB_LOG_INFO, "Failed to open replica DB with UUID of `%s'.", ent->d_name);
                    free(replica);
                    return TDB_ERROR;
                }

                if(uuid_parse(ent->d_name, replica->uuid) != 0) {
                    LOG(TDB_LOG_INFO, "Failed to parse replica with UUID of `%s'.", ent->d_name);
                    free(replica);
                    return TDB_ERROR;
                }

                taskcreate((void (*)(void*))TDB_replica_task, replica, TDB_REPLICA_STACK_SIZE);
                HASH_ADD(hh, ctx->replicas, uuid, sizeof(uuid_t), replica);
            }
        }

        closedir(dir);
    }
    else {
        LOG(TDB_LOG_INFO, "Replica state directory can not be read. Server is starting empty.");
    }

    return TDB_SUCCESS;
}

/* EOF */
