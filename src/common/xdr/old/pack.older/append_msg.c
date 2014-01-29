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

#include "log.h"
#include "common.h"

#include "pack.h"
#include "append_msg.h"

/** The debug stream to write log messages to. */
extern FILE* HVN_debug_stream;

/** The log level to write messages for. */
extern HVN_loglevel HVN_debug_level;

int HVN_proto_pack_append_msgpack(HVN_msg_append_t* data, \
                                size_t* len, \
                                char** msg)
{
    msgpack_sbuffer sbuf;
    msgpack_sbuffer_init(&sbuf);

    msgpack_packer pk;
    msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

    msgpack_pack_array(&pk, 5);
    msgpack_pack_uint16(&pk, HVN_PROTO_MSG_TYPE_APPEND);


    msgpack_pack_uint64(&pk, data->leader_term);
    msgpack_pack_uint64(&pk, data->commit_index);
    msgpack_pack_uint64(&pk, data->prev_log_index);
    msgpack_pack_uint64(&pk, data->prev_log_term);

    // TODO: pack leader uuid
    // TODO: pack append entries

    *msg = (char*) malloc(sizeof(char) * sbuf.size);
    memcpy(*msg, sbuf.data, sbuf.size);
    *len = sbuf.size;

    // FIXME: double free here?
    //msgpack_sbuffer_free(&sbuf);
    //msgpack_packer_free(&pk);

    return HVN_SUCCESS;
}

int HVN_proto_unpack_append_msgpack(HVN_msg_append_t* data, \
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

            data->leader_term = root.via.array.ptr[1].via.u64;
            data->commit_index = root.via.array.ptr[2].via.u64;
            data->prev_log_index = root.via.array.ptr[3].via.u64;
            data->prev_log_term = root.via.array.ptr[4].via.u64;

            // TODO: unpack leader uuid
            // TODO: unpack append entries

        }
    }

    if(msg_type != HVN_PROTO_MSG_TYPE_APPEND) {
        LOG(HVN_LOG_ERR, "Unexpected msg type when unpacking a append message (%d).", msg_type);
        return HVN_ERROR;
    }

    msgpack_unpacked_destroy(&unpacked);
    return HVN_SUCCESS;
}

int HVN_proto_pack_append_resp_msgpack(HVN_msg_append_resp_t* data, \
                                     size_t* len, \
                                     char** msg)
{
    msgpack_sbuffer sbuf;
    msgpack_sbuffer_init(&sbuf);

    msgpack_packer pck;
    msgpack_packer_init(&pck, &sbuf, msgpack_sbuffer_write);
    msgpack_pack_array(&pck, 2);

    msgpack_pack_uint64(&pck, data->term);
    msgpack_pack_uint8(&pck, data->success);

    *len = sbuf.size;
    *msg = (char*) malloc(sizeof(char) * sbuf.size);
    memcpy(*msg, sbuf.data, sbuf.size);

    msgpack_sbuffer_destroy(&sbuf);
    return HVN_SUCCESS;
}

int HVN_proto_unpack_append_resp_msgpack(HVN_msg_append_resp_t* data, \
        size_t len, \
        char* msg)
{
    HVN_INTENTIONALLY_UNUSED_VARIABLE(data);
    HVN_INTENTIONALLY_UNUSED_VARIABLE(len);
    HVN_INTENTIONALLY_UNUSED_VARIABLE(msg);

    return HVN_ERROR;
}

int HVN_proto_pack_append(HVN_msg_append_t* data, \
                        int scheme, \
                        size_t* len, \
                        char** msg)
{
    int result;

    switch(scheme) {
        case HVN_PROTO_PACK_TYPE_MSGPACK:
            result = HVN_proto_pack_append_msgpack(data, len, msg);
            break;
        default:
            LOG(HVN_LOG_WARN, "Pack scheme `%d' not recognized.", scheme);
            result = HVN_ERROR;
    }

    return result;
}

