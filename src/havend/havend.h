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

#include "database.h"
#include "xarray.h"

#include <stdlib.h>
#include <uuid/uuid.h>

#define HAVEN_SETTINGS_DB_PREFIX "/settings"
#define HAVEN_LOG_DB_PREFIX    "/log"
#define HAVEN_GROUP_DB_PREFIX  "/group"

typedef struct HAVEN_ctx_t {
    HAVEN_db_t* log_db;
    HAVEN_db_t* settings_db;
    HAVEN_xarray_t* server_queue;
    char* local_state_path;
    char* listen_addr;
    int listen_port;
    int listen_fd;
    uuid_t local_uuid;
} HAVEN_ctx_t;

void HAVEN_free_context(HAVEN_ctx_t* ctx);
void HAVEN_print_version(void);
void HAVEN_print_usage(char** argv);
int HAVEN_handle_havend_cli_args(HAVEN_ctx_t* ctx, int argc, char* argv[]);

#endif /* __HAVEN_HAVEND_H */
