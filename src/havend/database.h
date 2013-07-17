#ifndef __HAVEN_DATABASE_H
#define __HAVEN_DATABASE_H

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

#include <stdio.h>
#include <leveldb/c.h>

#include "common.h"

typedef struct HAVEN_db_t {
    char* path;
    leveldb_t* handle;
    leveldb_options_t* options;
    leveldb_readoptions_t* read_options;
    leveldb_writeoptions_t* write_options;
} HAVEN_db_t;

int HAVEN_init_db(HAVEN_db_t** db, char* path);
void HAVEN_close_db(HAVEN_db_t* db);
int HAVEN_destroy_db(HAVEN_db_t* db);

#endif /* __HAVEN_DATABASE_H */
