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
#include "pack/append_msg.h"
#include "pack/data_msg.h"
#include "pack/vote_msg.h"
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
    taskcreate((void (*)(void*))HVN_attach_send_task, client, HVN_ATTACH_STACK_SIZE);
    HVN_attach_recv(client);
}

void HVN_attach_recv(HVN_attach_t* client)
{
    switch(client->mode) {
        case HVN_ATTACH_MODE_DATA:
            HVN_attach_recv_data(client);
            break;

        case HVN_ATTACH_MODE_VOTE:
            HVN_attach_recv_vote(client);
            break;

        case HVN_ATTACH_MODE_APPEND:
            HVN_attach_recv_append(client);
            break;

        default:
            LOG(HVN_LOG_ERR, "Encountered an unrecognized attach mode.");
            break;
    }

    //TODO: send message to attach send task to shutdown
}

void HVN_attach_send_task(HVN_attach_t* client)
{
    static Alt alts[HVN_ATTACH_SEND_ALT_NK + 1];

    uint32_t exit_msg;

    HVN_msg_append_resp_t append_msg;
    HVN_msg_data_resp_t data_msg;
    HVN_msg_vote_resp_t vote_msg;

    alts[HVN_ATTACH_SEND_ALT_APPEND_KEY].c = client->append_reply_chan;
    alts[HVN_ATTACH_SEND_ALT_APPEND_KEY].v = &append_msg;
    alts[HVN_ATTACH_SEND_ALT_APPEND_KEY].op = CHANRCV;

    alts[HVN_ATTACH_SEND_ALT_DATA_KEY].c = client->data_reply_chan;
    alts[HVN_ATTACH_SEND_ALT_DATA_KEY].v = &data_msg;
    alts[HVN_ATTACH_SEND_ALT_DATA_KEY].op = CHANRCV;

    alts[HVN_ATTACH_SEND_ALT_EXIT_KEY].c = client->exit_chan;
    alts[HVN_ATTACH_SEND_ALT_EXIT_KEY].v = &exit_msg;
    alts[HVN_ATTACH_SEND_ALT_EXIT_KEY].op = CHANRCV;

    alts[HVN_ATTACH_SEND_ALT_VOTE_KEY].c = client->vote_reply_chan;
    alts[HVN_ATTACH_SEND_ALT_VOTE_KEY].v = &vote_msg;
    alts[HVN_ATTACH_SEND_ALT_VOTE_KEY].op = CHANRCV;

    for(;;) {
        switch(chanalt(alts)) {

            case HVN_ATTACH_SEND_ALT_APPEND_KEY:
                if(HVN_proto_send_append_resp_msg(client->fd, &append_msg) != HVN_SUCCESS) {
                    LOG(HVN_LOG_ERR, "Could not send a append message response while attached to a replica.");
                    taskexit(HVN_ERROR);
                }
                break;

            case HVN_ATTACH_SEND_ALT_DATA_KEY:
                if(HVN_proto_send_data_resp_msg(client->fd, &data_msg) != HVN_SUCCESS) {
                    LOG(HVN_LOG_ERR, "Could not send a data message response while attached to a replica.");
                    taskexit(HVN_ERROR);
                }
                break;

            case HVN_ATTACH_SEND_ALT_EXIT_KEY:
                LOG(HVN_LOG_DBG, "Exiting attach send task.");
                taskexit(HVN_ERROR);
                break;

            case HVN_ATTACH_SEND_ALT_VOTE_KEY:
                if(HVN_proto_send_vote_resp_msg(client->fd, &vote_msg) != HVN_SUCCESS) {
                    LOG(HVN_LOG_ERR, "Could not send a vote message response while attached to a replica.");
                    taskexit(HVN_ERROR);
                }
                break;

            default:
                LOG(HVN_LOG_DBG, "This attach task eceived an unknown message type from a replica.");
                break;
        }
    }
}

