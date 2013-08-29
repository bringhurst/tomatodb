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
    leveldb_comparator_t* comparator;

    HVN_db_t* new_db = \
                       (HVN_db_t*) malloc(sizeof(HVN_db_t));

    if(!new_db) {
        LOG(HVN_LOG_ERR, "Could not initialize memory for a new database.");
        return HVN_ERROR;
    }

    comparator = leveldb_comparator_create(NULL,
                                           HVN_db_comparator_destroy,
                                           HVN_db_comparator_compare,
                                           HVN_db_comparator_name);

    new_db->options = leveldb_options_create();

    leveldb_options_set_comparator(new_db->options, comparator);
    leveldb_options_set_create_if_missing(new_db->options, 1);

    new_db->handle = leveldb_open(new_db->options, path, &db_err);

    if(db_err != NULL) {
        LOG(HVN_LOG_ERR, "Could not open the local database. %s", db_err);
        return HVN_ERROR;
    }

    new_db->read_options = leveldb_readoptions_create();
    new_db->write_options = leveldb_writeoptions_create();

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

void HVN_db_comparator_destroy(void* arg)
{
    HVN_INTENTIONALLY_UNUSED_VARIABLE(arg);
    // FIXME: what does this do? Should something be here?
}

int HVN_db_comparator_compare(void* arg, const char* a, size_t alen,
                              const char* b, size_t blen)
{
    int n = (alen < blen) ? alen : blen;
    HVN_INTENTIONALLY_UNUSED_VARIABLE(arg);

    while(n--) {
        if(*a++ != *b++) {
            return *(unsigned char*)(a - 1) - *(unsigned char*)(b - 1);
        }
    }

    return 0;
}

const char* HVN_db_comparator_name(void* arg)
{
    HVN_INTENTIONALLY_UNUSED_VARIABLE(arg);
    return HVN_DB_COMPARATOR_NAME;
}

int HVN_db_unsafe_get(HVN_db_t* db, \
                      const char* key, \
                      size_t key_len, \
                      char** value, \
                      size_t* value_len)
{
    char* err = NULL;

    *value = leveldb_get(db->handle, db->read_options,
                         key, key_len, value_len, &err);

    if(*value == NULL) {
        LOG(HVN_LOG_ERR, "Failed to get key `%s' from database.", key);

        if(err != NULL) {
            LOG(HVN_LOG_ERR, "LevelDB reports error on get `%s'.", err);
            free(err);
        }

        return HVN_ERROR;
    }

    return HVN_SUCCESS;
}

int HVN_db_unsafe_put(HVN_db_t* db, \
                      const char* key, \
                      size_t key_len, \
                      char* value, \
                      size_t value_len)
{
    char* err = NULL;

    LOG(HVN_LOG_DBG, "Writing to database key `%s'.", key);

    leveldb_put(db->handle, db->write_options,
                key, key_len, value, value_len, &err);

    if(err != NULL) {
        LOG(HVN_LOG_ERR, "LevelDB reports error on put `%s'.", err);
        free(err);

        return HVN_ERROR;
    }

    return HVN_SUCCESS;
}

int HVN_db_unsafe_delete(HVN_db_t* db, \
                         const char* key, \
                         size_t key_len)
{
    char* err = NULL;

    leveldb_delete(db->handle, db->write_options,
                   key, key_len, &err);

    if(err != NULL) {
        LOG(HVN_LOG_ERR, "LevelDB reports error on delete `%s'.", err);
        free(err);

        return HVN_ERROR;
    }

    return HVN_SUCCESS;
}

int HVN_db_unsafe_put_uint64(HVN_db_t* db, const char* key, size_t key_len, uint64_t value)
{
    if(HVN_db_unsafe_put(db, key, key_len, (char*) &value, sizeof(uint64_t)) != HVN_SUCCESS) {
        LOG(HVN_LOG_ERR, "Could not write a uint64 to the database.");
        return HVN_ERROR;
    }

    return HVN_SUCCESS;
}

int HVN_db_unsafe_get_uint64(HVN_db_t* db, const char* key, size_t key_len, uint64_t** value)
{
    size_t value_len;

    if(HVN_db_unsafe_get(db, key, key_len, (char**) value,  &value_len) != HVN_SUCCESS) {
        LOG(HVN_LOG_ERR, "Could not read a uint64 from the database.");
        return HVN_ERROR;
    }

    return HVN_SUCCESS;
}

int HVN_db_unsafe_put_char(HVN_db_t* db, const char* key, size_t key_len, char value)
{
    if(HVN_db_unsafe_put(db, key, key_len, &value, sizeof(char)) != HVN_SUCCESS) {
        LOG(HVN_LOG_ERR, "Could not write a char to the database.");
        return HVN_ERROR;
    }

    return HVN_SUCCESS;
}

int HVN_db_unsafe_get_char(HVN_db_t* db, const char* key, size_t key_len, char** value)
{
    size_t value_len;

    if(HVN_db_unsafe_get(db, key, key_len, value,  &value_len) != HVN_SUCCESS) {
        LOG(HVN_LOG_ERR, "Could not read a char from the database.");
        return HVN_ERROR;
    }

    return HVN_SUCCESS;
}

int HVN_db_unsafe_put_string(HVN_db_t* db, const char* key, size_t key_len, char* value, size_t value_len)
{
    if(HVN_db_unsafe_put(db, key, key_len, (char*) &value, sizeof(char) * value_len) != HVN_SUCCESS) {
        LOG(HVN_LOG_ERR, "Could not write a string to the database.");
        return HVN_ERROR;
    }

    return HVN_SUCCESS;
}

int HVN_db_unsafe_get_string(HVN_db_t* db, const char* key, size_t key_len, char** value, size_t* value_len)
{
    if(HVN_db_unsafe_get(db, key, key_len, value,  value_len) != HVN_SUCCESS) {
        LOG(HVN_LOG_ERR, "Could not read a string from the database.");
        return HVN_ERROR;
    }

    return HVN_SUCCESS;
}

bool HVN_db_validate_key(const char* key)
{
    const char* p = key;

    // FIXME: not designed for malicious intent.

    while(*p++) {
        if(*p == '\0') { break; }

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
