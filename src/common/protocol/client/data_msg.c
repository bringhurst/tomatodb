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

#include <msgpack.h>
#include <uuid/uuid.h>

#include "log.h"
#include "common.h"

#include "client_protocol.h"
#include "data_msg.h"

/** The debug stream to write log messages to. */
extern FILE* HVN_debug_stream;

/** The log level to write messages for. */
extern HVN_loglevel HVN_debug_level;

int HVN_clnt_proto_pack_data_msgpack(HVN_msg_client_data_t* data, \
                                        size_t* len, \
                                        char** msg)
{
    msgpack_sbuffer sbuf;
    msgpack_sbuffer_init(&sbuf);

    msgpack_packer pk;
    msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

    msgpack_pack_array(&pk, 5);
    msgpack_pack_uint16(&pk, HVN_CLNT_PROTO_MSG_TYPE_DATA);
    msgpack_pack_uint16(&pk, data->action);

    // TODO: pack the rest of the struct.

    *msg = (char*) malloc(sizeof(char) * sbuf.size);
    memcpy(*msg, sbuf.data, sbuf.size);
    *len = sbuf.size;

    // FIXME: double free here?
    //msgpack_sbuffer_free(&sbuf);
    //msgpack_packer_free(&pk);

    return HVN_SUCCESS;
}

int HVN_clnt_proto_unpack_data_msgpack(HVN_msg_client_data_t* data, \
        size_t len, \
        char* msg)
{
    msgpack_unpacked unpacked;
    msgpack_unpacked_init(&unpacked);
    int16_t msg_type = 0;

    if(msgpack_unpack_next(&unpacked, msg, len, NULL)) {
        msgpack_object root = unpacked.data;

        if(root.type == MSGPACK_OBJECT_ARRAY && root.via.array.size == 6) {
            msg_type = root.via.array.ptr[0].via.u64;
            data->action = root.via.array.ptr[1].via.u64;
        }
    }

    if(msg_type != HVN_CLNT_PROTO_MSG_TYPE_DATA) {
        LOG(HVN_LOG_ERR, "Unexpected msg type when unpacking a data message (%d).", msg_type);
        return HVN_ERROR;
    }

    msgpack_unpacked_destroy(&unpacked);
    return HVN_SUCCESS;
}

int HVN_clnt_proto_pack_data_resp_msgpack(HVN_msg_client_data_resp_t* data, \
        size_t* len, \
        char** msg)
{
    msgpack_sbuffer sbuf;
    msgpack_sbuffer_init(&sbuf);

    msgpack_packer pck;
    msgpack_packer_init(&pck, &sbuf, msgpack_sbuffer_write);
    msgpack_pack_array(&pck, 2);

    msgpack_pack_uint8(&pck, data->success);
    msgpack_pack_uint8(&pck, data->err_code);

    *len = sbuf.size;
    *msg = (char*) malloc(sizeof(char) * sbuf.size);
    memcpy(*msg, sbuf.data, sbuf.size);

    msgpack_sbuffer_destroy(&sbuf);
    return HVN_SUCCESS;
}

int HVN_clnt_proto_unpack_data_resp_msgpack(HVN_msg_client_data_resp_t* data, \
        size_t len, \
        char* msg)
{
    return HVN_ERROR;
}

int HVN_clnt_proto_pack_data(HVN_msg_client_data_t* data, \
                                int scheme, \
                                size_t* len, \
                                char** msg)
{
    int result;

    switch(scheme) {
        case HVN_CLNT_PROTO_PACK_TYPE_MSGPACK:
            result = HVN_clnt_proto_pack_data_msgpack(data, len, msg);
            break;
        default:
            LOG(HVN_LOG_WARN, "Pack scheme `%d' not recognized.", scheme);
            result = HVN_ERROR;
    }

    return result;
}

int HVN_clnt_proto_unpack_data(HVN_msg_client_data_t* data, \
                                  int scheme, \
                                  size_t len, \
                                  char* msg)
{
    int result;

    switch(scheme) {
        case HVN_CLNT_PROTO_PACK_TYPE_MSGPACK:
            result = HVN_clnt_proto_unpack_data_msgpack(data, len, msg);
            break;
        default:
            LOG(HVN_LOG_WARN, "Unpack scheme `%d' not recognized.", scheme);
            result = HVN_ERROR;
    }

    return result;
}

int HVN_clnt_proto_pack_data_resp(HVN_msg_client_data_resp_t* data, \
                                     int scheme, \
                                     size_t* len, \
                                     char** msg)
{
    int result;

    switch(scheme) {
        case HVN_CLNT_PROTO_PACK_TYPE_MSGPACK:
            result = HVN_clnt_proto_pack_data_resp_msgpack(data, len, msg);
            break;
        default:
            LOG(HVN_LOG_WARN, "Pack scheme `%d' not recognized.", scheme);
            result = HVN_ERROR;
    }

    return result;
}

