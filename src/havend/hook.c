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
#include "pack/connect_msg.h"
#include "pack/control_msg.h"
#include "pack/vote_msg.h"

#include "hook.h"

/** The stream to send log messages to. */
extern FILE* HVN_debug_stream;

/** The log level to output. */
extern HVN_loglevel HVN_debug_level;

void HVN_hook_task(HVN_hook_t* hook)
{
    static Alt alts[HVN_HOOK_ALT_NK + 1];

    HVN_msg_append_t append_msg;
    HVN_msg_vote_t vote_msg;

    uint32_t exit_result;

    taskname("hook");
    taskstate("new");

    if(HVN_hook_prepare(hook->remote_address, hook->remote_port, \
            &(hook->fd_append), HVN_PROTO_CTRL_ATTACH_APPEND) != HVN_SUCCESS) {
        LOG(HVN_LOG_ERR, "Could not create an append hook connection.");
        taskexit(EXIT_FAILURE);
    }

    if(HVN_hook_prepare(hook->remote_address, hook->remote_port, \
            &(hook->fd_vote), HVN_PROTO_CTRL_ATTACH_VOTE) != HVN_SUCCESS) {
        LOG(HVN_LOG_ERR, "Could not create an vote hook connection.");
        taskexit(EXIT_FAILURE);
    }

    alts[HVN_HOOK_ALT_APPEND_KEY].c = hook->append_chan;
    alts[HVN_HOOK_ALT_APPEND_KEY].v = &append_msg;
    alts[HVN_HOOK_ALT_APPEND_KEY].op = CHANRCV;

    alts[HVN_HOOK_ALT_VOTE_KEY].c = hook->vote_chan;
    alts[HVN_HOOK_ALT_VOTE_KEY].v = &vote_msg;
    alts[HVN_HOOK_ALT_VOTE_KEY].op = CHANRCV;

    alts[HVN_HOOK_ALT_EXIT_KEY].c = hook->exit_chan;
    alts[HVN_HOOK_ALT_EXIT_KEY].v = &exit_result;
    alts[HVN_HOOK_ALT_EXIT_KEY].op = CHANRCV;

    for(;;) {
        taskstate("idle");
        switch(chanalt(alts)) {
            case HVN_HOOK_ALT_APPEND_KEY:
                taskstate("write/append");
                LOG(HVN_LOG_DBG, "A replica sent this hook an append message.");
                if(HVN_proto_send_append_msg(*fd) != HVN_SUCCESS) {
                    LOG(HVN_LOG_ERR, "Failed to send a hook append message.");
                    return HVN_ERROR;
                }
                break;

            case HVN_HOOK_ALT_VOTE_KEY:
                taskstate("write/vote");
                LOG(HVN_LOG_DBG, "A replica sent this hook a vote message.");
                if(HVN_proto_send_vote_msg(*fd) != HVN_SUCCESS) {
                    LOG(HVN_LOG_ERR, "Failed to send a hook vote message.");
                    return HVN_ERROR;
                }
                break;

            case HVN_HOOK_ALT_EXIT_KEY:
                LOG(HVN_LOG_DBG, "A replica sent this hook an exit message.");
                taskexit(EXIT_SUCCESS);

            default:
                LOG(HVN_LOG_ERR, "A replica sent a hook an unknown message.");
                taskexit(EXIT_FAILURE);
        }
    }
}

int HVN_hook_prepare(char* address, int port, int* fd, uint32_t mode)
{
    *fd = netdial(TCP, address, port);
    if(*fd < 1) {
        LOG(HVN_LOG_ERR, "Failed to open a hook socket.");
        return HVN_ERROR;
    }

    if(HVN_proto_send_connect_msg(*fd) != HVN_SUCCESS) {
        LOG(HVN_LOG_ERR, "Failed to send a hook connect message.");
        return HVN_ERROR;
    }

    if(HVN_proto_send_control_msg(*fd, mode) != HVN_SUCCESS) {
        LOG(HVN_LOG_ERR, "Failed to send a hook connect message.");
        return HVN_ERROR;
    }

    return HVN_SUCCESS;
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
