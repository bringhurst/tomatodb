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

#include "common.h"

#define TDB_DB_COMPARATOR_NAME "TDB_cmp_0001";

typedef struct TDB_kvs_leveldb_t {
    char* path;
    leveldb_t* handle;
    leveldb_options_t* options;
    leveldb_readoptions_t* read_options;
    leveldb_writeoptions_t* write_options;
} TDB_db_t;

#endif /* TDB__COMMON_KVS_KVS_LEVELDB_H */
