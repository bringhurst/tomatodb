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

#include "pack.h"
#include "control_msg.h"

/** The debug stream to write log messages to. */
extern FILE* HVN_debug_stream;

/** The log level to write messages for. */
extern HVN_loglevel HVN_debug_level;

int HVN_proto_pack_control_msgpack(HVN_msg_client_control_t* data, \
                                   size_t* len, \
                                   char** msg)
{
    msgpack_sbuffer sbuf;
    msgpack_sbuffer_init(&sbuf);

    msgpack_packer pk;
    msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

    msgpack_pack_array(&pk, 5);
    msgpack_pack_uint16(&pk, HVN_PROTO_MSG_TYPE_CONTROL);
    msgpack_pack_uint16(&pk, data->action);

    msgpack_pack_raw(&pk, sizeof(data->uuid));
    msgpack_pack_raw_body(&pk, data->uuid, sizeof(data->uuid));

    msgpack_pack_raw(&pk, data->path_len);
    msgpack_pack_raw_body(&pk, data->path, data->path_len);

    *msg = (char*) malloc(sizeof(char) * sbuf.size);
    memcpy(*msg, sbuf.data, sbuf.size);
    *len = sbuf.size;

    // FIXME: double free here?
    //msgpack_sbuffer_free(&sbuf);
    //msgpack_packer_free(&pk);

    return HVN_SUCCESS;
}

int HVN_proto_unpack_control_msgpack(HVN_msg_client_control_t* data, \
                                     size_t len, \
                                     char* msg)
{
    msgpack_unpacked unpacked;
    msgpack_unpacked_init(&unpacked);
    int16_t msg_type = 0;
    char* uuid_buf;

    if(msgpack_unpack_next(&unpacked, msg, len, NULL)) {
        msgpack_object root = unpacked.data;

        if(root.type == MSGPACK_OBJECT_ARRAY && root.via.array.size == 4) {
            msg_type = root.via.array.ptr[0].via.u64;
            data->action = root.via.array.ptr[1].via.u64;

            if(root.via.array.ptr[2].via.raw.size == UUID_STR_LEN - 1) {
                LOG(HVN_LOG_DBG, "Control message UUID is a valid length.");

                uuid_buf = (char*) malloc(sizeof(char) * UUID_STR_LEN);
                memcpy(uuid_buf, root.via.array.ptr[2].via.raw.ptr, UUID_STR_LEN - 1);
                uuid_buf[UUID_STR_LEN - 1] = '\0';

                LOG(HVN_LOG_DBG, "UUID from the control message is `%s'.", uuid_buf);

                if(uuid_parse(uuid_buf, data->uuid) == 0) {
                    LOG(HVN_LOG_DBG, "Parsed a valid UUID from the control message.");
                } else {
                    LOG(HVN_LOG_DBG, "Did not parse a valid UUID from the control message (might not be a problem).");
                }

                free(uuid_buf);
            } else {
                LOG(HVN_LOG_DBG, "Control message UUID is not a valid length (`%d').", root.via.array.ptr[2].via.raw.size);
            }
        }
    }

    if(msg_type != HVN_PROTO_MSG_TYPE_CONTROL) {
        LOG(HVN_LOG_ERR, "Unexpected msg type when unpacking a control message (%d).", msg_type);
        return HVN_ERROR;
    }

    msgpack_unpacked_destroy(&unpacked);
    return HVN_SUCCESS;
}

