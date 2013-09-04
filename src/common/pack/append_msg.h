#ifndef __HVN__COMMON_PACK_APPEND_MSG_H_
#define __HVN__COMMON_PACK_APPEND_MSG_H_

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

#include <stdbool.h>
#include <stdint.h>
#include <uuid/uuid.h>

#include "ut/utarray.h"

typedef struct HVN_msg_append_t {
    uint64_t leader_term;
    uint64_t prev_log_index;
    uint64_t prev_log_term;
    uint64_t commit_index;
    uuid_t leader_id;
    UT_array* log_entries;
} HVN_msg_append_t;

typedef struct HVN_msg_append_resp_t {
    uint64_t term;
    bool success;
} HVN_msg_append_resp_t;

int HVN_proto_pack_append_msgpack(HVN_msg_append_t* data, \
                                  size_t* len, \
                                  char** msg);

int HVN_proto_unpack_append_msgpack(HVN_msg_append_t* data, \
                                    size_t len, \
                                    char* msg);

int HVN_proto_pack_append_resp_msgpack(HVN_msg_append_resp_t* data, \
                                       size_t* len, \
                                       char** msg);

int HVN_proto_unpack_append_resp_msgpack(HVN_msg_append_resp_t* data, \
                                         size_t len, \
                                         char* msg);

int HVN_proto_pack_append(HVN_msg_append_t* data, \
                          int scheme, \
                          size_t* len, \
                          char** msg);

int HVN_proto_unpack_append(HVN_msg_append_t* data, \
                            int scheme, \
                            size_t len, \
                            char* msg);

int HVN_proto_pack_append_resp(HVN_msg_append_resp_t* data, \
                               int scheme, \
                               size_t* len, \
                               char** msg);

int HVN_proto_unpack_append_resp(HVN_msg_append_resp_t* data, \
                                 int scheme, \
                                 size_t len, \
                                 char* msg);

int HVN_proto_receive_append_msg(int fd, \
                                 HVN_msg_append_t* append_msg_data);

int HVN_proto_send_append_resp_msg(int fd);

void HVN_proto_print_append_msg(HVN_msg_append_t* data);

#endif /* __HVN__COMMON_PACK_APPEND_MSG_H_ */
