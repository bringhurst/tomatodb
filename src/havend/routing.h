#ifndef __HVN__HAVEND_ROUTING_H_
#define __HVN__HAVEND_ROUTING_H_

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

#define HVN_ROUTER_STACK_SIZE (32768)

typedef struct HVN_router_t {
    HVN_ctx_t* ctx;
    char* remote_addr;
    int remote_port;
    int accept_fd;
} HVN_router_t;

void HVN_routing_task(HVN_router_t* router);

int HVN_init_router(HVN_router_t** router, \
                    HVN_ctx_t* ctx, \
                    char* remote_addr, \
                    int accept_port, \
                    int accept_fd);

void HVN_free_router(HVN_router_t* router);

#endif /* __HVN__HAVEND_ROUTING_H_ */
