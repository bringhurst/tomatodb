#ifndef __HVN_SERVER_H
#define __HVN_SERVER_H

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

#include "havend.h"
#include "database.h"

#include "ut/utarray.h"

#include <stdio.h>
#include <uuid/uuid.h>

#define INITIAL_SERVER_QUEUE_SIZE (100)
#define HVN_SERVER_STACK_SIZE (32768)

typedef struct HVN_server_t {
    struct HVN_ctx_t* ctx;
    HVN_db_t* consensus_db;
    UT_array* consensus_log;
    int listen_fd;
    uuid_t uuid;
} HVN_server_t;

#endif /* __HVN_SERVER_H */
