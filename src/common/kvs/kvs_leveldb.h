#ifndef __TDB__COMMON_DATABASE_H
#define __TDB__COMMON_DATABASE_H

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
#include "xtime.h"

// The primary type of database operation.
#define TDB_DB_VERB_READ     0x01
#define TDB_DB_VERB_WRITE    0x02
#define TDB_DB_VERB_DELETE   0x03
#define TDB_DB_VERB_WATCH    0x04
#define TDB_DB_VERB_UNWATCH  0x05

// Transaction control.
#define TDB_DB_TRANS_ABORT   0x11
#define TDB_DB_TRANS_BEGIN   0x12
#define TDB_DB_TRANS_COMMIT  0x13

// The mode of the primary operation type.
#define TDB_DB_MODE_RW       0x21
#define TDB_DB_MODE_RO       0x22
#define TDB_DB_MODE_RB       0x23
#define TDB_DB_MODE_RT       0x24

#define TDB_DB_COMPARATOR_NAME "TDB_cmp_0001";

typedef struct TDB_db_t {
    char* path;
    leveldb_t* handle;
    leveldb_options_t* options;
    leveldb_readoptions_t* read_options;
    leveldb_writeoptions_t* write_options;
} TDB_db_t;

typedef struct TDB_db_op_t {
    uint16_t action;
    char* key;
    size_t key_len;
    char* value;
    size_t value_len;
} TDB_db_op_t;

int TDB_db_init(TDB_db_t** db, char* path);
void TDB_db_close(TDB_db_t* db);
int TDB_db_destroy(TDB_db_t* db);

int TDB_db_unsafe_get(TDB_db_t* db, \
                      const char* key, \
                      size_t key_len, \
                      char** value, \
                      size_t* value_len);
int TDB_db_unsafe_put(TDB_db_t* db, \
                      const char* key, \
                      size_t key_len, \
                      char* value, \
                      size_t value_len);
int TDB_db_unsafe_delete(TDB_db_t* db, \
                         const char* key, \
                         size_t key_len);

int TDB_db_unsafe_put_uint64(TDB_db_t* db, const char* key, size_t key_len, uint64_t value);
int TDB_db_unsafe_get_uint64(TDB_db_t* db, const char* key, size_t key_len, uint64_t** value);
int TDB_db_unsafe_put_char(TDB_db_t* db, const char* key, size_t key_len, char value);
int TDB_db_unsafe_get_char(TDB_db_t* db, const char* key, size_t key_len, char** value);
int TDB_db_unsafe_put_string(TDB_db_t* db, const char* key, size_t key_len, char* value, size_t value_len);
int TDB_db_unsafe_get_string(TDB_db_t* db, const char* key, size_t key_len, char** value, size_t* value_len);

void TDB_db_comparator_destroy(void* arg);
const char* TDB_db_comparator_name(void* arg);
int TDB_db_comparator_compare(void* arg, const char* a, size_t alen,
                              const char* b, size_t blen);

bool TDB_db_validate_key(const char* key);

#endif /* __TDB__COMMON_DATABASE_H */
