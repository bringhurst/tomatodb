#ifndef TDB__COMMON_KVS_KVS_LEVELDB_H
#define TDB__COMMON_KVS_KVS_LEVELDB_H

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

#include <stdbool.h>
#include <stdio.h>

#include <leveldb/c.h>

#include "kvs.h"
#include "common.h"
#include "batch.h"

#define TDB_DB_COMPARATOR_NAME "TDB_cmp_0001";

typedef struct TDB_kvs_leveldb_t {
    leveldb_t* handle;
    leveldb_options_t* options;
    leveldb_readoptions_t* read_options;
    leveldb_writeoptions_t* write_options;
} TDB_kvs_leveldb_t;

int TDB_kvs_leveldb_init(TDB_kvs_t* kvs, char* path);

void TDB_kvs_leveldb_close(TDB_kvs_t* kvs);
int TDB_kvs_leveldb_destroy(TDB_kvs_t* kvs);

void TDB_kvs_leveldb_comparator_destroy(void* arg);
int TDB_kvs_leveldb_comparator_compare(void* arg, const char* a, size_t alen, \
                              const char* b, size_t blen);
const char* TDB_kvs_leveldb_comparator_name(void* arg);

int TDB_kvs_leveldb_get(TDB_kvs_t* kvs, \
                        const char* key, \
                        size_t key_len, \
                        char** value, \
                        size_t* value_len);

int TDB_kvs_leveldb_put_batch(TDB_kvs_t* kvs, TDB_kvs_batch_t* batch);

int TDB_kvs_leveldb_delete(TDB_kvs_t* kvs, \
                           const char* key, \
                           size_t key_len);

bool TDB_kvs_leveldb_validate_key(const char* key);

#endif /* TDB__COMMON_KVS_KVS_LEVELDB_H */
