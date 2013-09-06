#ifndef __HVN__COMMON_PACK_VOTE_MSG_H_
#define __HVN__COMMON_PACK_VOTE_MSG_H_

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

typedef struct HVN_msg_vote_t {
    uint64_t candidate_term;
    uint64_t last_log_index;
    uint64_t last_log_term;
    uuid_t candidate_id;
} HVN_msg_vote_t;

typedef struct HVN_msg_vote_resp_t {
    uint64_t term;
    bool vote_granted;
} HVN_msg_vote_resp_t;

int HVN_proto_pack_vote_msgpack(HVN_msg_vote_t* data, \
                                size_t* len, \
                                char** msg);

int HVN_proto_unpack_vote_msgpack(HVN_msg_vote_t* data, \
                                  size_t len, \
                                  char* msg);

int HVN_proto_pack_vote_resp_msgpack(HVN_msg_vote_resp_t* data, \
                                     size_t* len, \
                                     char** msg);

int HVN_proto_unpack_vote_resp_msgpack(HVN_msg_vote_resp_t* data, \
                                       size_t len, \
                                       char* msg);

int HVN_proto_pack_vote(HVN_msg_vote_t* data, \
                        int scheme, \
                        size_t* len, \
                        char** msg);

int HVN_proto_unpack_vote(HVN_msg_vote_t* data, \
                          int scheme, \
                          size_t len, \
                          char* msg);

int HVN_proto_pack_vote_resp(HVN_msg_vote_resp_t* data, \
                             int scheme, \
                             size_t* len, \
                             char** msg);

int HVN_proto_unpack_vote_resp(HVN_msg_vote_resp_t* data, \
                               int scheme, \
                               size_t len, \
                               char* msg);

int HVN_proto_receive_vote_msg(int fd, \
                               HVN_msg_vote_t* vote_msg_data);

int HVN_proto_send_vote_resp_msg(int fd, HVN_msg_vote_resp_t* resp);

void HVN_proto_print_vote_msg(HVN_msg_vote_t* data);

#endif /* __HVN__COMMON_PACK_VOTE_MSG_H_ */
