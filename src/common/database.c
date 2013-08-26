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

#include <limits.h>
#include <string.h>
#include <netdb.h>

#include "log.h"
#include "database.h"

#define HVN_CONFIG_DB_PREFIX "/config"
#define HVN_LOG_DB_PREFIX    "/log"
#define HVN_GROUP_DB_PREFIX  "/group"

/** The stream to send log messages to. */
extern FILE* HVN_debug_stream;

/** The log level to output. */
extern HVN_loglevel HVN_debug_level;

int HVN_db_init(HVN_db_t** db, char* path)
{
    char* db_err = NULL;

    HVN_db_t* new_db = \
                         (HVN_db_t*) malloc(sizeof(HVN_db_t));

    if(!new_db) {
        LOG(HVN_LOG_ERR, "Could not initialize memory for a new database.");
        return HVN_ERROR;
    }

    new_db->options = leveldb_options_create();
    leveldb_options_set_create_if_missing(new_db->options, 1);
    new_db->handle = leveldb_open(new_db->options, path, &db_err);

    if(db_err != NULL) {
        LOG(HVN_LOG_ERR, "Could not open the local database. %s", db_err);
        return HVN_ERROR;
    }

    new_db->path = path;
    *db = new_db;

    leveldb_free(db_err);
    return HVN_SUCCESS;
}

void HVN_db_close(HVN_db_t* db)
{
    leveldb_close(db->handle);
}

int HVN_db_destroy(HVN_db_t* db)
{
    char* db_err = NULL;

    leveldb_destroy_db(db->options, db->path, &db_err);

    if(db_err != NULL) {
        LOG(HVN_LOG_ERR, "Could not destroy the database at `%s'. %s", \
            db->path, db_err);
        return HVN_ERROR;
    }

    leveldb_free(db_err);
    return HVN_SUCCESS;
}

int HVN_db_write(HVN_db_trans_t* trans, char* key, size_t value_len, void* value)
{

}

int HVN_db_read(HVN_db_trans_t* trans, char* key, size_t value_len, void** value)
{

}

int HVN_db_exec(HVN_db_trans_t* trans)
{

}

bool HVN_db_validate_key(char* key)
{
    char* p = key;

    // FIXME: not designed for malicious intent.

    while (*p++) {
        if(*p == '\0') break;
        if((*p < 'a') || (*p > 'z')) {
            if((*p != '_') && (*p != '/')) {
                LOG(HVN_LOG_DBG, "Returning false on %c", *p);
                return false;
            }
        }
    }

    return true;
}

/* EOF */
