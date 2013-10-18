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
#include <string.h>
#include <uuid/uuid.h>

#include "attach.h"
#include "log.h"
#include "pack/pack.h"
#include "pack/connect_msg.h"
#include "pack/control_msg.h"
#include "bootstrap.h"
#include "task/task.h"

#include "routing.h"

/** The debug stream to write log messages to. */
extern FILE* TDB_debug_stream;

/** The log level to write messages for. */
extern TDB_loglevel TDB_debug_level;

int TDB_init_router(TDB_router_t** router, \
                    TDB_ctx_t* ctx, \
                    char* remote_addr, \
                    int remote_port,
                    int accept_fd)
{
    *router = (TDB_router_t*) malloc(sizeof(TDB_router_t));

    if(*router == NULL) {
        LOG(TDB_LOG_ERR, "Failed to allocate memory for a connection router.");
        return TDB_ERROR;
    }

    (*router)->remote_addr = (char*) malloc(sizeof(char) * _POSIX_HOST_NAME_MAX);

    if((*router)->remote_addr == NULL) {
        LOG(TDB_LOG_ERR, "Failed to allocate memory within a connection router.");
        free(*router);
        return TDB_ERROR;
    }

    strncpy((*router)->remote_addr, remote_addr, _POSIX_HOST_NAME_MAX);

    (*router)->ctx = ctx;
    (*router)->remote_port = remote_port;
    (*router)->accept_fd = accept_fd;

    return TDB_SUCCESS;
}

void TDB_free_router(TDB_router_t* router)
{
    free(router->remote_addr);
    free(router);
}

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
void TDB_routing_task(TDB_router_t* router)
{
    TDB_msg_client_control_t control_msg_data;
    char new_uuid_string[UUID_STR_LEN];
    uuid_t new_uuid;
    TDB_replica_t* new_replica;

    taskname("router");

    if(TDB_proto_handle_connect_msg(router->accept_fd) != TDB_SUCCESS) {
        LOG(TDB_LOG_ERR, "Did not receive a valid connect message while routing.");
        taskexit(TDB_ERROR);
    }

    if(TDB_proto_receive_control_msg(router->accept_fd, &control_msg_data) != TDB_SUCCESS) {
        LOG(TDB_LOG_ERR, "Did not receive a valid control message while routing.");
        taskexit(TDB_ERROR);
    }
    else {
        if(TDB_proto_send_control_resp_msg(router->accept_fd) != TDB_SUCCESS) {
            LOG(TDB_LOG_ERR, "Failed to send a response to the control message.");
            taskexit(TDB_ERROR);
        }
        else {
            LOG(TDB_LOG_DBG, "Sent response to control message.");
        }
    }

    switch(control_msg_data.action) {

        case TDB_PROTO_CTRL_ATTACH_APPEND:
        case TDB_PROTO_CTRL_ATTACH_DATA:
        case TDB_PROTO_CTRL_ATTACH_VOTE:

            if(TDB_replica_attach(router, control_msg_data.uuid, control_msg_data.action) != TDB_SUCCESS) {
                uuid_unparse(control_msg_data.uuid, new_uuid_string);
                LOG(TDB_LOG_ERR, "Failed to attach to UUID `%s'.", new_uuid_string);
            }
            else {
                LOG(TDB_LOG_DBG, "Attach completed.");
            }

            break;

        case TDB_PROTO_CTRL_PROXY:
            LOG(TDB_LOG_DBG, "Handling control proxy message.");
            LOG(TDB_LOG_ERR, "Not implemented yet.");
            break;

        case TDB_PROTO_CTRL_FOLLOWER:
            LOG(TDB_LOG_DBG, "Handling control follower message.");
            LOG(TDB_LOG_ERR, "Not implemented yet.");
            break;

        case TDB_PROTO_CTRL_LEADER:
            TDB_replica_init(&new_replica);

            if(TDB_bootstrap_leader(new_replica, router->ctx, \
                                    &new_uuid, control_msg_data.path) == TDB_SUCCESS) {
                uuid_unparse(new_uuid, new_uuid_string);
                LOG(TDB_LOG_ERR, "Quorum leader created with UUID `%s'.", new_uuid_string);
                //TODO: return uuid to requestor?
            }
            else {
                LOG(TDB_LOG_ERR, "Failed to bootstrap a quorum leader.");
            }

            break;

        case TDB_PROTO_CTRL_LOCATION:
            TDB_replica_init(&new_replica);

            if(TDB_bootstrap_location(new_replica, router->ctx, \
                                      &new_uuid) == TDB_SUCCESS) {
                uuid_unparse(new_uuid, new_uuid_string);
                LOG(TDB_LOG_ERR, "Location quorum leader created with UUID `%s'.", new_uuid_string);
                //TODO: return uuid to requestor?
            }
            else {
                LOG(TDB_LOG_ERR, "Failed to bootstrap a location quorum leader.");
            }

            break;

        case TDB_PROTO_CTRL_EXIT:
            LOG(TDB_LOG_DBG, "Handling control exit message.");
            LOG(TDB_LOG_ERR, "Not implemented yet.");
            break;

        case TDB_PROTO_CTRL_DESTROY:
            LOG(TDB_LOG_DBG, "Handling control destroy message.");
            LOG(TDB_LOG_ERR, "Not implemented yet.");
            break;

        default:
            LOG(TDB_LOG_ERR, "Encountered unrecognized control message type of `%d'.", \
                control_msg_data.action);
            taskexit(TDB_ERROR);
    }

    LOG(TDB_LOG_DBG, "Completed routing. Exiting task.");
    TDB_free_router(router);

    taskexit(TDB_SUCCESS);
}

/* EOF */
