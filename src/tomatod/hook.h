#ifndef __TDB__TOMATOD_HOOK_H
#define __TDB__TOMATOD_HOOK_H

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

#include "log.h"
#include "task/task.h"

#define TDB_HOOK_STACK_SIZE  32768

#define TDB_HOOK_ALT_NK  3

#define TDB_HOOK_ALT_APPEND_KEY  0
#define TDB_HOOK_ALT_EXIT_KEY    1
#define TDB_HOOK_ALT_VOTE_KEY    2

#define TDB_HOOK_APPEND_CHANNEL_BACKLOG  10
#define TDB_HOOK_VOTE_CHANNEL_BACKLOG  10

typedef struct TDB_hook_t {
    uuid_t remote_uuid;

    int remote_port;
    char* remote_address;

    int fd_append;
    int fd_vote;

    Channel* append_chan;
    Channel* vote_chan;
    Channel* exit_chan;
} TDB_hook_t;

void TDB_hook_task(TDB_hook_t* hook);
int TDB_hook_prepare(char* address, int port, int* fd, uint32_t mode);

int TDB_hook_init(TDB_hook_t** hook, uuid_t uuid, int port, char* address);
void TDB_hook_free(TDB_hook_t* hook);

#endif /* __TDB__TOMATOD_HOOK_H */
