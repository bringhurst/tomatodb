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

#include <limits.h>
#include <string.h>
#include <netdb.h>

#include "log.h"
#include "database.h"

#define HAVEN_CONFIG_DB_PREFIX "/config"
#define HAVEN_LOG_DB_PREFIX    "/log"
#define HAVEN_GROUP_DB_PREFIX  "/group"

/** The stream to send log messages to. */
extern FILE* HAVEN_debug_stream;

/** The log level to output. */
extern HAVEN_loglevel HAVEN_debug_level;

int HAVEN_init_db(HAVEN_db_t** db, char* path)
{
    char* db_err = NULL;

    HAVEN_db_t* new_db = \
                         (HAVEN_db_t*) malloc(sizeof(HAVEN_db_t));

    if(!new_db) {
        LOG(HAVEN_LOG_ERR, "Could not initialize memory for a new database.");
        return HAVEN_ERROR;
    }

    new_db->options = leveldb_options_create();
    leveldb_options_set_create_if_missing(new_db->options, 1);
    new_db->handle = leveldb_open(new_db->options, path, &db_err);

    if(db_err != NULL) {
        LOG(HAVEN_LOG_ERR, "Could not open the local database. %s", db_err);
        return HAVEN_ERROR;
    }

    new_db->path = path;
    *db = new_db;

    leveldb_free(db_err);
    return HAVEN_SUCCESS;
}

void HAVEN_close_db(HAVEN_db_t* db)
{
    leveldb_close(db->handle);
}

int HAVEN_destroy_db(HAVEN_db_t* db)
{
    char* db_err = NULL;

    leveldb_destroy_db(db->options, db->path, &db_err);

    if(db_err != NULL) {
        LOG(HAVEN_LOG_ERR, "Could not destroy the database at `%s'. %s", \
            db->path, db_err);
        return HAVEN_ERROR;
    }

    leveldb_free(db_err);
    return HAVEN_SUCCESS;
}

/* EOF */
