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
 */

#include "log.h"
#include "database.h"

/** The stream to send log messages to. */
extern FILE* HAVEN_debug_stream;

/** The log level to output. */
extern HAVEN_loglevel HAVEN_debug_level;

void HAVEN_init_internal_database(HAVEN_server_context_t* ctx)
{
    char* db_err = NULL;

    HAVEN_server_internal_database_t* internal_db = \
        (HAVEN_server_internal_database_t*) malloc(sizeof(HAVEN_server_internal_database_t));

    if(!internal_db) {
        LOG(HAVEN_LOG_ERR, "Could not initialize memory for the internal database.");
        return;
    }

    /* TODO: determine and set internal db id. */

    internal_db->options = leveldb_options_create();
    leveldb_options_set_create_if_missing(internal_db->options, 1);
    internal_db->db = leveldb_open(internal_db->options, internal_db->id, &db_err);

    if(db_err != NULL) {
        LOG(HAVEN_LOG_ERR, "Could not open the local database. %s", db_err);
        exit(EXIT_FAILURE);
    }

    ctx->internal_database = internal_db;
    leveldb_free(db_err);
}

void HAVEN_close_internal_database(HAVEN_server_context_t* ctx)
{
    leveldb_close(ctx->internal_database->db);
}

int HAVEN_destroy_internal_database(HAVEN_server_context_t* ctx)
{
    char* db_err = NULL;
    HAVEN_server_internal_database_t* db = ctx->internal_database;

    leveldb_destroy_db(db->options, db->id, &db_err);

    if(db_err != NULL) {
        LOG(HAVEN_LOG_ERR, "Could not destroy the local database. %s", db_err);
        return -1;
    }

    leveldb_free(db_err);
}

/* EOF */
