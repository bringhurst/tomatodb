#ifndef __HVN__HAVEND_ATTACH_H_
#define __HVN__HAVEND_ATTACH_H_

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

#include <stdio.h>

#include "replica.h"
#include "routing.h"

#define HVN_ATTACH_STACK_SIZE       32768
#define HVN_ATTACH_CHANNEL_BACKLOG  10

#define HVN_ATTACH_MODE_DATA    0x01
#define HVN_ATTACH_MODE_VOTE    0x03
#define HVN_ATTACH_MODE_APPEND  0x02

#define HVN_ATTACH_SEND_ALT_NK  4

#define HVN_ATTACH_SEND_ALT_APPEND_KEY  0
#define HVN_ATTACH_SEND_ALT_DATA_KEY    1
#define HVN_ATTACH_SEND_ALT_EXIT_KEY    2
#define HVN_ATTACH_SEND_ALT_VOTE_KEY    3

typedef struct HVN_attach_t {
    HVN_replica_t* replica;
    Channel* append_reply_chan;
    Channel* data_reply_chan;
    Channel* vote_reply_chan;
    Channel* exit_chan;
    char* remote_addr;
    int remote_port;
    int mode;
    int fd;
} HVN_attach_t;

typedef struct HVN_attach_msg_t {
    Channel* append_reply_chan;
    Channel* data_reply_chan;
    Channel* vote_reply_chan;
    void* msg;
} HVN_attach_msg_t;

void HVN_attach_task(HVN_attach_t* client);
void HVN_attach_recv(HVN_attach_t* client);
void HVN_attach_send_task(HVN_attach_t* client);

void HVN_attach_append(HVN_attach_t* client);
void HVN_attach_vote(HVN_attach_t* client);
void HVN_attach_data(HVN_attach_t* client);

int HVN_replica_attach(HVN_router_t* router, uuid_t uuid);
int HVN_attach_init(HVN_attach_t** client, HVN_router_t* router, HVN_replica_t* replica);
void HVN_attach_free(HVN_attach_t* client);

#endif /* __HVN__HAVEND_ATTACH_H_ */
