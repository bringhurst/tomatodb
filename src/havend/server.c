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
#include "xarray.h"
#include "newbie.h"
#include "settings.h"

#include "task/task.h"
#include "task/taskimpl.h"

#include <stdbool.h>

/** The stream to send log messages to. */
extern FILE* HAVEN_debug_stream;

/** The log level to output. */
extern HAVEN_loglevel HAVEN_debug_level;

int HAVEN_server_task(HAVEN_server_t* server)
{
    // TODO: add server to newbie state machine.

    int new_state = HAVEN_newbie_listen_and_wait(server);

    return HAVEN_SUCCESS;
}

int HAVEN_init_server_queue(HAVEN_ctx_t* ctx)
{
    if(HAVEN_xarray_init(&(ctx->server_queue), \
                         INITIAL_SERVER_QUEUE_SIZE) != HAVEN_SUCCESS) {
        LOG(HAVEN_LOG_ERR, "Couldn't not initialize server queue array.");
        return HAVEN_ERROR;
    }

    // TODO: Read from local settings and initialize the servers listed there.
    //       If no servers are listed in settings, then perform the following
    //       bootstrap init instead.
    //
    //HAVEN_load_servers_from_settings(ctx->server_queue);
    LOG(HAVEN_LOG_WARN, "Loading servers from settings is not implemented yet.");

    if(HAVEN_xarray_size(ctx->server_queue) == 0) {
        LOG(HAVEN_LOG_INFO, "No existing servers found. Performing bootstrap.");

        HAVEN_server_t* bootstrap_server = \
            (HAVEN_server_t*) malloc(sizeof(HAVEN_server_t));
        if(bootstrap_server == NULL) {
            LOG(HAVEN_LOG_ERR, "Could not allocate the bootstrap server.");
            return HAVEN_ERROR;
        }

        if(HAVEN_generate_uuid(&(bootstrap_server->uuid)) != HAVEN_SUCCESS) {
            LOG(HAVEN_LOG_ERR, "Could not create a UUID for the bootstrap server.");
            return HAVEN_ERROR;
        }

        bootstrap_server->ctx = ctx;

        if(HAVEN_xarray_push(ctx->server_queue, bootstrap_server) != HAVEN_SUCCESS) {
            LOG(HAVEN_LOG_ERR, "Failed to put bootstrap server on the primary server queue.");
            return HAVEN_ERROR;
        }
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
        HAVEN_server_t* server = NULL;

        if(HAVEN_xarray_pop(ctx->server_queue, (void**)&server) == HAVEN_SUCCESS) {
            server->listen_fd = ctx->listen_fd;
            taskcreate((void (*)(void *))HAVEN_server_task, server, HAVEN_SERVER_STACK_SIZE);
        }

        taskswitch();
        LOG(HAVEN_LOG_INFO, "Server loop tick.");
    }

    free(is_running);
    HAVEN_xarray_free(ctx->server_queue);
    return HAVEN_SUCCESS;
}

/* EOF */
