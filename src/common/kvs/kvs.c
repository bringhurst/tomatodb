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
#include "kvs.h"
#include "batch.h"

/** The stream to send log messages to. */
extern FILE* TDB_debug_stream;

/** The log level to output. */
extern TDB_loglevel TDB_debug_level;

int TDB_kvs_init(TDB_kvs_t** kvs, char* path, uint32_t type)
{
    *kvs = (TDB_kvs_t*) malloc(sizeof(TDB_kvs_t));

    if(!(*kvs)) {
        LOG(TDB_LOG_ERR, "Could not initialize memory for a new KVS.");
        return TDB_ERROR;
    }

    if(type == KVS_TYPE_LEVELDB) {
        (*kvs)->type = KVS_TYPE_LEVELDB;
        return TDB_kvs_leveldb_init(*kvs, path);
    } else {
        LOG(TDB_LOG_ERR, "Could not initialize memory for a new KVS.");
        return TDB_ERROR;
    }
}

int TDB_kvs_get(TDB_kvs_t* kvs, char* key, size_t key_len, \
                void* result, size_t* result_len)
{
    uint32_t result;

    switch(kvs->storage_type) {
        case KVS_TYPE_LEVELDB:
            result = TDB_kvs_leveldb_get(kvs, key, key_len, result, result_len);
            break;
        default:
            LOG(TDB_LOG_ERR, "Could not determine KVS type when attempting to close.");
            result = TDB_ERROR;
            break;
    }

    return result;
}

int TDB_kvs_put_batch(TDB_kvs_t* kvs, TDB_kvs_batch_t* batch)
{
    int result;

    switch(kvs->storage_type) {
        case KVS_TYPE_LEVELDB:
            result = TDB_kvs_leveldb_put_batch(kvs, batch);
            break;
        default:
            LOG(TDB_LOG_ERR, "Could not determine KVS type when attempting to put multiple items.");
            result = TDB_ERROR;
            break;
    }

    return result;
}

int TDB_kvs_put(TDB_kvs_t* kvs, char* key, size_t key_len, \
                void* value, size_t value_len)
{
    TDB_kvs_batch_t* batch;

    TDB_kvs_create_batch(kvs, &batch);
    TDB_kvs_batch_append(batch, key, key_len, value, value_len);
    TDB_kvs_put_batch(kvs, batch);
}

int TDB_kvs_delete(TDB_kvs_t* kvs, char* key)
{
    int result;

    switch(kvs->storage_type) {
        case KVS_TYPE_LEVELDB:
            result = TDB_kvs_leveldb_delete(kvs, key);
            break;
        default:
            LOG(TDB_LOG_ERR, "Could not determine KVS type when attempting to delete a key.");
            result = TDB_ERROR;
            break;
    }

    return result;
}

void TDB_kvs_close(TDB_kvs_t* kvs)
{
    switch(kvs->storage_type) {
        case KVS_TYPE_LEVELDB:
            TDB_kvs_leveldb_close(kvs);
            break;
        default:
            LOG(TDB_LOG_ERR, "Could not determine KVS type when attempting to close.");
            break;
    }
}

void TDB_kvs_destroy(TDB_kvs_t* kvs)
{
    switch(kvs->storage_type) {
        case KVS_TYPE_LEVELDB:
            TDB_kvs_leveldb_destroy(kvs);
            break;
        default:
            LOG(TDB_LOG_ERR, "Could not determine KVS type when attempting to destroy.");
            break;
    }
}

/* EOF */
