#ifndef __HVN__COMMON_PACK_ADDR_STRUCT_H_
#define __HVN__COMMON_PACK_ADDR_STRUCT_H_

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

#include <stdint.h>
#include <uuid/uuid.h>

typedef struct HVN_addr_t {
    char* address;
    int port;
    uuid_t uuid;
} HVN_addr_t;

int HVN_pack_addr_msgpack(HVN_addr_t* data, \
                          size_t* len, \
                          char** msg);

int HVN_unpack_addr_msgpack(HVN_addr_t* data, \
                            size_t len, \
                            char* msg);

int HVN_pack_addr(HVN_addr_t* data, \
                  int scheme, \
                  size_t* len, \
                  char** msg);

int HVN_unpack_addr(HVN_addr_t* data, \
                    int scheme, \
                    size_t len, \
                    char* msg);

#endif /* __HVN__COMMON_PACK_ADDR_STRUCT_H_ */
