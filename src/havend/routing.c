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

#include "routing.h"
#include "log.h"

#include "common.h"

#include "task/task.h"
#include "task/taskimpl.h"

#include <stdbool.h>

/** The debug stream to write log messages to. */
extern FILE* HAVEN_debug_stream;

/** The log level to write messages for. */
extern HAVEN_loglevel HAVEN_debug_level;

//
// This is the router for all new incoming connections. The overall flow
// for what we're doing here is as follows:
//
// First, we'll read the first message that comes in from the new connection.
//
// If a server ID is in the request, we'll attempt to lookup the server task
// in the context routing hash table. If the server task exists in the hash
// table, we'll create a new connection task and add it to the server's
// connection list.
//
// If the server ID is not in the context's routing hash table, we respond to
// the new connection with a suggestion to ask the location quorum where the
// server may be located at. If possible, we should send the address and port
// number of the last known location quorum leader.
//
// If the client doesn't send a server ID at all, we'll then try to determine
// what the client wants to do. The remaining options are:
//
//   1. Become a client-server instance.
//   2. Simply want help to find the address and port of the location quorum
//      leader
//   3. Become a new location quorum leader.
//
void HAVEN_routing_task(HAVEN_router_t* router)
{
    LOG(HAVEN_LOG_INFO, "Started routing task.");

    // TODO: everything described above.

    HAVEN_free_router(router);
}

// This is where we'll listen and accept for connections so we can launch
// routing tasks.
int HAVEN_listen_and_accept(HAVEN_ctx_t* ctx)
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

int HAVEN_init_router(HAVEN_router_t** router, \
                      HAVEN_ctx_t* ctx, \
                      char* remote_addr, \
                      int remote_port, 
                      int accept_fd)
{
    *router = (HAVEN_router_t*) malloc(sizeof(HAVEN_router_t));

    if(*router == NULL) {
        LOG(HAVEN_LOG_ERR, "Failed to allocate memory for a connection router.");
        return HAVEN_ERROR;
    }

    (*router)->remote_addr = (char*) malloc(sizeof(char) * _POSIX_HOST_NAME_MAX);

    if((*router)->remote_addr == NULL) {
        LOG(HAVEN_LOG_ERR, "Failed to allocate memory within a connection router.");
        free(*router);
        return HAVEN_ERROR;
    }

    strncpy((*router)->remote_addr, remote_addr, _POSIX_HOST_NAME_MAX);

    (*router)->ctx = ctx;
    (*router)->remote_port = remote_port;
    (*router)->accept_fd = accept_fd;

    return HAVEN_SUCCESS;
}

void HAVEN_free_router(HAVEN_router_t* router)
{
    free(router->remote_addr);
    free(router);
}

/* EOF */
