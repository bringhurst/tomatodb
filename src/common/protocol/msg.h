#ifndef __HVN__COMMON_PROTOCOL_MSG_H_
#define __HVN__COMMON_PROTOCOL_MSG_H_

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

/*
 *  General protocol definitions.
 */

/* Overall protocol version. */
#define HVN_MSG_VERSION           0xCCCC0001
#define HVN_MSG_MAGIC             0xDECAFBAD

/* Client request msg types. */
#define HVN_MSG_TYPE_BOOTSTRAP    0x01
#define HVN_MSG_TYPE_CONSENSUS    0x03
#define HVN_MSG_TYPE_CONTROL      0x05
#define HVN_MSG_TYPE_DATA         0x07

/* Server response msg types. */
#define HVN_MSG_TYPE_BOOTSTRAP_R  0x02
#define HVN_MSG_TYPE_CONSENSUS_R  0x04
#define HVN_MSG_TYPE_CONTROL_R    0x06
#define HVN_MSG_TYPE_DATA_R       0x08

/*
 * Bootstrap msg definition.
 */

typedef struct HVN_msg_bootstrap_t {
} HVN_msg_bootstrap_t;

typedef struct HVN_msg_resp_bootstrap_t {
} HVN_msg_resp_bootstrap_t;

int HVN_msg_bootstrap_send(int fd, HVN_msg_bootstrap_t* msg_in);
int HVN_msg_bootstrap_recv(HVN_msg_bootstrap_t** msg_out, int fd);

int HVN_msg_resp_bootstrap_send(int fd, HVN_msg_resp_bootstrap_t* msg_in);
int HVN_msg_resp_bootstrap_recv(HVN_msg_bootstrap_t** msg_out, int fd);

/*
 * Consensus msg definitions.
 */

typedef struct HVN_msg_consensus_t {
    uint64_t term;
    uuid_t id;
    uint64_t log_index;
    uint64_t log_term;
    uint64_t commit_index;
    UT_array* log_entries;
} HVN_msg_consensus_t;

typedef struct HVN_msg_resp_consensus_t {
    uint64_t term;
    bool success;
} HVN_msg_resp_consensus_t;

int HVN_msg_consensus_send(int fd, HVN_msg_consensus_t* msg_in);
int HVN_msg_consensus_recv(HVN_msg_consensus_t** msg_out, int fd);

int HVN_msg_resp_consensus_send(int fd, HVN_msg_resp_consensus_t* msg_in);
int HVN_msg_resp_consensus_recv(HVN_msg_consensus_t** msg_out, int fd);

/*
 * Control msg definition.
 */

typedef struct HVN_msg_control_t {
} HVN_msg_control_t;

typedef struct HVN_msg_resp_control_t {
} HVN_msg_resp_control_t;

int HVN_msg_control_send(int fd, HVN_msg_control_t* msg_in);
int HVN_msg_control_recv(HVN_msg_control_t** msg_out, int fd);

int HVN_msg_resp_control_send(int fd, HVN_msg_resp_control_t* msg_in);
int HVN_msg_resp_control_recv(HVN_msg_control_t** msg_out, int fd);

/*
 * Data msg definition.
 */

typedef struct HVN_msg_data_t {
} HVN_msg_data_t;

typedef struct HVN_msg_resp_data_t {
} HVN_msg_resp_data_t;

int HVN_msg_data_send(int fd, HVN_msg_data_t* msg_in);
int HVN_msg_data_recv(HVN_msg_data_t** msg_out, int fd);

int HVN_msg_resp_data_send(int fd, HVN_msg_resp_data_t* msg_in);
int HVN_msg_resp_data_recv(HVN_msg_data_t** msg_out, int fd);

#endif /* __HVN__COMMON_PROTOCOL_MSG_H_ */
