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
#include <stdlib.h>
#include <string.h>
#include <netdb.h>

#include "log.h"
#include "kvs.h"

#define TDB_CONFIG_DB_PREFIX "/config"
#define TDB_LOG_DB_PREFIX    "/log"
#define TDB_GROUP_DB_PREFIX  "/group"

/** The stream to send log messages to. */
extern FILE* TDB_debug_stream;

/** The log level to output. */
extern TDB_loglevel TDB_debug_level;

int TDB_db_init(TDB_db_t** db, char* path)
{
    char* db_err = NULL;
    leveldb_comparator_t* comparator;

    TDB_db_t* new_db = \
                       (TDB_db_t*) malloc(sizeof(TDB_db_t));

    if(!new_db) {
        LOG(TDB_LOG_ERR, "Could not initialize memory for a new database.");
        return TDB_ERROR;
    }

    comparator = leveldb_comparator_create(NULL,
                                           TDB_db_comparator_destroy,
                                           TDB_db_comparator_compare,
                                           TDB_db_comparator_name);

    new_db->options = leveldb_options_create();

    leveldb_options_set_comparator(new_db->options, comparator);
    leveldb_options_set_create_if_missing(new_db->options, 1);

    new_db->handle = leveldb_open(new_db->options, path, &db_err);

    if(db_err != NULL) {
        LOG(TDB_LOG_ERR, "Could not open the local database. %s", db_err);
        return TDB_ERROR;
    }

    new_db->read_options = leveldb_readoptions_create();
    new_db->write_options = leveldb_writeoptions_create();

    new_db->path = path;
    *db = new_db;

    leveldb_free(db_err);
    return TDB_SUCCESS;
}

void TDB_db_close(TDB_db_t* db)
{
    leveldb_close(db->handle);
}

int TDB_db_destroy(TDB_db_t* db)
{
    char* db_err = NULL;

    leveldb_destroy_db(db->options, db->path, &db_err);

    if(db_err != NULL) {
        LOG(TDB_LOG_ERR, "Could not destroy the database at `%s'. %s", \
            db->path, db_err);
        return TDB_ERROR;
    }

    leveldb_free(db_err);
    return TDB_SUCCESS;
}

void TDB_db_comparator_destroy(void* arg)
{
    TDB_INTENTIONALLY_UNUSED_VARIABLE(arg);
    // FIXME: what does this do? Should something be here?
}

int TDB_db_comparator_compare(void* arg, const char* a, size_t alen,
                              const char* b, size_t blen)
{
    int n = (alen < blen) ? alen : blen;
    TDB_INTENTIONALLY_UNUSED_VARIABLE(arg);

    while(n--) {
        if(*a++ != *b++) {
            return *(unsigned char*)(a - 1) - *(unsigned char*)(b - 1);
        }
    }

    return 0;
}

const char* TDB_db_comparator_name(void* arg)
{
    TDB_INTENTIONALLY_UNUSED_VARIABLE(arg);
    return TDB_DB_COMPARATOR_NAME;
}

int TDB_db_unsafe_get(TDB_db_t* db, \
                      const char* key, \
                      size_t key_len, \
                      char** value, \
                      size_t* value_len)
{
    char* err = NULL;

    *value = leveldb_get(db->handle, db->read_options,
                         key, key_len, value_len, &err);

    if(*value == NULL) {
        LOG(TDB_LOG_ERR, "Failed to get key `%s' from database.", key);

        if(err != NULL) {
            LOG(TDB_LOG_ERR, "LevelDB reports error on get `%s'.", err);
            free(err);
        }

        return TDB_ERROR;
    }

    return TDB_SUCCESS;
}

int TDB_db_unsafe_put(TDB_db_t* db, \
                      const char* key, \
                      size_t key_len, \
                      char* value, \
                      size_t value_len)
{
    char* err = NULL;

    LOG(TDB_LOG_DBG, "Writing to database key `%s'.", key);

    leveldb_put(db->handle, db->write_options,
                key, key_len, value, value_len, &err);

    if(err != NULL) {
        LOG(TDB_LOG_ERR, "LevelDB reports error on put `%s'.", err);
        free(err);

        return TDB_ERROR;
    }

    return TDB_SUCCESS;
}

int TDB_db_unsafe_delete(TDB_db_t* db, \
                         const char* key, \
                         size_t key_len)
{
    char* err = NULL;

    leveldb_delete(db->handle, db->write_options,
                   key, key_len, &err);

    if(err != NULL) {
        LOG(TDB_LOG_ERR, "LevelDB reports error on delete `%s'.", err);
        free(err);

        return TDB_ERROR;
    }

    return TDB_SUCCESS;
}

int TDB_db_unsafe_put_uint64(TDB_db_t* db, const char* key, size_t key_len, uint64_t value)
{
    if(TDB_db_unsafe_put(db, key, key_len, (char*) &value, sizeof(uint64_t)) != TDB_SUCCESS) {
        LOG(TDB_LOG_ERR, "Could not write a uint64 to the database.");
        return TDB_ERROR;
    }

    return TDB_SUCCESS;
}

int TDB_db_unsafe_get_uint64(TDB_db_t* db, const char* key, size_t key_len, uint64_t** value)
{
    size_t value_len;

    if(TDB_db_unsafe_get(db, key, key_len, (char**) value,  &value_len) != TDB_SUCCESS) {
        LOG(TDB_LOG_ERR, "Could not read a uint64 from the database.");
        return TDB_ERROR;
    }

    return TDB_SUCCESS;
}

int TDB_db_unsafe_put_char(TDB_db_t* db, const char* key, size_t key_len, char value)
{
    if(TDB_db_unsafe_put(db, key, key_len, &value, sizeof(char)) != TDB_SUCCESS) {
        LOG(TDB_LOG_ERR, "Could not write a char to the database.");
        return TDB_ERROR;
    }

    return TDB_SUCCESS;
}

int TDB_db_unsafe_get_char(TDB_db_t* db, const char* key, size_t key_len, char** value)
{
    size_t value_len;

    if(TDB_db_unsafe_get(db, key, key_len, value,  &value_len) != TDB_SUCCESS) {
        LOG(TDB_LOG_ERR, "Could not read a char from the database.");
        return TDB_ERROR;
    }

    return TDB_SUCCESS;
}

int TDB_db_unsafe_put_string(TDB_db_t* db, const char* key, size_t key_len, char* value, size_t value_len)
{
    if(TDB_db_unsafe_put(db, key, key_len, (char*) &value, sizeof(char) * value_len) != TDB_SUCCESS) {
        LOG(TDB_LOG_ERR, "Could not write a string to the database.");
        return TDB_ERROR;
    }

    return TDB_SUCCESS;
}

int TDB_db_unsafe_get_string(TDB_db_t* db, const char* key, size_t key_len, char** value, size_t* value_len)
{
    if(TDB_db_unsafe_get(db, key, key_len, value,  value_len) != TDB_SUCCESS) {
        LOG(TDB_LOG_ERR, "Could not read a string from the database.");
        return TDB_ERROR;
    }

    return TDB_SUCCESS;
}

bool TDB_db_validate_key(const char* key)
{
    const char* p = key;

    // FIXME: not designed for malicious intent.

    while(*p++) {
        if(*p == '\0') { break; }

        if((*p < 'a') || (*p > 'z')) {
            if((*p != '_') && (*p != '/')) {
                LOG(TDB_LOG_DBG, "Returning false on %c", *p);
                return false;
            }
        }
    }

    return true;
}

/* EOF */
