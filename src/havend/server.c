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
 */

#include "server.h"
#include "log.h"
#include "ut/utarray.h"
#include "bootstrap.h"
#include "settings.h"

#include "task/task.h"
#include "task/taskimpl.h"

#include <stdbool.h>

/** The stream to send log messages to. */
extern FILE* HAVEN_debug_stream;

/** The log level to output. */
extern HAVEN_loglevel HAVEN_debug_level;

void UT_HAVEN_server_t_copy(void* _dst, const void* _src)
{
    HAVEN_server_t* dst = (HAVEN_server_t*)_dst;
    HAVEN_server_t* src = (HAVEN_server_t*)_src;

    dst->ctx = src->ctx;
    dst->consensus_db = src->consensus_db;
    dst->consensus_log = src->consensus_log;
    dst->listen_fd = src->listen_fd;

    uuid_copy(dst->uuid, src->uuid);
}

void UT_HAVEN_server_t_dtor(void* _elt)
{
    /* FIXME: properly free the contents of this thing (use halloc?). */
    //HAVEN_server_t* elt = (HAVEN_server_t*)_elt;
}

UT_icd HAVEN_server_t_icd = {
    sizeof(HAVEN_server_t), NULL,
    UT_HAVEN_server_t_copy,
    UT_HAVEN_server_t_dtor
};

int HAVEN_server_task(HAVEN_server_t* server)
{
    // TODO: remove the following and add the server to bootstrap state machine.
    int new_state = HAVEN_bootstrap_listen(server);

    return HAVEN_SUCCESS;
}

int HAVEN_init_server_queue(HAVEN_ctx_t* ctx)
{
    char* uuid_string = (char*) malloc(sizeof(char) * UUID_STR_LEN);
    utarray_new(ctx->server_queue, &HAVEN_server_t_icd);

    // TODO: Read from local settings and initialize the servers listed there.
    //       If no servers are listed in settings, then perform the following
    //       bootstrap init instead.
    //
    //HAVEN_load_servers_from_settings(ctx->server_queue);
    LOG(HAVEN_LOG_WARN, "Loading servers from settings is not implemented yet.");

    if(utarray_len(ctx->server_queue) == 0) {
        LOG(HAVEN_LOG_INFO, "No existing servers found. Performing bootstrap.");

        HAVEN_server_t* bootstrap_server = \
                                           (HAVEN_server_t*) malloc(sizeof(HAVEN_server_t));

        if(bootstrap_server == NULL) {
            LOG(HAVEN_LOG_ERR, "Could not allocate the bootstrap server.");
            return HAVEN_ERROR;
        }

        bootstrap_server->ctx = ctx;

        if(HAVEN_generate_uuid(&(bootstrap_server->uuid)) != HAVEN_SUCCESS) {
            LOG(HAVEN_LOG_ERR, "Could not create a UUID for the bootstrap server.");
            return HAVEN_ERROR;
        }

        uuid_unparse(bootstrap_server->uuid, uuid_string);
        LOG(HAVEN_LOG_INFO, "Using new UUID of `%s' for the bootstrap server.", uuid_string);

        utarray_push_back(ctx->server_queue, bootstrap_server);
    }

    return HAVEN_SUCCESS;
}

int HAVEN_init_server_loop(HAVEN_ctx_t* ctx)
{
    bool* is_running = (bool*) malloc(sizeof(bool));
    *is_running = true;

    if(HAVEN_init_server_queue(ctx) != HAVEN_SUCCESS) {
        LOG(HAVEN_LOG_ERR, "Couldn't not initialize server queue.");
        return HAVEN_ERROR;
    }

    ctx->listen_fd = netannounce(TCP, ctx->listen_addr, ctx->listen_port);
    LOG(HAVEN_LOG_INFO, "Listening on `%s:%d'.", ctx->listen_addr, ctx->listen_port);

    while(*is_running) {
        HAVEN_server_t* server = (HAVEN_server_t*) utarray_next(ctx->server_queue, NULL);

        server->listen_fd = ctx->listen_fd;
        taskcreate((void (*)(void*))HAVEN_server_task, server, HAVEN_SERVER_STACK_SIZE);

        taskswitch();
        LOG(HAVEN_LOG_INFO, "Server loop tick.");
    }

    free(is_running);
    utarray_free(ctx->server_queue);
    return HAVEN_SUCCESS;
}

/* EOF */
