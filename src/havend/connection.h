#ifndef __HAVEN__HAVEND_CONNECTION_H_
#define __HAVEN__HAVEND_CONNECTION_H_

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
#include "server.h"

#include <stdio.h>

#define HAVEN_CONNECTION_STACK_SIZE (32768)

typedef struct HAVEN_connection_t {
    HAVEN_server_t* server;
    char* remote_addr;
    int remote_port;
    int fd;
} HAVEN_connection_t;

void HAVEN_connection_task(HAVEN_connection_t* conn);

#endif /* __HAVEN__HAVEND_CONNECTION_H_ */