void HVN_attach_recv_append(HVN_attach_t* client)
{
    HVN_msg_append_t* append_msg_data = (HVN_msg_append_t*) malloc(sizeof(HVN_msg_append_t));
    HVN_attach_msg_t* attach_msg = (HVN_attach_msg_t*) malloc(sizeof(HVN_attach_msg_t));

    if(append_msg_data != NULL && attach_msg != NULL) {
        LOG(HVN_LOG_ERR, "Could not allocate memory to send an append msg to a replica.");
        taskexit(HVN_ERROR);
    }

    attach_msg->msg = append_msg_data;
    attach_msg->append_reply_chan = client->append_reply_chan;

    for(;;) {
        if(HVN_proto_receive_append_msg(client->fd, append_msg_data) != HVN_SUCCESS) {
            LOG(HVN_LOG_ERR, "Did not receive a valid append message while attached to a replica.");
            taskexit(HVN_ERROR);
        }

        if(chansendp(client->replica->append_chan, attach_msg) != 1) {
            LOG(HVN_LOG_ERR, "Failed to send the append message to the appropriate replica.");
            taskexit(HVN_ERROR);
        }
        else {
            LOG(HVN_LOG_DBG, "Successfully sent the append message to the appropriate replica.");
        }
    }
}

void HVN_attach_recv_vote(HVN_attach_t* client)
{
    HVN_msg_vote_t* vote_msg_data = (HVN_msg_vote_t*) malloc(sizeof(HVN_msg_vote_t));
    HVN_attach_msg_t* attach_msg = (HVN_attach_msg_t*) malloc(sizeof(HVN_attach_msg_t));

    if(vote_msg_data != NULL && attach_msg != NULL) {
        LOG(HVN_LOG_ERR, "Could not allocate memory to send a vote msg to a replica.");
        taskexit(HVN_ERROR);
    }

    attach_msg->msg = vote_msg_data;
    attach_msg->vote_reply_chan = client->vote_reply_chan;

    for(;;) {
        if(HVN_proto_receive_vote_msg(client->fd, vote_msg_data) != HVN_SUCCESS) {
            LOG(HVN_LOG_ERR, "Did not receive a valid vote message while attached to a replica.");
            taskexit(HVN_ERROR);
        }

        if(chansendp(client->replica->vote_chan, attach_msg) != 1) {
            LOG(HVN_LOG_ERR, "Failed to send the vote message to the appropriate replica.");
            taskexit(HVN_ERROR);
        }
        else {
            LOG(HVN_LOG_DBG, "Successfully sent the vote message to the appropriate replica.");
        }
    }
}

void HVN_attach_recv_data(HVN_attach_t* client)
{
    HVN_msg_data_t* data_msg_data = (HVN_msg_data_t*) malloc(sizeof(HVN_msg_data_t));
    HVN_attach_msg_t* attach_msg = (HVN_attach_msg_t*) malloc(sizeof(HVN_attach_msg_t));

    if(data_msg_data != NULL && attach_msg != NULL) {
        LOG(HVN_LOG_ERR, "Could not allocate memory to send a data msg to a replica.");
        taskexit(HVN_ERROR);
    }

    attach_msg->msg = data_msg_data;
    attach_msg->data_reply_chan = client->data_reply_chan;

    for(;;) {
        if(HVN_proto_receive_data_msg(client->fd, data_msg_data) != HVN_SUCCESS) {
            LOG(HVN_LOG_ERR, "Did not receive a valid data message while attached to a replica.");
            taskexit(HVN_ERROR);
        }

        if(chansendp(client->replica->data_chan, attach_msg) != 1) {
            LOG(HVN_LOG_ERR, "Failed to send the data message to the appropriate replica.");
            taskexit(HVN_ERROR);
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
        LOG(HVN_LOG_ERR, "Attempted to attach to a replica which doesn't exist in this instance.");
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

    (*client)->append_reply_chan = chancreate(sizeof(HVN_msg_append_resp_t*), \
                                              HVN_ATTACH_CHANNEL_BACKLOG);
    (*client)->data_reply_chan = chancreate(sizeof(HVN_msg_data_resp_t*), \
                                            HVN_ATTACH_CHANNEL_BACKLOG);
    (*client)->vote_reply_chan = chancreate(sizeof(HVN_msg_vote_resp_t*), \
                                            HVN_ATTACH_CHANNEL_BACKLOG);

    (*client)->exit_chan = chancreate(sizeof(uint32_t), \
                                      HVN_ATTACH_CHANNEL_BACKLOG);

    return HVN_SUCCESS;
}

void HVN_attach_free(HVN_attach_t* client)
{
    free(client->remote_addr);
    free(client);
}

/* EOF */
