#ifndef __TDB__TOMATOD_ATTACH_H_
#define __TDB__TOMATOD_ATTACH_H_

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

#define TDB_ATTACH_STACK_SIZE       32768
#define TDB_ATTACH_CHANNEL_BACKLOG  10

#define TDB_ATTACH_SEND_ALT_NK  4

#define TDB_ATTACH_SEND_ALT_APPEND_KEY  0
#define TDB_ATTACH_SEND_ALT_DATA_KEY    1
#define TDB_ATTACH_SEND_ALT_EXIT_KEY    2
#define TDB_ATTACH_SEND_ALT_VOTE_KEY    3

typedef struct TDB_attach_t {
    TDB_replica_t* replica;
    Channel* append_reply_chan;
    Channel* data_reply_chan;
    Channel* vote_reply_chan;
    Channel* exit_chan;
    char* remote_addr;
    int remote_port;
    uint32_t mode;
    int fd;
} TDB_attach_t;

typedef struct TDB_attach_msg_t {
    Channel* append_reply_chan;
    Channel* data_reply_chan;
    Channel* vote_reply_chan;
    void* msg;
} TDB_attach_msg_t;

void TDB_attach_task(TDB_attach_t* client);
void TDB_attach_recv(TDB_attach_t* client);
void TDB_attach_send_task(TDB_attach_t* client);

void TDB_attach_recv_append(TDB_attach_t* client);
void TDB_attach_recv_vote(TDB_attach_t* client);
void TDB_attach_recv_data(TDB_attach_t* client);

int TDB_replica_attach(TDB_router_t* router, \
                       uuid_t uuid, \
                       uint32_t mode);
int TDB_attach_init(TDB_attach_t** client, \
                    TDB_router_t* router, \
                    TDB_replica_t* replica, \
                    uint32_t mode);
void TDB_attach_free(TDB_attach_t* client);

#endif /* __TDB__TOMATOD_ATTACH_H_ */
