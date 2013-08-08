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
#include "settings.h"
#include "routing.h"

#include "task/task.h"
#include "task/taskimpl.h"

#include <stdbool.h>

/** The stream to send log messages to. */
extern FILE* HAVEN_debug_stream;

/** The log level to output. */
extern HAVEN_loglevel HAVEN_debug_level;

int HAVEN_init_server_loop(HAVEN_ctx_t* ctx)
{
    int remote_port, accept_fd = 0;
    bool* is_running = (bool*) malloc(sizeof(bool));
    char* remote_addr = (char*) malloc(sizeof(char) * _POSIX_HOST_NAME_MAX);

    *is_running = true;

    // TODO
    //HAVEN_handle_shutdown_signals(is_running);

    ctx->listen_fd = netannounce(TCP, ctx->listen_addr, ctx->listen_port);
    LOG(HAVEN_LOG_INFO, "Listening on `%s:%d'.", ctx->listen_addr, ctx->listen_port);

    while(*is_running) {
        accept_fd = netaccept(ctx->listen_fd, remote_addr, &remote_port);
        HAVEN_router_t* router = NULL;
        
        if(HAVEN_init_router(&router, ctx, remote_addr, remote_port, accept_fd) != HAVEN_SUCCESS) {
            LOG(HAVEN_LOG_ERR, "Could not proper initialize a new connection router. Attempting to shut down.");
            *is_running = false;
            break;
        }
        taskcreate((void (*)(void*))HAVEN_routing_task, router, HAVEN_ROUTER_STACK_SIZE);
        taskswitch();
    }

    free(is_running);
    return HAVEN_SUCCESS;
}

/* EOF */
