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

int HVN_clnt_proto_pack(int type, \
                        int scheme, \
                        void* msg_struct, \
                        size_t* len, \
                        unsigned char* msg)
{
    int result = HVN_SUCCESS;

    switch(type) {
        case HVN_CLNT_PROTO_MSG_TYPE_CONNECT:
            return HVN_clnt_proto_pack_connect(msg_struct, scheme, len, msg);
            break;
        case HVN_CLNT_PROTO_MSG_TYPE_CONTROL:
            LOG(HVN_LOG_WARN, "Pack not implemented for `%d'.", type);
            break;
        case HVN_CLNT_PROTO_MSG_TYPE_DATA:
            LOG(HVN_LOG_WARN, "Pack not implemented for `%d'.", type);
            break;
        case HVN_CLNT_PROTO_MSG_TYPE_DISCOVER:
            LOG(HVN_LOG_WARN, "Pack not implemented for `%d'.", type);
            break;
        case HVN_CLNT_PROTO_MSG_TYPE_HEARTBEAT:
            LOG(HVN_LOG_WARN, "Pack not implemented for `%d'.", type);
            break;
        case HVN_CLNT_PROTO_MSG_TYPE_CONNECT_R:
            return HVN_clnt_proto_pack_connect_resp(msg_struct, scheme, len, msg);
            break;
        case HVN_CLNT_PROTO_MSG_TYPE_CONTROL_R:
            LOG(HVN_LOG_WARN, "Pack not implemented for `%d'.", type);
            break;
        case HVN_CLNT_PROTO_MSG_TYPE_DATA_R:
            LOG(HVN_LOG_WARN, "Pack not implemented for `%d'.", type);
            break;
        case HVN_CLNT_PROTO_MSG_TYPE_DISCOVER_R:
            LOG(HVN_LOG_WARN, "Pack not implemented for `%d'.", type);
            break;
        case HVN_CLNT_PROTO_MSG_TYPE_HEARTBEAT_R:
            LOG(HVN_LOG_WARN, "Pack not implemented for `%d'.", type);
            break;
        default:
            LOG(HVN_LOG_WARN, \
                "Encountered an unknown message type of `%d' on pack operation. Skipping", type);
            result = HVN_ERROR;
    }

    return result;
}

int HVN_clnt_proto_unpack(int type, \
                          int scheme, \
                          void* msg_struct, \
                          size_t len, \
                          unsigned char* msg)
{
    int result = HVN_SUCCESS;

    switch(type) {
        case HVN_CLNT_PROTO_MSG_TYPE_CONNECT:
            return HVN_clnt_proto_unpack_connect(msg_struct, scheme, len, msg);
            break;
        case HVN_CLNT_PROTO_MSG_TYPE_CONTROL:
            LOG(HVN_LOG_WARN, "Unpack not implemented for `%d'.", type);
            break;
        case HVN_CLNT_PROTO_MSG_TYPE_DATA:
            LOG(HVN_LOG_WARN, "Unpack not implemented for `%d'.", type);
            break;
        case HVN_CLNT_PROTO_MSG_TYPE_DISCOVER:
            LOG(HVN_LOG_WARN, "Unpack not implemented for `%d'.", type);
            break;
        case HVN_CLNT_PROTO_MSG_TYPE_HEARTBEAT:
            LOG(HVN_LOG_WARN, "Unpack not implemented for `%d'.", type);
            break;
        case HVN_CLNT_PROTO_MSG_TYPE_CONNECT_R:
            return HVN_clnt_proto_unpack_connect_resp(msg_struct, scheme, len, msg);
            break;
        case HVN_CLNT_PROTO_MSG_TYPE_CONTROL_R:
            LOG(HVN_LOG_WARN, "Unpack not implemented for `%d'.", type);
            break;
        case HVN_CLNT_PROTO_MSG_TYPE_DATA_R:
            LOG(HVN_LOG_WARN, "Unpack not implemented for `%d'.", type);
            break;
        case HVN_CLNT_PROTO_MSG_TYPE_DISCOVER_R:
            LOG(HVN_LOG_WARN, "Unpack not implemented for `%d'.", type);
            break;
        case HVN_CLNT_PROTO_MSG_TYPE_HEARTBEAT_R:
            LOG(HVN_LOG_WARN, "Unpack not implemented for `%d'.", type);
            break;
        default:
            LOG(HVN_LOG_WARN, \
                "Encountered an unknown message type of `%d' on unpack operation. Skipping", type);
            result = HVN_ERROR;
    }

    return result;
}

/* EOF */
