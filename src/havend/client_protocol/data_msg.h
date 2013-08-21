#ifndef __HVN__HAVEND_CLIENT_PROTOCOL_DATA_MSG_H_
#define __HVN__HAVEND_CLIENT_PROTOCOL_DATA_MSG_H_

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
#define HVN_CLNT_PROTO_DATA_READ_WRITE               0x01
#define HVN_CLNT_PROTO_DATA_UNSAFE_READ_WRITE        0x03
#define HVN_CLNT_PROTO_DATA_TRANSACTION_READ_WRITE   0x02
#define HVN_CLNT_PROTO_DATA_TRANSACTION_READ_ONLY    0x06
#define HVN_CLNT_PROTO_DATA_SNAPSHOT_READ_TIMESTAMP  0x07
#define HVN_CLNT_PROTO_DATA_SNAPSHOT_READ_BOUND      0x05

/* Server responses. */
#define HVN_CLNT_PROTO_DATA_R_OK                     0x04
#define HVN_CLNT_PROTO_DATA_R_ERR                    0x0C

/* Operations. */
#define HVN_CLNT_PROTO_DATA_OP_GET                   0x0D
#define HVN_CLNT_PROTO_DATA_OP_PUT                   0x0F
#define HVN_CLNT_PROTO_DATA_OP_DELETE                0x0E

typedef struct HVN_msg_client_data_op_t {
    uint8_t type;
    unsigned char uuid[16];
    uint16_t key_len;
    uint16_t value_len;
    unsigned char payload[];
} HVN_msg_client_data_op_t;

typedef struct HVN_msg_client_data_t {
    uint8_t mode;
    uint16_t op_count;
    HVN_msg_client_data_op_t ops[];
} HVN_msg_client_data_t;

typedef struct HVN_msg_client_data_resp_t {
    uint8_t status;
    HVN_msg_client_data_op_t results[];
} HVN_msg_client_data_resp_t;

#endif /* __HVN__HAVEND_CLIENT_PROTOCOL_DATA_MSG_H_ */
