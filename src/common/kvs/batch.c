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

#include "log.h"
#include "batch.h"

/** The stream to send log messages to. */
extern FILE* TDB_debug_stream;

/** The log level to output. */
extern TDB_loglevel TDB_debug_level;

int TDB_kvs_batch_init(TDB_kvs_batch_t** batch)
{
    *kvs = (TDB_kvs_t*) malloc(sizeof(TDB_kvs_t));

    if(!(*kvs)) {
        LOG(TDB_LOG_ERR, "Could not initialize memory for a new KVS.");
        return TDB_ERROR;
    }

    if(storage_type == KVS_TYPE_LEVELDB) {
        (*kvs)->storage_type = KVS_TYPE_LEVELDB;
        return TDB_kvs_leveldb_init(*kvs, path);
    } else {
        LOG(TDB_LOG_ERR, "Could not initialize memory for a new KVS.");
        return TDB_ERROR;
    }
}

void TDB_kvs_batch_free(TDB_kvs_batch_t* batch)
{
    // TODO: free utarray, then batch
}

int TDB_kvs_batch_append(TDB_kvs_batch_t* kvs, \
                         char* key, size_t key_len, \
                         void* value, size_t* value_len)
{
    // TODO: append to utarray
}

/* EOF */
