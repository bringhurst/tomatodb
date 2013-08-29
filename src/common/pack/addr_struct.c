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
#include "addr_struct.h"

/** The debug stream to write log messages to. */
extern FILE* HVN_debug_stream;

/** The log level to write messages for. */
extern HVN_loglevel HVN_debug_level;

int HVN_pack_addr_msgpack(HVN_addr_t* data, \
                          size_t* len, \
                          char** msg)
{
    size_t addr_len = strlen(data->address);
    size_t uuid_len = sizeof(data->uuid);

    msgpack_sbuffer sbuf;
    msgpack_sbuffer_init(&sbuf);

    msgpack_packer pk;
    msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

    msgpack_pack_array(&pk, 3);

    msgpack_pack_raw(&pk, addr_len);
    msgpack_pack_raw_body(&pk, data->address, addr_len);

    msgpack_pack_uint32(&pk, data->port);

    msgpack_pack_raw(&pk, uuid_len);
    msgpack_pack_raw_body(&pk, data->uuid, uuid_len);

    *msg = (char*) malloc(sizeof(char) * sbuf.size);
    memcpy(*msg, sbuf.data, sbuf.size);
    *len = sbuf.size;

    // FIXME: double free here?
    //msgpack_sbuffer_free(&sbuf);
    //msgpack_packer_free(&pk);

    return HVN_SUCCESS;
}

int HVN_unpack_addr_msgpack(HVN_addr_t* data, \
                            size_t len, \
                            char* msg)
{
    msgpack_unpacked unpacked;
    msgpack_unpacked_init(&unpacked);
    int16_t msg_type = 0;
    char* uuid_buf;

    if(msgpack_unpack_next(&unpacked, msg, len, NULL)) {
        msgpack_object root = unpacked.data;

        if(root.type == MSGPACK_OBJECT_ARRAY && root.via.array.size == 3) {
            data->address = (char*) malloc(sizeof(char) * _POSIX_HOST_NAME_MAX);
            memcpy(data->address, root.via.array.ptr[2].via.raw.ptr, _POSIX_HOST_NAME_MAX - 1);
            data->address[_POSIX_HOST_NAME_MAX] = '\0';

            data->port = root.via.array.ptr[1].via.u64;

            if(root.via.array.ptr[2].via.raw.size == UUID_STR_LEN - 1) {
                uuid_buf = (char*) malloc(sizeof(char) * UUID_STR_LEN);
                memcpy(uuid_buf, root.via.array.ptr[2].via.raw.ptr, UUID_STR_LEN - 1);
                uuid_buf[UUID_STR_LEN] = '\0';

                if(uuid_parse(uuid_buf, data->uuid) == 0) {
                    LOG(HVN_LOG_DBG, "Parsed a valid UUID from the address struct.");
                } else {
                    LOG(HVN_LOG_DBG, "Did not parse a valid UUID from the address struct.");
                    return HVN_ERROR;
                }

                free(uuid_buf);
            } else {
                LOG(HVN_LOG_DBG, "Addr struct UUID is not a valid length (`%d').", root.via.array.ptr[2].via.raw.size);
            }
        }
    }

    if(msg_type != HVN_CLNT_PROTO_MSG_TYPE_CONNECT) {
        LOG(HVN_LOG_ERR, "Unexpected msg type when unpacking a connect message (%d).", msg_type);
        return HVN_ERROR;
    }

    msgpack_unpacked_destroy(&unpacked);
    return HVN_SUCCESS;
}

int HVN_pack_addr(HVN_addr_t* data, \
                  int scheme, \
                  size_t* len, \
                  char** msg)
{
    int result;

    switch(scheme) {
        case HVN_CLNT_PROTO_PACK_TYPE_MSGPACK:
            result = HVN_pack_addr_msgpack(data, len, msg);
            break;
        default:
            LOG(HVN_LOG_WARN, "Pack scheme `%d' not recognized.", scheme);
            result = HVN_ERROR;
    }

    return result;
}

int HVN_unpack_addr(HVN_addr_t* data, \
                    int scheme, \
                    size_t len, \
                    char* msg)
{
    int result;

    switch(scheme) {
        case HVN_CLNT_PROTO_PACK_TYPE_MSGPACK:
            result = HVN_unpack_addr_msgpack(data, len, msg);
            break;
        default:
            LOG(HVN_LOG_WARN, "Unpack scheme `%d' not recognized.", scheme);
            result = HVN_ERROR;
    }

    return result;
}

/* EOF */
