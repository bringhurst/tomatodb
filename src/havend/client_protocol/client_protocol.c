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

/** The debug stream to write log messages to. */
extern FILE* HVN_debug_stream;

/** The log level to write messages for. */
extern HVN_loglevel HVN_debug_level;

int HVN_clnt_proto_unpack(int type, \
                          int scheme, \
                          void* msg_struct, \
                          size_t len, \
                          unsigned char* msg)
{
    int result = HVN_SUCCESS;

    switch(type) {
        case HVN_CLNT_PROTO_MSG_TYPE_CONNECT:
            break;
        case HVN_CLNT_PROTO_MSG_TYPE_CONTROL:
            break;
        case HVN_CLNT_PROTO_MSG_TYPE_DATA:
            break;
        case HVN_CLNT_PROTO_MSG_TYPE_DISCOVER:
            break;
        case HVN_CLNT_PROTO_MSG_TYPE_HEARTBEAT:
            break;
        case HVN_CLNT_PROTO_MSG_TYPE_CONNECT_R:
            break;
        case HVN_CLNT_PROTO_MSG_TYPE_CONTROL_R:
            break;
        case HVN_CLNT_PROTO_MSG_TYPE_DATA_R:
            break;
        case HVN_CLNT_PROTO_MSG_TYPE_DISCOVER_R:
            break;
        case HVN_CLNT_PROTO_MSG_TYPE_HEARTBEAT_R:
            break;
        default:
           LOG(HVN_LOG_WARN, \
               "Encountered an unknown message type of `%d'. Skipping", type);
           result = HVN_ERROR;
    }

    return result;
}

int HVN_clnt_proto_pack(int type, \
                        int scheme, \
                        void* msg_struct, \
                        size_t len, \
                        unsigned char* msg)
{

}

/* EOF */
