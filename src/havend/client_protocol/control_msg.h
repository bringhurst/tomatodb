#ifndef __HVN__HAVEND_CLIENT_PROTOCOL_CONTROL_MSG_H_
#define __HVN__HAVEND_CLIENT_PROTOCOL_CONTROL_MSG_H_

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
#define HVN_CLNT_PROTO_CTRL_ATTACH       0x01
#define HVN_CLNT_PROTO_CTRL_DESTROY      0x03
#define HVN_CLNT_PROTO_CTRL_EXIT         0x02
#define HVN_CLNT_PROTO_CTRL_FOLLOWER     0x06
#define HVN_CLNT_PROTO_CTRL_LEADER       0x07
#define HVN_CLNT_PROTO_CTRL_LOCATION     0x05
#define HVN_CLNT_PROTO_CTRL_PROXY        0x04
#define HVN_CLNT_PROTO_CTRL_ROUTER       0x0C

/* Server responses. */
#define HVN_CLNT_PROTO_CTRL_R_OK         0x0D
#define HVN_CLNT_PROTO_CTRL_R_ERR        0x1D
#define HVN_CLNT_PROTO_CTRL_R_NOT_FOUND  0x1C

typedef struct HVN_msg_client_control_t {
    uint8_t action;
    unsigned char uuid[16];
} HVN_msg_client_control_t;

typedef struct HVN_msg_client_control_resp_t {
    uint8_t status;
    uint8_t err;
} HVN_msg_client_control_resp_t;

#endif /* __HVN__HAVEND_CLIENT_PROTOCOL_CONTROL_MSG_H_ */
