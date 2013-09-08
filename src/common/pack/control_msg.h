#ifndef __HVN__COMMON_PACK_CONTROL_MSG_H_
#define __HVN__COMMON_PACK_CONTROL_MSG_H_

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

/* Client requests. */
#define HVN_PROTO_CTRL_ATTACH_APPEND  0x01
#define HVN_PROTO_CTRL_ATTACH_DATA    0x03
#define HVN_PROTO_CTRL_ATTACH_VOTE    0x02
#define HVN_PROTO_CTRL_DESTROY        0x06
#define HVN_PROTO_CTRL_EXIT           0x07
#define HVN_PROTO_CTRL_FOLLOWER       0x05
#define HVN_PROTO_CTRL_LEADER         0x04
#define HVN_PROTO_CTRL_LOCATION       0x0C
#define HVN_PROTO_CTRL_PROXY          0x0D
#define HVN_PROTO_CTRL_ROUTER         0x0F

/* Server responses. */
#define HVN_PROTO_CTRL_R_OK           0x0E
#define HVN_PROTO_CTRL_R_ERR          0x1E
#define HVN_PROTO_CTRL_R_NOT_FOUND    0x1F

typedef struct HVN_msg_client_control_t {
    uint16_t action;
    unsigned char uuid[16];
    uint16_t path_len;
    char* path;
} HVN_msg_client_control_t;

typedef struct HVN_msg_client_control_resp_t {
    uint8_t success;
    uint8_t err_code;
} HVN_msg_client_control_resp_t;

int HVN_proto_pack_control_msgpack(HVN_msg_client_control_t* data, \
                                   size_t* len, \
                                   char** msg);

int HVN_proto_unpack_control_msgpack(HVN_msg_client_control_t* data, \
                                     size_t len, \
                                     char* msg);

int HVN_proto_pack_control_resp_msgpack(HVN_msg_client_control_resp_t* data, \
                                        size_t* len, \
                                        char** msg);

int HVN_proto_unpack_control_resp_msgpack(HVN_msg_client_control_resp_t* data, \
                                          size_t len, \
                                          char* msg);

int HVN_proto_pack_control(HVN_msg_client_control_t* data, \
                           int scheme, \
                           size_t* len, \
                           char** msg);

int HVN_proto_unpack_control(HVN_msg_client_control_t* data, \
                             int scheme, \
                             size_t len, \
                             char* msg);

int HVN_proto_pack_control_resp(HVN_msg_client_control_resp_t* data, \
                                int scheme, \
                                size_t* len, \
                                char** msg);

int HVN_proto_unpack_control_resp(HVN_msg_client_control_resp_t* data, \
                                  int scheme, \
                                  size_t len, \
                                  char* msg);

int HVN_proto_receive_control_msg(int fd, \
                                  HVN_msg_client_control_t* control_msg_data);

int HVN_proto_send_control_msg(int fd);

int HVN_proto_send_control_resp_msg(int fd);

#endif /* __HVN__COMMON_PACK_CONTROL_MSG_H_ */
