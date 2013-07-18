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

#include "database.h"

#define HAVEN_CONFIG_DB_PREFIX "/config"
#define HAVEN_LOG_DB_PREFIX    "/log"
#define HAVEN_GROUP_DB_PREFIX  "/group"

typedef struct HAVEN_ctx_t {
    HAVEN_db_t* config_db;
    HAVEN_db_t* log_db;
    char* local_id;
    char* local_state_path;
} HAVEN_ctx_t;

int HAVEN_get_local_machine_id(char** hostname);
void HAVEN_free_context(HAVEN_ctx_t* ctx);

#endif /* __HAVEN_HAVEND_H */
