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

#include "consensus.h"
#include "pack/data_msg.h"
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
    switch(client->mode) {
        case HVN_ATTACH_CLNT_MODE_DATA:
            HVN_attach_data(client);
            break;

        case HVN_ATTACH_CLNT_MODE_VOTE:
            HVN_attach_vote(client);
            break;

        case HVN_ATTACH_CLNT_MODE_APPEND:
            HVN_attach_append(client);
            break;

        default:
            LOG(HVN_LOG_ERR, "Encountered an unrecognized attach mode.");
            break;
    }
}

void HVN_attach_append(HVN_attach_t* client)
{
    HVN_consensus_append_t append_msg_data;

    for(;;) {
        if(HVN_proto_receive_append_msg(client->fd, &append_msg_data) != HVN_SUCCESS) {
            LOG(HVN_LOG_ERR, "Did not receive a valid append message while attached to a replica.");
            taskexit(HVN_ERROR);
        }
        else {
            if(HVN_proto_send_append_resp_msg(client->fd) != HVN_SUCCESS) {
                LOG(HVN_LOG_ERR, "Could not send a append message response while attached to a replica.");
                taskexit(HVN_ERROR);
            }
        }

        if(chansendp(client->replica->append_chan, &append_msg_data) != 1) {
            LOG(HVN_LOG_ERR, "Failed to send the append message to the appropriate replica.");
        }
        else {
            LOG(HVN_LOG_DBG, "Successfully sent the append message to the appropriate replica.");
        }
    }
}

void HVN_attach_vote(HVN_attach_t* client)
{
    HVN_consensus_vote_t vote_msg_data;

    for(;;) {
        if(HVN_proto_receive_vote_msg(client->fd, &vote_msg_data) != HVN_SUCCESS) {
            LOG(HVN_LOG_ERR, "Did not receive a valid vote message while attached to a replica.");
            taskexit(HVN_ERROR);
        }
        else {
            if(HVN_proto_send_vote_resp_msg(client->fd) != HVN_SUCCESS) {
                LOG(HVN_LOG_ERR, "Could not send a vote message response while attached to a replica.");
                taskexit(HVN_ERROR);
            }
        }

        if(chansendp(client->replica->vote_chan, &vote_msg_data) != 1) {
            LOG(HVN_LOG_ERR, "Failed to send the vote message to the appropriate replica.");
        }
        else {
            LOG(HVN_LOG_DBG, "Successfully sent the vote message to the appropriate replica.");
        }
    }
}

void HVN_attach_data(HVN_attach_t* client)
{
    HVN_msg_client_data_t data_msg_data;

    for(;;) {
        if(HVN_proto_receive_data_msg(client->fd, &data_msg_data) != HVN_SUCCESS) {
            LOG(HVN_LOG_ERR, "Did not receive a valid data message while attached to a replica.");
            taskexit(HVN_ERROR);
        }
        else {
            if(HVN_proto_send_data_resp_msg(client->fd) != HVN_SUCCESS) {
                LOG(HVN_LOG_ERR, "Could not send a data message response while attached to a replica.");
                taskexit(HVN_ERROR);
            }
        }

        if(chansendp(client->replica->data_chan, &data_msg_data) != 1) {
            LOG(HVN_LOG_ERR, "Failed to send the data message to the appropriate replica.");
        }
        else {
            LOG(HVN_LOG_DBG, "Successfully sent the data message to the appropriate replica.");
        }
    }
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
