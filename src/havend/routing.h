#ifndef __HAVEN__HAVEND_ROUTING_H_
#define __HAVEN__HAVEND_ROUTING_H_

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

#include "havend.h"

#define HAVEN_ROUTER_STACK_SIZE (32768)

typedef struct HAVEN_router_t {
    HAVEN_ctx_t* ctx;
    char* remote_addr;
    int remote_port;
    int accept_fd;
} HAVEN_router_t;

void HAVEN_routing_task(HAVEN_router_t* router);

int HAVEN_init_router(HAVEN_router_t** router, \
                      HAVEN_ctx_t* ctx, \
                      char* remote_addr, \
                      int accept_port, \
                      int accept_fd);
void HAVEN_free_router(HAVEN_router_t* router);

#endif /* __HAVEN__HAVEND_ROUTING_H_ */
