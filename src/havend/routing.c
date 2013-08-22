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

#include "routing.h"
#include "log.h"

#include "common.h"

#include "task/task.h"
#include "task/taskimpl.h"

#include "client_protocol/client_protocol.h"
#include "client_protocol/connect_msg.h"
#include "client_protocol/control_msg.h"

#include <errno.h>
#include <msgpack.h>
#include <stdbool.h>

/** The debug stream to write log messages to. */
extern FILE* HVN_debug_stream;

/** The log level to write messages for. */
extern HVN_loglevel HVN_debug_level;

//
// This is the router for all new incoming connections. The overall flow
// for what we're doing here is as follows:
//
// First, we'll read the request message that comes in off the new connection.
//
// If a server ID is in the request, we'll attempt to lookup the server task
// in the context routing hash table. If the server task exists in the hash
// table, we'll create a new connection task and add it to the server's
// connection list.
//
// If the server ID is not in the context's routing hash table, we respond to
// the new connection with a suggestion to have the client ask the location
// quorum where the server may be located at. If possible, we should send the
// address and port number of the last known location quorum leader (or any
// location quorum member if the leader is not known).
//
// If the client doesn't send a server ID at all, we'll then try to determine
// what the client wants to do. The remaining options to attempt at this point
// are the following:
//
//   1. Become a client-server instance.
//   2. Respond with the address and port of the location quorum leader (or a
//      location quorum member if the leader is not known).
//   3. Create a new location quorum by becoming a location quorum leader.
//
void HVN_routing_task(HVN_router_t* router)
{
    HVN_msg_client_control_t control_msg_data;

    if(HVN_proto_handle_connect_msg(router->accept_fd) != HVN_SUCCESS) {
        LOG(HVN_LOG_ERR, "Did not receive a valid connect message while routing.");
        taskexit(HVN_ERROR);
    }

    LOG(HVN_LOG_DBG, "Connect handshake completed.");

    if(HVN_proto_receive_control_msg(router->accept_fd, &control_msg_data) != HVN_SUCCESS) {
        LOG(HVN_LOG_ERR, "Did not receive a valid control message while routing.");
        taskexit(HVN_ERROR);
    }

    switch(control_msg_data.action) {

        case HVN_CLNT_PROTO_CTRL_ATTACH:
            LOG(HVN_LOG_DBG, "Handling control attach message.");
            LOG(HVN_LOG_ERR, "Not implemented yet.");
            break;

        case HVN_CLNT_PROTO_CTRL_PROXY:
            LOG(HVN_LOG_DBG, "Handling control proxy message.");
            LOG(HVN_LOG_ERR, "Not implemented yet.");
            break;

        case HVN_CLNT_PROTO_CTRL_FOLLOWER:
            LOG(HVN_LOG_DBG, "Handling control follower message.");
            LOG(HVN_LOG_ERR, "Not implemented yet.");
            break;

        case HVN_CLNT_PROTO_CTRL_LEADER:
            LOG(HVN_LOG_DBG, "Handling control leader message.");
            LOG(HVN_LOG_ERR, "Not implemented yet.");
            break;

        case HVN_CLNT_PROTO_CTRL_LOCATION:
            LOG(HVN_LOG_DBG, "Handling control location message.");
            LOG(HVN_LOG_ERR, "Not implemented yet.");
            break;

        case HVN_CLNT_PROTO_CTRL_EXIT:
            LOG(HVN_LOG_DBG, "Handling control exit message.");
            LOG(HVN_LOG_ERR, "Not implemented yet.");
            break;

        case HVN_CLNT_PROTO_CTRL_DESTROY:
            LOG(HVN_LOG_DBG, "Handling control destroy message.");
            LOG(HVN_LOG_ERR, "Not implemented yet.");
            break;

        default:
            LOG(HVN_LOG_ERR, "Encountered unrecognized control message type of `%d'.", \
                control_msg_data.action);
            taskexit(HVN_ERROR);
    }

    LOG(HVN_LOG_DBG, "Completed routing. Exiting task.");
    HVN_free_router(router);

    taskexit(HVN_SUCCESS);
}

// This is where we'll listen and accept for connections so we can launch
// routing tasks.
int HVN_listen_and_accept(HVN_ctx_t* ctx)
{
    int remote_port, accept_fd = 0;
    bool* is_running = (bool*) malloc(sizeof(bool));
    char* remote_addr = (char*) malloc(sizeof(char) * _POSIX_HOST_NAME_MAX);

    *is_running = true;

    // FIXME: clean up signal blocking
    //HVN_handle_shutdown_signals(is_running);
    struct sigaction sa;
    memset(&sa, 0, sizeof sa);
    sa.sa_handler = SIG_IGN;
    sigaction(SIGPIPE, &sa, NULL);

    ctx->listen_fd = netannounce(TCP, ctx->listen_addr, ctx->listen_port);

    if(fdnoblock(ctx->listen_fd) < 0) {
        LOG(HVN_LOG_ERR, "Failed to set the listening socket to non-blocking. %s", strerror(errno));
        return HVN_ERROR;
    }

    LOG(HVN_LOG_INFO, "Listening on `%s:%d'.", ctx->listen_addr, ctx->listen_port);

    while(*is_running) {
        accept_fd = netaccept(ctx->listen_fd, remote_addr, &remote_port);
        HVN_router_t* router = NULL;

        if(accept_fd < 0) {
            LOG(HVN_LOG_ERR, "Failed to accept a new connection. Attempting to shut down.");
            *is_running = false;
            break;
        }

        if(HVN_init_router(&router, ctx, remote_addr, remote_port, accept_fd) != HVN_SUCCESS) {
            LOG(HVN_LOG_ERR, "Could not proper initialize a new connection router. Attempting to shut down.");
            *is_running = false;
            break;
        }
        taskcreate((void (*)(void*))HVN_routing_task, router, HVN_ROUTER_STACK_SIZE);
    }

    free(is_running);
    return HVN_SUCCESS;
}

int HVN_init_router(HVN_router_t** router, \
                      HVN_ctx_t* ctx, \
                      char* remote_addr, \
                      int remote_port, 
                      int accept_fd)
{
    *router = (HVN_router_t*) malloc(sizeof(HVN_router_t));

    if(*router == NULL) {
        LOG(HVN_LOG_ERR, "Failed to allocate memory for a connection router.");
        return HVN_ERROR;
    }

    (*router)->remote_addr = (char*) malloc(sizeof(char) * _POSIX_HOST_NAME_MAX);

    if((*router)->remote_addr == NULL) {
        LOG(HVN_LOG_ERR, "Failed to allocate memory within a connection router.");
        free(*router);
        return HVN_ERROR;
    }

    strncpy((*router)->remote_addr, remote_addr, _POSIX_HOST_NAME_MAX);

    (*router)->ctx = ctx;
    (*router)->remote_port = remote_port;
    (*router)->accept_fd = accept_fd;

    return HVN_SUCCESS;
}

void HVN_free_router(HVN_router_t* router)
{
    free(router->remote_addr);
    free(router);
}

/* EOF */
