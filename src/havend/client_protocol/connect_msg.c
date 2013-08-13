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

#include "log.h"
#include "havend.h"

#include "client_protocol.h"
#include "connect_msg.h"

/** The debug stream to write log messages to. */
extern FILE* HVN_debug_stream;

/** The log level to write messages for. */
extern HVN_loglevel HVN_debug_level;

int HVN_clnt_proto_pack_connect_msgpack(HVN_msg_client_connect_t* data, \
                                        size_t* len, \
                                        unsigned char* msg)
{
/*
    msgpack_sbuffer sbuf;
    msgpack_sbuffer_init(&sbuf);

    msgpack_packer pk;
    msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

    msgpack_pack_array(&pk, 3);
    msgpack_pack_int(&pk, 1);
    msgpack_pack_true(&pk);
    msgpack_pack_raw(&pk, 7);
    msgpack_pack_raw_body(&pk, "example", 7);
*/
    return HVN_SUCCESS;
}

int HVN_clnt_proto_unpack_connect_msgpack(HVN_msg_client_connect_t* data, \
                                          size_t len, \
                                          unsigned char* msg)
{
/*
    msgpack_zone mempool;
    msgpack_zone_init(&mempool, 2048);

    msgpack_object deserialized;
    msgpack_unpack(sbuf.data, sbuf.size, NULL, &mempool, &deserialized);

    msgpack_object_print(stdout, deserialized);
    puts("");

    msgpack_zone_destroy(&mempool);
    msgpack_sbuffer_destroy(&sbuf);
*/
    return HVN_SUCCESS;
}

int HVN_clnt_proto_pack_connect_resp_msgpack(HVN_msg_client_connect_resp_t* data, \
                                             size_t* len, \
                                             unsigned char* msg)
{
    return HVN_ERROR;
}

int HVN_clnt_proto_unpack_connect_resp_msgpack(HVN_msg_client_connect_resp_t* data, \
                                               size_t len, \
                                               unsigned char* msg)
{
    return HVN_ERROR;
}

int HVN_clnt_proto_pack_connect(HVN_msg_client_connect_t* data, \
                                int scheme, \
                                size_t* len, \
                                unsigned char* msg)
{
    int result;

    switch(scheme) {
        case HVN_CLNT_PROTO_PACK_TYPE_MSGPACK:
           result = HVN_clnt_proto_pack_connect_msgpack(data, len, msg);
           break;
        default:
           LOG(HVN_LOG_WARN, "Pack scheme `%d' not recognized.", scheme);
           result = HVN_ERROR;
    }

    return result;
}

int HVN_clnt_proto_unpack_connect(HVN_msg_client_connect_t* data, \
                                  int scheme, \
                                  size_t len, \
                                  unsigned char* msg)
{
    int result;

    switch(scheme) {
        case HVN_CLNT_PROTO_PACK_TYPE_MSGPACK:
           result = HVN_clnt_proto_unpack_connect_msgpack(data, len, msg);
           break;
        default:
           LOG(HVN_LOG_WARN, "Unpack scheme `%d' not recognized.", scheme);
           result = HVN_ERROR;
    }

    return result;
}

int HVN_clnt_proto_pack_connect_resp(HVN_msg_client_connect_resp_t* data, \
                                     int scheme, \
                                     size_t* len, \
                                     unsigned char* msg)
{
    int result;

    switch(scheme) {
        case HVN_CLNT_PROTO_PACK_TYPE_MSGPACK:
           result = HVN_clnt_proto_pack_connect_resp_msgpack(data, len, msg);
           break;
        default:
           LOG(HVN_LOG_WARN, "Pack scheme `%d' not recognized.", scheme);
           result = HVN_ERROR;
    }

    return result;
}

int HVN_clnt_proto_unpack_connect_resp(HVN_msg_client_connect_resp_t * data, \
                                       int scheme, \
                                       size_t len, \
                                       unsigned char* msg)
{
    int result;

    switch(scheme) {
        case HVN_CLNT_PROTO_PACK_TYPE_MSGPACK:
           result = HVN_clnt_proto_unpack_connect_resp_msgpack(data, len, msg);
           break;
        default:
           LOG(HVN_LOG_WARN, "Unpack scheme `%d' not recognized.", scheme);
           result = HVN_ERROR;
    }

    return result;
}

/* EOF */
