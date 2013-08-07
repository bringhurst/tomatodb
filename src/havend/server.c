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

#include "task/task.h"

#include <stdbool.h>

/** The stream to send log messages to. */
extern FILE* HAVEN_debug_stream;

/** The log level to output. */
extern HAVEN_loglevel HAVEN_debug_level;

int HAVEN_server_accept(HAVEN_server_t* server)
{
    // TODO: create a new connection task in the server struct.
    //
    // TODO: netaccept in the connection task.
    //
    // TODO: pass server instance to state machine

    LOG(HAVEN_LOG_ERR, "Server accept is not implemented.");
    return HAVEN_SUCCESS;
}

int HAVEN_init_server_loop(HAVEN_ctx_t* ctx)
{
    bool RUNNING = true;

    if(HAVEN_xarray_init(&(ctx->server_queue), \
                         INITIAL_SERVER_QUEUE_SIZE) != HAVEN_SUCCESS) {
        LOG(HAVEN_LOG_ERR, "Couldn't not initialize server queue.");
        return HAVEN_ERROR;
    }

    while(RUNNING) {
        // FIXME: should this be a pop?
        HAVEN_server_t* server = HAVEN_xarray_peek(ctx->server_queue);
        server->listen_fd = netannounce(TCP, server->listen_addr, server->listen_port);

        // TODO: setup server struct
        
        if(HAVEN_server_accept(server) != HAVEN_SUCCESS) {
            LOG(HAVEN_LOG_INFO, "Failed to accept a new client.");
        }

        RUNNING = false;
    }

    HAVEN_xarray_free(ctx->server_queue);
    return HAVEN_SUCCESS;
}

/* EOF */
