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

#include "protocol/client/data_msg.h"
#include "replica.h"
#include "task/task.h"

#include "attach.h"
#include "log.h"

/** The stream to send log messages to. */
extern FILE* HVN_debug_stream;

/** The log level to output. */
extern HVN_loglevel HVN_debug_level;

void HVN_attach_task(HVN_attach_t* client)
{
    HVN_msg_client_data_t data_msg_data;

    if(HVN_proto_receive_data_msg(client->fd, &data_msg_data) != HVN_SUCCESS) {
        LOG(HVN_LOG_ERR, "Did not receive a valid control message while routing.");
        taskexit(HVN_ERROR);
    }

    HVN_proto_print_data_msg(&data_msg_data);

    LOG(HVN_LOG_DBG, "Received a sane data msg.");

    //TODO: handle DATA commands and apply to client->replica->db
}

int HVN_replica_attach(HVN_router_t* router, uuid_t uuid)
{
    HVN_attach_t* client;
    HVN_replica_t* replica;

    HASH_FIND(hh, router->ctx->replicas, uuid, sizeof(uuid_t), replica);
    if(replica == NULL) {
        LOG(HVN_LOG_ERR, "Attempted to attach to a replica which doesn't exit in this instance.");
        return HVN_ERROR;
    }

    LOG(HVN_LOG_DBG, "Client attached to a replica. Preparing to handle commands.");
    
    if(HVN_attach_init(&client, router, replica) != HVN_SUCCESS) {
        LOG(HVN_LOG_INFO, "Failed to allocate memory to attach a new client.");
    }

    taskcreate((void (*)(void*))HVN_attach_task, client, HVN_ATTACH_STACK_SIZE);
    return HVN_SUCCESS;
}

int HVN_attach_init(HVN_attach_t** client, HVN_router_t* router, HVN_replica_t* replica)
{
    *client = (HVN_attach_t*) malloc(sizeof(HVN_attach_t));
    if(*client == NULL) {
        LOG(HVN_LOG_ERR, "Failed to allocate memory for a new client attach.");
        return HVN_ERROR;
    }

    (*client)->remote_addr = (char*) malloc(sizeof(char) * _POSIX_HOST_NAME_MAX);
    if((*client)->remote_addr == NULL) {
        LOG(HVN_LOG_ERR, "Failed to allocate memory for the address of a new client attach.");
        return HVN_ERROR;
    }

    strncpy((*client)->remote_addr, router->remote_addr, _POSIX_HOST_NAME_MAX);
    (*client)->fd = router->accept_fd;
    (*client)->remote_port = router->remote_port;
    (*client)->replica = replica;

    return HVN_SUCCESS;
}

void HVN_attach_free(HVN_attach_t* client)
{
    free(client->remote_addr);
    free(client);
}

/* EOF */
