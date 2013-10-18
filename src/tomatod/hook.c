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
extern FILE* TDB_debug_stream;

/** The log level to output. */
extern TDB_loglevel TDB_debug_level;

void TDB_hook_task(TDB_hook_t* hook)
{
    static Alt alts[TDB_HOOK_ALT_NK + 1];

    TDB_msg_append_t append_msg;
    TDB_msg_vote_t vote_msg;

    uint32_t exit_result;

    taskname("hook");
    taskstate("new");

    if(TDB_hook_prepare(hook->remote_address, hook->remote_port, \
            &(hook->fd_append), TDB_PROTO_CTRL_ATTACH_APPEND) != TDB_SUCCESS) {
        LOG(TDB_LOG_ERR, "Could not create an append hook connection.");
        taskexit(EXIT_FAILURE);
    }

    if(TDB_hook_prepare(hook->remote_address, hook->remote_port, \
            &(hook->fd_vote), TDB_PROTO_CTRL_ATTACH_VOTE) != TDB_SUCCESS) {
        LOG(TDB_LOG_ERR, "Could not create an vote hook connection.");
        taskexit(EXIT_FAILURE);
    }

    alts[TDB_HOOK_ALT_APPEND_KEY].c = hook->append_chan;
    alts[TDB_HOOK_ALT_APPEND_KEY].v = &append_msg;
    alts[TDB_HOOK_ALT_APPEND_KEY].op = CHANRCV;

    alts[TDB_HOOK_ALT_VOTE_KEY].c = hook->vote_chan;
    alts[TDB_HOOK_ALT_VOTE_KEY].v = &vote_msg;
    alts[TDB_HOOK_ALT_VOTE_KEY].op = CHANRCV;

    alts[TDB_HOOK_ALT_EXIT_KEY].c = hook->exit_chan;
    alts[TDB_HOOK_ALT_EXIT_KEY].v = &exit_result;
    alts[TDB_HOOK_ALT_EXIT_KEY].op = CHANRCV;

    for(;;) {
        taskstate("idle");
        switch(chanalt(alts)) {
            case TDB_HOOK_ALT_APPEND_KEY:
                taskstate("write/append");
                LOG(TDB_LOG_DBG, "A replica sent this hook an append message.");
                if(TDB_proto_send_append_msg(hook->fd_append, NULL /* TODO */) != TDB_SUCCESS) {
                    LOG(TDB_LOG_ERR, "Failed to send a hook append message.");
                    taskexit(EXIT_FAILURE);
                }
                break;

            case TDB_HOOK_ALT_VOTE_KEY:
                taskstate("write/vote");
                LOG(TDB_LOG_DBG, "A replica sent this hook a vote message.");
                if(TDB_proto_send_vote_msg(hook->fd_vote, NULL /* TODO */) != TDB_SUCCESS) {
                    LOG(TDB_LOG_ERR, "Failed to send a hook vote message.");
                    taskexit(EXIT_FAILURE);
                }
                break;

            case TDB_HOOK_ALT_EXIT_KEY:
                LOG(TDB_LOG_DBG, "A replica sent this hook an exit message.");
                taskexit(EXIT_SUCCESS);

            default:
                LOG(TDB_LOG_ERR, "A replica sent a hook an unknown message.");
                taskexit(EXIT_FAILURE);
        }
    }
}

int TDB_hook_prepare(char* address, int port, int* fd, uint32_t mode)
{
    *fd = netdial(TCP, address, port);
    if(*fd < 1) {
        LOG(TDB_LOG_ERR, "Failed to open a hook socket.");
        return TDB_ERROR;
    }

    if(TDB_proto_send_connect_msg(*fd) != TDB_SUCCESS) {
        LOG(TDB_LOG_ERR, "Failed to send a hook connect message.");
        return TDB_ERROR;
    }

    if(TDB_proto_send_control_msg(*fd /* TODO: , mode */) != TDB_SUCCESS) {
        LOG(TDB_LOG_ERR, "Failed to send a hook connect message.");
        return TDB_ERROR;
    }

    return TDB_SUCCESS;
}

int TDB_hook_init(TDB_hook_t** hook, uuid_t uuid, int port, char* address)
{
    *hook = (TDB_hook_t*) malloc(sizeof(TDB_hook_t));

    if(*hook == NULL) {
        LOG(TDB_LOG_ERR, "Could not allocate memory for a replica hook.");
        return TDB_ERROR;
    }

    (*hook)->remote_address = (char*) malloc(sizeof(char) * _POSIX_HOST_NAME_MAX);

    if((*hook)->remote_address == NULL) {
        LOG(TDB_LOG_ERR, "Could not allocate memory for a replica hook address.");
        return TDB_ERROR;
    }

    (*hook)->remote_port = port;
    strncpy((*hook)->remote_address, address, _POSIX_HOST_NAME_MAX);

    // FIXME: allocate mem for remote_uuid?
    uuid_copy((*hook)->remote_uuid, uuid);

    (*hook)->fd_append = -1;
    (*hook)->fd_vote = -1;

    (*hook)->append_chan = chancreate(sizeof(TDB_msg_append_t*), TDB_HOOK_APPEND_CHANNEL_BACKLOG);
    (*hook)->vote_chan = chancreate(sizeof(TDB_msg_vote_t*), TDB_HOOK_VOTE_CHANNEL_BACKLOG);
    (*hook)->exit_chan = chancreate(sizeof(uint32_t), 0);

    taskcreate((void (*)(void*)) TDB_hook_task, (void*) (*hook), TDB_HOOK_STACK_SIZE);
    return TDB_SUCCESS;
}

void TDB_hook_free(TDB_hook_t* hook)
{
    free(hook->remote_address);
    chanfree(hook->append_chan);
    chanfree(hook->vote_chan);
    chanfree(hook->exit_chan);
    free(hook);
}

/* EOF */
