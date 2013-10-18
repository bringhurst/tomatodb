#ifndef __TDB__TOMATOD_CONTEXT_H
#define __TDB__TOMATOD_CONTEXT_H

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

#include <uuid/uuid.h>

#include "database.h"
#include "ut/utarray.h"

#define DEFAULT_LISTEN_ADDRESS "127.0.0.1"
#define DEFAULT_LISTEN_PORT    (7854)

typedef struct TDB_ctx_t {
    TDB_db_t* settings_db;
    char* local_state_path;
    char* listen_addr;
    int listen_port;
    int listen_fd;
    struct TDB_replica_t* replicas;
    uuid_t process_uuid;
} TDB_ctx_t;

int TDB_ctx_init(TDB_ctx_t** ctx);
void TDB_ctx_free(TDB_ctx_t* ctx);

#endif /* __TDB__TOMATOD_CONTEXT_H */