int HVN_proto_pack_control_resp_msgpack(HVN_msg_client_control_resp_t* data, \
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

int HVN_proto_unpack_control_resp_msgpack(HVN_msg_client_control_resp_t* data, \
                                          size_t len, \
                                          char* msg)
{
    HVN_INTENTIONALLY_UNUSED_VARIABLE(data);
    HVN_INTENTIONALLY_UNUSED_VARIABLE(len);
    HVN_INTENTIONALLY_UNUSED_VARIABLE(msg);

    return HVN_ERROR;
}

int HVN_proto_pack_control(HVN_msg_client_control_t* data, \
                           int scheme, \
                           size_t* len, \
                           char** msg)
{
    int result;

    switch(scheme) {
        case HVN_PROTO_PACK_TYPE_MSGPACK:
            result = HVN_proto_pack_control_msgpack(data, len, msg);
            break;
        default:
            LOG(HVN_LOG_WARN, "Pack scheme `%d' not recognized.", scheme);
            result = HVN_ERROR;
    }

    return result;
}

int HVN_proto_unpack_control(HVN_msg_client_control_t* data, \
                                  int scheme, \
                                  size_t len, \
                                  char* msg)
{
    int result;

    switch(scheme) {
        case HVN_PROTO_PACK_TYPE_MSGPACK:
            result = HVN_proto_unpack_control_msgpack(data, len, msg);
            break;
        default:
            LOG(HVN_LOG_WARN, "Unpack scheme `%d' not recognized.", scheme);
            result = HVN_ERROR;
    }

    return result;
}

int HVN_proto_pack_control_resp(HVN_msg_client_control_resp_t* data, \
                                     int scheme, \
                                     size_t* len, \
                                     char** msg)
{
    int result;

    switch(scheme) {
        case HVN_PROTO_PACK_TYPE_MSGPACK:
            result = HVN_proto_pack_control_resp_msgpack(data, len, msg);
            break;
        default:
            LOG(HVN_LOG_WARN, "Pack scheme `%d' not recognized.", scheme);
            result = HVN_ERROR;
    }

    return result;
}

int HVN_proto_unpack_control_resp(HVN_msg_client_control_resp_t* data, \
                                       int scheme, \
                                       size_t len, \
                                       char* msg)
{
    int result;

    switch(scheme) {
        case HVN_PROTO_PACK_TYPE_MSGPACK:
            result = HVN_proto_unpack_control_resp_msgpack(data, len, msg);
            break;
        default:
            LOG(HVN_LOG_WARN, "Unpack scheme `%d' not recognized.", scheme);
            result = HVN_ERROR;
    }

    return result;
}

int HVN_proto_receive_control_msg(int fd, \
                                  HVN_msg_client_control_t* control_msg_data)
{
    size_t len = 0;
    char* msg;

    if(HVN_msgpack_fdread(fd, &len, &msg) != HVN_SUCCESS) {
        LOG(HVN_LOG_ERR, "Failed to read a control message from the socket.");
        return HVN_ERROR;
    }

    if(HVN_proto_unpack(HVN_PROTO_MSG_TYPE_CONTROL, \
                        HVN_PROTO_PACK_TYPE_MSGPACK, \
                        control_msg_data, len, msg) != HVN_SUCCESS) {
        LOG(HVN_LOG_ERR, "Failed to unpack a control message.");
        return HVN_ERROR;
    }

    free(msg);
    return HVN_SUCCESS;
}

int HVN_proto_send_control_msg(int fd)
{
    HVN_msg_client_control_t control_msg;
    size_t len = 0;
    char* msg;

    if(HVN_proto_pack(HVN_PROTO_MSG_TYPE_CONTROL, \
                      HVN_PROTO_PACK_TYPE_MSGPACK, \
                      &control_msg, &len, &msg) != HVN_SUCCESS) {
        LOG(HVN_LOG_ERR, "Failed to pack a control message.");
        return HVN_ERROR;
    }

    if(HVN_msgpack_fdwrite(fd, len, msg) != HVN_SUCCESS) {
        LOG(HVN_LOG_ERR, "Failed to write a control message to the socket.");
        return HVN_ERROR;
    }

    free(msg);
    return HVN_SUCCESS;
}

int HVN_proto_send_control_resp_msg(int fd)
{
    HVN_msg_client_control_resp_t control_resp_msg_data;
    size_t len = 0;
    char* msg;

    if(HVN_proto_pack(HVN_PROTO_MSG_TYPE_CONTROL_R, \
                      HVN_PROTO_PACK_TYPE_MSGPACK, \
                      &control_resp_msg_data, &len, &msg) != HVN_SUCCESS) {
        LOG(HVN_LOG_ERR, "Failed to pack a control message response.");
        return HVN_ERROR;
    }

    if(HVN_msgpack_fdwrite(fd, len, msg) != HVN_SUCCESS) {
        LOG(HVN_LOG_ERR, "Failed to write a control message response to the socket.");
        return HVN_ERROR;
    }

    free(msg);
    return HVN_SUCCESS;
}

/* EOF */
