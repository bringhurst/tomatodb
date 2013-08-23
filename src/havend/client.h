#ifndef __HVN__HAVEND_CLIENT_H_
#define __HVN__HAVEND_CLIENT_H_

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

#include <stdio.h>

#define HVN_CLIENT_STACK_SIZE (32768)

typedef struct HVN_client_t {
    char* remote_addr;
    int remote_port;
    int fd;
} HVN_client_t;

void HVN_client_task(HVN_client_t* client);

#endif /* __HVN__HAVEND_CLIENT_H_ */