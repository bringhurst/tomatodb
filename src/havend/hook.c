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

#include <stdlib.h>

#include "common.h"
#include "pack/append_msg.h"
#include "pack/vote_msg.h"

#include "hook.h"

/** The stream to send log messages to. */
extern FILE* HVN_debug_stream;

/** The log level to output. */
extern HVN_loglevel HVN_debug_level;

void HVN_hook_task(HVN_hook_t* hook)
{
    HVN_INTENTIONALLY_UNUSED_VARIABLE(hook);

    // TODO: create two sockets to the remote replica, one for vote and for append.
    // TODO: create two channels that allow the replica to send to the two sockets.
}

int HVN_hook_init(HVN_hook_t** hook, uuid_t uuid, int port, char* address)
{
    *hook = (HVN_hook_t*) malloc(sizeof(HVN_hook_t));

    if(*hook == NULL) {
        LOG(HVN_LOG_ERR, "Could not allocate memory for a replica hook.");
        return HVN_ERROR;
    }

    (*hook)->remote_address = (char*) malloc(sizeof(char) * _POSIX_HOST_NAME_MAX);

    if((*hook)->remote_address == NULL) {
        LOG(HVN_LOG_ERR, "Could not allocate memory for a replica hook address.");
        return HVN_ERROR;
    }

    (*hook)->remote_port = port;
    strncpy((*hook)->remote_address, address, _POSIX_HOST_NAME_MAX);

    // FIXME: allocate mem for remote_uuid?
    uuid_copy((*hook)->remote_uuid, uuid);

    (*hook)->fd_append = -1;
    (*hook)->fd_vote = -1;

    (*hook)->append_chan = chancreate(sizeof(HVN_msg_append_t*), HVN_HOOK_APPEND_CHANNEL_BACKLOG);
    (*hook)->vote_chan = chancreate(sizeof(HVN_msg_vote_t*), HVN_HOOK_VOTE_CHANNEL_BACKLOG);
    (*hook)->exit_chan = chancreate(sizeof(uint32_t), 0);

    taskcreate((void (*)(void*)) HVN_hook_task, (void*) (*hook), HVN_HOOK_STACK_SIZE);
    return HVN_SUCCESS;
}

void HVN_hook_free(HVN_hook_t* hook)
{
    free(hook->remote_address);
    chanfree(hook->append_chan);
    chanfree(hook->vote_chan);
    chanfree(hook->exit_chan);
    free(hook);
}

/* EOF */
