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

#include <stdlib.h>

#include "log.h"
#include "kvs_leveldb.h"

/** The stream to send log messages to. */
extern FILE* TDB_debug_stream;

/** The log level to output. */
extern TDB_loglevel TDB_debug_level;

int TDB_kvs_leveldb_init(TDB_kvs_t* kvs, char* path)
{
    char* db_err = NULL;
    leveldb_comparator_t* comparator;

    kvs->db.leveldb = (TDB_kvs_leveldb_t*) malloc(sizeof(TDB_kvs_leveldb_t));

    if(!kvs->db.leveldb) {
        LOG(TDB_LOG_ERR, "Could not initialize memory for a new database.");
        return TDB_ERROR;
    }

    comparator = leveldb_comparator_create(NULL,
                                           TDB_kvs_leveldb_comparator_destroy,
                                           TDB_kvs_leveldb_comparator_compare,
                                           TDB_kvs_leveldb_comparator_name);

    kvs->db.leveldb->options = leveldb_options_create();

    leveldb_options_set_comparator(kvs->db.leveldb->options, comparator);
    leveldb_options_set_create_if_missing(kvs->db.leveldb->options, 1);

    kvs->db.leveldb->handle = leveldb_open(kvs->db.leveldb->options, path, &db_err);

    if(db_err != NULL) {
        LOG(TDB_LOG_ERR, "Could not open the local database. %s", db_err);
        return TDB_ERROR;
    }

    kvs->db.leveldb->read_options = leveldb_readoptions_create();
    kvs->db.leveldb->write_options = leveldb_writeoptions_create();

    kvs->path = path;

    leveldb_free(db_err);
    return TDB_SUCCESS;
}

void TDB_kvs_leveldb_close(TDB_kvs_t* kvs)
{
    leveldb_close(kvs->db.leveldb->handle);
}

int TDB_kvs_leveldb_destroy(TDB_kvs_t* kvs)
{
    char* db_err = NULL;

    leveldb_destroy_db(kvs->db.leveldb->options, kvs->path, &db_err);

    if(db_err != NULL) {
        LOG(TDB_LOG_ERR, "Could not destroy the database at `%s'. %s", \
            kvs->path, db_err);
        return TDB_ERROR;
    }

    leveldb_free(db_err);
    return TDB_SUCCESS;
}

void TDB_kvs_leveldb_comparator_destroy(void* arg)
{
    TDB_INTENTIONALLY_UNUSED_VARIABLE(arg);
    // FIXME: what does this do? Should something be here?
}

int TDB_kvs_leveldb_comparator_compare(void* arg, const char* a, size_t alen,
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

const char* TDB_kvs_leveldb_comparator_name(void* arg)
{
    TDB_INTENTIONALLY_UNUSED_VARIABLE(arg);
    return TDB_DB_COMPARATOR_NAME;
}

int TDB_kvs_leveldb_get(TDB_kvs_t* kvs, \
                        const char* key, \
                        size_t key_len, \
                        char** value, \
                        size_t* value_len)
{
    char* err = NULL;

    *value = leveldb_get(kvs->db.leveldb->handle, kvs->db.leveldb->read_options,
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

int TDB_kvs_leveldb_put_batch(TDB_kvs_t* kvs, TDB_kvs_batch_t* batch)
{
    TDB_INTENTIONALLY_UNUSED_VARIABLE(kvs);
    TDB_INTENTIONALLY_UNUSED_VARIABLE(batch);
    // TODO: something

    return TDB_ERROR;
}

int TDB_kvs_leveldb_delete(TDB_kvs_t* kvs, \
                           const char* key, \
                           size_t key_len)
{
    char* err = NULL;

    leveldb_delete(kvs->db.leveldb->handle, kvs->db.leveldb->write_options,
                   key, key_len, &err);

    if(err != NULL) {
        LOG(TDB_LOG_ERR, "LevelDB reports error on delete `%s'.", err);
        free(err);

        return TDB_ERROR;
    }

    return TDB_SUCCESS;
}

bool TDB_kvs_leveldb_validate_key(const char* key)
{
    const char* p = key;

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