int HVN_clnt_proto_unpack_data_resp(HVN_msg_client_data_resp_t* data, \
                                       int scheme, \
                                       size_t len, \
                                       char* msg)
{
    int result;

    switch(scheme) {
        case HVN_CLNT_PROTO_PACK_TYPE_MSGPACK:
            result = HVN_clnt_proto_unpack_data_resp_msgpack(data, len, msg);
            break;
        default:
            LOG(HVN_LOG_WARN, "Unpack scheme `%d' not recognized.", scheme);
            result = HVN_ERROR;
    }

    return result;
}

int HVN_proto_receive_data_msg(int fd, \
                                  HVN_msg_client_data_t* data_msg_data)
{
    size_t len = 0;
    char* msg;

    if(HVN_msgpack_fdread(fd, &len, &msg) != HVN_SUCCESS) {
        LOG(HVN_LOG_ERR, "Failed to read a data message from the socket.");
        return HVN_ERROR;
    }

    if(HVN_clnt_proto_unpack(HVN_CLNT_PROTO_MSG_TYPE_DATA, \
                             HVN_CLNT_PROTO_PACK_TYPE_MSGPACK, \
                             data_msg_data, len, msg) != HVN_SUCCESS) {
        LOG(HVN_LOG_ERR, "Failed to unpack a data message.");
        return HVN_ERROR;
    }

    free(msg);
    return HVN_SUCCESS;
}

int HVN_proto_send_data_resp_msg(int fd)
{
    HVN_msg_client_data_resp_t data_resp_msg_data;
    size_t len = 0;
    char* msg;

    if(HVN_clnt_proto_pack(HVN_CLNT_PROTO_MSG_TYPE_DATA_R, \
                           HVN_CLNT_PROTO_PACK_TYPE_MSGPACK, \
                           &data_resp_msg_data, &len, &msg) != HVN_SUCCESS) {
        LOG(HVN_LOG_ERR, "Failed to pack a data message response.");
        return HVN_ERROR;
    }

    if(HVN_msgpack_fdwrite(fd, len, msg) != HVN_SUCCESS) {
        LOG(HVN_LOG_ERR, "Failed to write a data message to the socket.");
        return HVN_ERROR;
    }

    free(msg);
    return HVN_SUCCESS;
}

void HVN_proto_print_data_msg(HVN_msg_client_data_t* data)
{
    switch(data->action) {
        case HVN_CLNT_PROTO_DATA_VERB_READ:
            LOG(HVN_LOG_DBG, "Data message type is `read'.");
            break;
        case HVN_CLNT_PROTO_DATA_VERB_WRITE:
            LOG(HVN_LOG_DBG, "Data message type is `write'.");
            break;
        case HVN_CLNT_PROTO_DATA_VERB_DELETE:
            LOG(HVN_LOG_DBG, "Data message type is `delete'.");
            break;

        case HVN_CLNT_PROTO_DATA_VERB_WATCH:
            LOG(HVN_LOG_DBG, "Data message type is `watch'.");
            break;

        case HVN_CLNT_PROTO_DATA_VERB_UNWATCH:
            LOG(HVN_LOG_DBG, "Data message type is `unwatch'.");
            break;

        case HVN_CLNT_PROTO_DATA_TRANSACTION:
            LOG(HVN_LOG_DBG, "Data message type is `transaction'.");
            break;
        default:
            LOG(HVN_LOG_DBG, "Data message type is UNKNOWN.");
            break;
    }

    switch(data->mode) {
        case HVN_CLNT_PROTO_DATA_MODE_RW:
            LOG(HVN_LOG_DBG, "Data message mode is `read-write'.");
            break;
        case HVN_CLNT_PROTO_DATA_MODE_RO:
            LOG(HVN_LOG_DBG, "Data message mode is `read-only'.");
            break;
        case HVN_CLNT_PROTO_DATA_MODE_RB:
            LOG(HVN_LOG_DBG, "Data message mode is `read-bounded'.");
            break;
        case HVN_CLNT_PROTO_DATA_MODE_RT:
            LOG(HVN_LOG_DBG, "Data message mode is `read-timestamp'.");
            break;
        default:
            LOG(HVN_LOG_DBG, "Data message mode is UNKNOWN.");
            break;
    }

    if(data->key == NULL) {
        LOG(HVN_LOG_DBG, "A key value was not included.");
    } else {
        LOG(HVN_LOG_DBG, "Data command has key of `%s'.", data->key);
    }
    
    if(data->value == NULL) {
        LOG(HVN_LOG_DBG, "A value was not included.");
    } else {
        LOG(HVN_LOG_DBG, "A value was included (length of `%d').", data->value_len);
    }

    HVN_time_interval_print(&(data->time));
}

/* EOF */
