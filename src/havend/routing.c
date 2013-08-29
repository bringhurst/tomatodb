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
#include "replica.h"
#include "task/task.h"

#include "routing.h"

/** The debug stream to write log messages to. */
extern FILE* HVN_debug_stream;

/** The log level to write messages for. */
extern HVN_loglevel HVN_debug_level;

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
    char new_uuid_string[UUID_STR_LEN];
    uuid_t new_uuid;
    HVN_replica_t* new_replica;

    if(HVN_proto_handle_connect_msg(router->accept_fd) != HVN_SUCCESS) {
        LOG(HVN_LOG_ERR, "Did not receive a valid connect message while routing.");
        taskexit(HVN_ERROR);
    }

    if(HVN_proto_receive_control_msg(router->accept_fd, &control_msg_data) != HVN_SUCCESS) {
        LOG(HVN_LOG_ERR, "Did not receive a valid control message while routing.");
        taskexit(HVN_ERROR);
    }
    else {
        if(HVN_proto_send_control_resp_msg(router->accept_fd) != HVN_SUCCESS) {
            LOG(HVN_LOG_ERR, "Failed to send a response to the control message.");
            taskexit(HVN_ERROR);
        }
        else {
            LOG(HVN_LOG_DBG, "Sent response to control message.");
        }
    }

    switch(control_msg_data.action) {

        case HVN_CLNT_PROTO_CTRL_ATTACH:

            if(HVN_replica_attach(router, control_msg_data.uuid) != HVN_SUCCESS) {
                uuid_unparse(control_msg_data.uuid, new_uuid_string);
                LOG(HVN_LOG_ERR, "Failed to attach to UUID `%s'.", new_uuid_string);
            }
            else {
                LOG(HVN_LOG_DBG, "Attach completed.");
            }

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
            HVN_replica_init(&new_replica);

            if(HVN_replica_bootstrap_leader(new_replica, router->ctx, \
                                            &new_uuid, control_msg_data.path) == HVN_SUCCESS) {
                uuid_unparse(new_uuid, new_uuid_string);
                LOG(HVN_LOG_ERR, "Quorum leader created with UUID `%s'.", new_uuid_string);
                //TODO: return uuid to requestor?
            }
            else {
                LOG(HVN_LOG_ERR, "Failed to bootstrap a quorum leader.");
            }

            break;

        case HVN_CLNT_PROTO_CTRL_LOCATION:
            HVN_replica_init(&new_replica);

            if(HVN_replica_bootstrap_location(new_replica, router->ctx, \
                                              &new_uuid) == HVN_SUCCESS) {
                uuid_unparse(new_uuid, new_uuid_string);
                LOG(HVN_LOG_ERR, "Location quorum leader created with UUID `%s'.", new_uuid_string);
                //TODO: return uuid to requestor?
            }
            else {
                LOG(HVN_LOG_ERR, "Failed to bootstrap a location quorum leader.");
            }

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

/* EOF */
