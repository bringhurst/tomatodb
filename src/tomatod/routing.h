#ifndef __TDB__TOMATOD_ROUTING_H_
#define __TDB__TOMATOD_ROUTING_H_

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

#include "tomatod.h"

#define TDB_ROUTER_STACK_SIZE (32768)

typedef struct TDB_router_t {
    TDB_ctx_t* ctx;
    char* remote_addr;
    int remote_port;
    int accept_fd;
} TDB_router_t;

void TDB_routing_task(TDB_router_t* router);

int TDB_init_router(TDB_router_t** router, \
                    TDB_ctx_t* ctx, \
                    char* remote_addr, \
                    int accept_port, \
                    int accept_fd);

void TDB_free_router(TDB_router_t* router);

#endif /* __TDB__TOMATOD_ROUTING_H_ */
