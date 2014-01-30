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

#include "log.h"
#include "common.h"

#include "pack.h"

#include "append_msg.h"
#include "connect_msg.h"
#include "control_msg.h"
#include "data_msg.h"
#include "vote_msg.h"

/** The debug stream to write log messages to. */
extern FILE* HVN_debug_stream;

/** The log level to write messages for. */
extern HVN_loglevel HVN_debug_level;

int HVN_proto_pack(int type, \
                   int scheme, \
                   void* msg_struct, \
                   size_t* len, \
                   char** msg)
{
    int result = HVN_SUCCESS;

    switch(type) {
        case HVN_PROTO_MSG_TYPE_APPEND:
            return HVN_proto_pack_append(msg_struct, scheme, len, msg);
            break;
        case HVN_PROTO_MSG_TYPE_CONNECT:
            return HVN_proto_pack_connect(msg_struct, scheme, len, msg);
            break;
        case HVN_PROTO_MSG_TYPE_CONTROL:
            return HVN_proto_pack_control(msg_struct, scheme, len, msg);
            break;
        case HVN_PROTO_MSG_TYPE_DATA:
            return HVN_proto_pack_data(msg_struct, scheme, len, msg);
            break;
        case HVN_PROTO_MSG_TYPE_DISCOVER:
            LOG(HVN_LOG_WARN, "Pack not implemented for `%d'.", type);
            break;
        case HVN_PROTO_MSG_TYPE_HEARTBEAT:
            LOG(HVN_LOG_WARN, "Pack not implemented for `%d'.", type);
            break;
        case HVN_PROTO_MSG_TYPE_VOTE:
            return HVN_proto_pack_vote(msg_struct, scheme, len, msg);
            break;
        case HVN_PROTO_MSG_TYPE_APPEND_R:
            return HVN_proto_pack_append_resp(msg_struct, scheme, len, msg);
            break;
        case HVN_PROTO_MSG_TYPE_CONNECT_R:
            return HVN_proto_pack_connect_resp(msg_struct, scheme, len, msg);
            break;
        case HVN_PROTO_MSG_TYPE_CONTROL_R:
            return HVN_proto_pack_control_resp(msg_struct, scheme, len, msg);
            break;
        case HVN_PROTO_MSG_TYPE_DATA_R:
            return HVN_proto_pack_data_resp(msg_struct, scheme, len, msg);
            break;
        case HVN_PROTO_MSG_TYPE_DISCOVER_R:
            LOG(HVN_LOG_WARN, "Pack not implemented for `%d'.", type);
            break;
        case HVN_PROTO_MSG_TYPE_HEARTBEAT_R:
            LOG(HVN_LOG_WARN, "Pack not implemented for `%d'.", type);
            break;
        case HVN_PROTO_MSG_TYPE_VOTE_R:
            return HVN_proto_pack_vote_resp(msg_struct, scheme, len, msg);
            break;
        default:
            LOG(HVN_LOG_WARN, \
                "Encountered an unknown message type of `%d' on pack operation. Skipping", type);
            result = HVN_ERROR;
    }

    return result;
}

int HVN_proto_unpack(int type, \
                     int scheme, \
                     void* msg_struct, \
                     size_t len, \
                     char* msg)
{
    int result = HVN_SUCCESS;

    switch(type) {
        case HVN_PROTO_MSG_TYPE_APPEND:
            return HVN_proto_unpack_append(msg_struct, scheme, len, msg);
            break;
        case HVN_PROTO_MSG_TYPE_CONNECT:
            return HVN_proto_unpack_connect(msg_struct, scheme, len, msg);
            break;
        case HVN_PROTO_MSG_TYPE_CONTROL:
            return HVN_proto_unpack_control(msg_struct, scheme, len, msg);
            break;
        case HVN_PROTO_MSG_TYPE_DATA:
            return HVN_proto_unpack_data(msg_struct, scheme, len, msg);
            break;
        case HVN_PROTO_MSG_TYPE_DISCOVER:
            LOG(HVN_LOG_WARN, "Unpack not implemented for `%d'.", type);
            return HVN_ERROR;
            break;
        case HVN_PROTO_MSG_TYPE_HEARTBEAT:
            LOG(HVN_LOG_WARN, "Unpack not implemented for `%d'.", type);
            return HVN_ERROR;
            break;
        case HVN_PROTO_MSG_TYPE_VOTE:
            return HVN_proto_unpack_vote(msg_struct, scheme, len, msg);
            break;
        case HVN_PROTO_MSG_TYPE_APPEND_R:
            return HVN_proto_unpack_append_resp(msg_struct, scheme, len, msg);
            break;
        case HVN_PROTO_MSG_TYPE_CONNECT_R:
            return HVN_proto_unpack_connect_resp(msg_struct, scheme, len, msg);
            break;
        case HVN_PROTO_MSG_TYPE_CONTROL_R:
            LOG(HVN_LOG_WARN, "Unpack not implemented for `%d'.", type);
            return HVN_ERROR;
            break;
        case HVN_PROTO_MSG_TYPE_DATA_R:
            LOG(HVN_LOG_WARN, "Unpack not implemented for `%d'.", type);
            return HVN_ERROR;
            break;
        case HVN_PROTO_MSG_TYPE_DISCOVER_R:
            LOG(HVN_LOG_WARN, "Unpack not implemented for `%d'.", type);
            return HVN_ERROR;
            break;
        case HVN_PROTO_MSG_TYPE_HEARTBEAT_R:
            LOG(HVN_LOG_WARN, "Unpack not implemented for `%d'.", type);
            return HVN_ERROR;
            break;
        case HVN_PROTO_MSG_TYPE_VOTE_R:
            return HVN_proto_unpack_vote_resp(msg_struct, scheme, len, msg);
            break;
        default:
            LOG(HVN_LOG_WARN, \
                "Encountered an unknown message type of `%d' on unpack operation. Skipping", type);
            result = HVN_ERROR;
    }

    return result;
}

/* EOF */
