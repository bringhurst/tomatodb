#ifndef __HAVEN__HAVEND_HAVEND_H_
#define __HAVEN__HAVEND_HAVEND_H_

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

#include "server.h"
#include "database.h"

#include <stdlib.h>
#include <uuid/uuid.h>
#include <msgpack.h>

#define HAVEN_SETTINGS_DB_PREFIX "/settings"
#define HAVEN_LOG_DB_PREFIX    "/log"
#define HAVEN_GROUP_DB_PREFIX  "/group"

#define DEFAULT_LISTEN_ADDRESS "127.0.0.1"
#define DEFAULT_LISTEN_PORT    (7854)

typedef struct HAVEN_ctx_t {
    HAVEN_db_t* settings_db;
    struct HAVEN_server_t* server_routes;
    char* local_state_path;
    char* listen_addr;
    int listen_port;
    int listen_fd;
    uuid_t process_uuid;
    msgpack_zone* mp_mempool;
} HAVEN_ctx_t;

int HAVEN_context_init(HAVEN_ctx_t** ctx);
void HAVEN_context_free(HAVEN_ctx_t* ctx);
void HAVEN_print_version(void);
void HAVEN_print_usage(void);
int HAVEN_handle_havend_cli_args(HAVEN_ctx_t* ctx, int argc, char* argv[]);

#endif /* __HAVEN__HAVEND_HAVEND_H_ */
