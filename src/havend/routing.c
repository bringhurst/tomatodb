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

#include "client_protocol/client_protocol.h"
#include "client_protocol/connect_msg.h"

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
    int pack_result, unpack_result;
    size_t len = 0;
    char* msg;

    LOG(HVN_LOG_DBG, "Entered routing task.");

    HVN_msg_client_connect_t connect_msg_data;

    if(HVN_msgpack_fdread(router->accept_fd, &len, &msg) != HVN_SUCCESS) {
        LOG(HVN_LOG_ERR, "Failed to read a connect message while routing.");
        return;
    }

//    connect_msg_data_orig.magic = HVN_CLIENT_PROTOCOL_MAGIC;
//    connect_msg_data_orig.version = HVN_CLIENT_PROTOCOL_VERSION;

    unpack_result = HVN_clnt_proto_unpack(HVN_CLNT_PROTO_MSG_TYPE_CONNECT, \
                                          HVN_CLNT_PROTO_PACK_TYPE_MSGPACK, \
                                          &connect_msg_data, len, msg);


    if(connect_msg_data.magic == HVN_CLIENT_PROTOCOL_MAGIC) {
        LOG(HVN_LOG_DBG, "Incoming messsage has correct magic value.");
    } else {
        LOG(HVN_LOG_DBG, "Incoming messsage does not have the correct magic value.");
        // TODO exit task
    }

    if(connect_msg_data.version == HVN_CLIENT_PROTOCOL_VERSION) {
        LOG(HVN_LOG_DBG, "Incoming messsage has correct version value.");
    } else {
        LOG(HVN_LOG_DBG, "Incoming messsage does not have the correct version value.");
        // TODO exit task
    }

/*****
    if(incoming_message_contains_server_uuid) {
        HVN_server_t* server = NULL;
        HASH_FIND_INT(router->ctx->server_routes, &incoming_server_uuid, server);

        if(server != NULL) {
            LOG(HVN_LOG_INFO, "We found the server that the client was looking for!");
            //TODO: create a connection task and add it to the server task.
        } else {
            LOG(HVN_LOG_INFO, "We could not find the server that the client specified.");
            //TODO: tell the client to use the location quorum.
        }
    } else {
        LOG(HVN_LOG_INFO, "The incoming message did not contain a server UUID.");
        //TODO: determine what the client is trying to do (1, 2, or 3).
    }
*******/

    free(msg);
    HVN_free_router(router);
}

// This is where we'll listen and accept for connections so we can launch
// routing tasks.
int HVN_listen_and_accept(HVN_ctx_t* ctx)
{
    int remote_port, accept_fd = 0;
    bool* is_running = (bool*) malloc(sizeof(bool));
    char* remote_addr = (char*) malloc(sizeof(char) * _POSIX_HOST_NAME_MAX);

    *is_running = true;

    // TODO
    //HVN_handle_shutdown_signals(is_running);

    ctx->listen_fd = netannounce(TCP, ctx->listen_addr, ctx->listen_port);
    LOG(HVN_LOG_INFO, "Listening on `%s:%d'.", ctx->listen_addr, ctx->listen_port);

    while(*is_running) {
        accept_fd = netaccept(ctx->listen_fd, remote_addr, &remote_port);
        HVN_router_t* router = NULL;

        if(HVN_init_router(&router, ctx, remote_addr, remote_port, accept_fd) != HVN_SUCCESS) {
            LOG(HVN_LOG_ERR, "Could not proper initialize a new connection router. Attempting to shut down.");
            *is_running = false;
            break;
        }
        taskcreate((void (*)(void*))HVN_routing_task, router, HVN_ROUTER_STACK_SIZE);
        taskswitch();
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
