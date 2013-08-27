#ifndef __HVN_DATABASE_H
#define __HVN_DATABASE_H

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

typedef struct HVN_db_t {
    char* path;
    leveldb_t* handle;
    leveldb_options_t* options;
    leveldb_readoptions_t* read_options;
    leveldb_writeoptions_t* write_options;
} HVN_db_t;

typedef struct HVN_db_op_t {
    uint16_t action;
    uint16_t mode;
    HVN_time_interval_t time_stamp;
    char* key;
    size_t key_len;
    char* value;
    size_t value_len;
} HVN_db_op_t;

int HVN_db_init(HVN_db_t** db, char* path);
void HVN_db_close(HVN_db_t* db);
int HVN_db_destroy(HVN_db_t* db);

bool HVN_db_validate_key(char* key);

#endif /* __HVN_DATABASE_H */