int HVN_proto_unpack_append(HVN_msg_append_t* data, \
                          int scheme, \
                          size_t len, \
                          char* msg)
{
    int result;

    switch(scheme) {
        case HVN_PROTO_PACK_TYPE_MSGPACK:
            result = HVN_proto_unpack_append_msgpack(data, len, msg);
            break;
        default:
            LOG(HVN_LOG_WARN, "Unpack scheme `%d' not recognized.", scheme);
            result = HVN_ERROR;
    }

    return result;
}

int HVN_proto_pack_append_resp(HVN_msg_append_resp_t* data, \
                             int scheme, \
                             size_t* len, \
                             char** msg)
{
    int result;

    switch(scheme) {
        case HVN_PROTO_PACK_TYPE_MSGPACK:
            result = HVN_proto_pack_append_resp_msgpack(data, len, msg);
            break;
        default:
            LOG(HVN_LOG_WARN, "Pack scheme `%d' not recognized.", scheme);
            result = HVN_ERROR;
    }

    return result;
}

int HVN_proto_unpack_append_resp(HVN_msg_append_resp_t* data, \
                               int scheme, \
                               size_t len, \
                               char* msg)
{
    int result;

    switch(scheme) {
        case HVN_PROTO_PACK_TYPE_MSGPACK:
            result = HVN_proto_unpack_append_resp_msgpack(data, len, msg);
            break;
        default:
            LOG(HVN_LOG_WARN, "Unpack scheme `%d' not recognized.", scheme);
            result = HVN_ERROR;
    }

    return result;
}

int HVN_proto_receive_append_msg(int fd, \
                         HVN_msg_append_t* append_msg_data)
{
    size_t len = 0;
    char* msg;

    if(HVN_msgpack_fdread(fd, &len, &msg) != HVN_SUCCESS) {
        LOG(HVN_LOG_ERR, "Failed to read a append message from the socket.");
        return HVN_ERROR;
    }

    if(HVN_proto_unpack(HVN_PROTO_MSG_TYPE_APPEND, \
                        HVN_PROTO_PACK_TYPE_MSGPACK, \
                        append_msg_data, len, msg) != HVN_SUCCESS) {
        LOG(HVN_LOG_ERR, "Failed to unpack a append message.");
        return HVN_ERROR;
    }

    free(msg);
    return HVN_SUCCESS;
}

int HVN_proto_send_append_msg(int fd, HVN_msg_append_t* append_msg)
{
    size_t len = 0;
    char* msg;

    if(HVN_proto_pack(HVN_PROTO_MSG_TYPE_APPEND, \
                      HVN_PROTO_PACK_TYPE_MSGPACK, \
                      &append_msg, &len, &msg) != HVN_SUCCESS) {
        LOG(HVN_LOG_ERR, "Failed to pack a append message.");
        return HVN_ERROR;
    }

    if(HVN_msgpack_fdwrite(fd, len, msg) != HVN_SUCCESS) {
        LOG(HVN_LOG_ERR, "Failed to write a append message to the socket.");
        return HVN_ERROR;
    }

    free(msg);
    return HVN_SUCCESS;
}

int HVN_proto_send_append_resp_msg(int fd, HVN_msg_append_resp_t* resp)
{
    size_t len = 0;
    char* msg;

    if(HVN_proto_pack(HVN_PROTO_MSG_TYPE_APPEND_R, \
                      HVN_PROTO_PACK_TYPE_MSGPACK, \
                      &resp, &len, &msg) != HVN_SUCCESS) {
        LOG(HVN_LOG_ERR, "Failed to pack a append message response.");
        return HVN_ERROR;
    }

    if(HVN_msgpack_fdwrite(fd, len, msg) != HVN_SUCCESS) {
        LOG(HVN_LOG_ERR, "Failed to write a append message response to the socket.");
        return HVN_ERROR;
    }

    free(msg);
    return HVN_SUCCESS;
}

void HVN_proto_print_append_msg(HVN_msg_append_t* data)
{
    HVN_INTENTIONALLY_UNUSED_VARIABLE(data);
    //TODO
}

/* EOF */
