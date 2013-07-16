#ifndef __HAVEN_HAVEND_H
#define __HAVEN_HAVEND_H

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
 */

#include <stdlib.h>
#include <leveldb/c.h>

#define HAVEN_DEFAULT_STATE_DIRECTORY "/var/lib/haven";

typedef struct HAVEN_server_internal_database_t {
    char* id;
    leveldb_t* db;
    leveldb_options_t* options;
    leveldb_readoptions_t* read_options;
    leveldb_writeoptions_t* write_options;
} HAVEN_server_internal_database_t;

typedef struct HAVEN_server_context_t {
    HAVEN_server_internal_database_t* internal_database;
    char* local_id;
} HAVEN_server_context_t;

int HAVEN_get_local_machine_id(char** hostname);
void HAVEN_free_context(HAVEN_server_context_t* ctx);

#endif /* __HAVEN_HAVEND_H */
