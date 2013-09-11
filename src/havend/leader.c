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

#include "attach.h"
#include "log.h"
#include "pack/data_msg.h"

#include "leader.h"

/** The stream to send log messages to. */
extern FILE* HVN_debug_stream;

/** The log level to output. */
extern HVN_loglevel HVN_debug_level;

int HVN_replica_leader(HVN_replica_t* replica)
{
    UT_array* replica_hooks;
    UT_array* log;
    HVN_addr_t* vote;
    uint64_t term;

    uint32_t heartbeat_result = 0;
    int default_leader_heartbeat = 5000;
    static Alt alts[HVN_REPLICA_LEADER_ALT_NK + 1];

    HVN_attach_msg_t data_attach_msg;
    data_attach_msg.msg = NULL;

    HVN_msg_data_t* data_msg;
    HVN_msg_data_resp_t* data_msg_resp = (HVN_msg_data_resp_t*) malloc(sizeof(HVN_msg_data_resp_t));

    //LOG(HVN_LOG_INFO, "Replica has entered leader state.");

    // TODO: Initialize nextIndex for each follower to the local last log index + 1.

    if(HVN_timer_init(&(replica->election_timer)) != HVN_SUCCESS) {
        LOG(HVN_LOG_ERR, "Could not allocate a heartbeat timer for this leader replica.");
        return HVN_ERROR;
    }

    alts[HVN_REPLICA_LEADER_ALT_DATA_KEY].c = replica->data_chan;
    alts[HVN_REPLICA_LEADER_ALT_DATA_KEY].v = &data_attach_msg;
    alts[HVN_REPLICA_LEADER_ALT_DATA_KEY].op = CHANRCV;

    alts[HVN_REPLICA_LEADER_ALT_TIMER_KEY].c = replica->election_timer->alarm_chan;
    alts[HVN_REPLICA_LEADER_ALT_TIMER_KEY].v = &heartbeat_result;
    alts[HVN_REPLICA_LEADER_ALT_TIMER_KEY].op = CHANRCV;

    // TODO: Send initial empty AppendEntries RPCs (heartbeat) to each follower.

    HVN_timer_start(replica->election_timer, default_leader_heartbeat);

    switch(chanalt(alts)) {

        case HVN_REPLICA_LEADER_ALT_DATA_KEY:
            LOG(HVN_LOG_DBG, "This leader received a data message.");

            if(data_attach_msg.msg == NULL) {
                LOG(HVN_LOG_DBG, "Received an attach wrapper without a valid message.");
                break;
            }

            data_msg = data_attach_msg.msg;
            HVN_proto_print_data_msg(data_msg);

            data_msg_resp->success = HVN_PROTO_DATA_R_OK;
            data_msg_resp->err_code = 0;

            chansendp(data_attach_msg.data_reply_chan, data_msg_resp);

            // TODO: Append data messages to local log.

            // Pack and append to local log.
            //HVN_clnt_proto_pack_data_msgpack(client_data_msg, &op_packed_len, &op_packed);
            //HVN_replica_append_to_log(replica, op_packed, op_packed_len);

            // TODO: Whenever last log index is greater or equal to nextIndex for a follower:
            //           1. Send AppendEntries RPC with log entries starting at nextIndex.
            //               a. Update nextIndex if successful.
            //               b. If AppendEntries fails because of log inconsistency, decrement
            //                  nextIndex and retry.

            // TODO: Mark entries commited if stored on a majority of servers and some entry
            //       from current term is stored on a majority of servers. Apply newly committed
            //       entries to state machine.

            // TODO: Step down if currentTerm changes.
            break;

        case HVN_REPLICA_LEADER_ALT_TIMER_KEY:
            LOG(HVN_LOG_DBG, "This leader received a heartbeat alarm.");
            // TODO: Send empty append messages (heartbeat) to each follower.
            break;

        default:
            LOG(HVN_LOG_ERR, "Unknown index received for leader alt array.");
            break;
    }

    return HVN_SUCCESS;
}

/* EOF */
