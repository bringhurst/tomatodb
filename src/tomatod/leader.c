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
extern FILE* TDB_debug_stream;

/** The log level to output. */
extern TDB_loglevel TDB_debug_level;

int TDB_replica_leader(TDB_replica_t* replica)
{
    UT_array* replica_hooks;
    UT_array* log;
    TDB_addr_t* vote;
    uint64_t term;

    uint32_t heartbeat_result = 0;
    int default_leader_heartbeat = 5000;
    static Alt alts[TDB_REPLICA_LEADER_ALT_NK + 1];

    TDB_attach_msg_t data_attach_msg;
    data_attach_msg.msg = NULL;

    TDB_msg_data_t* data_msg;
    TDB_msg_data_resp_t* data_msg_resp = (TDB_msg_data_resp_t*) malloc(sizeof(TDB_msg_data_resp_t));

    //LOG(TDB_LOG_INFO, "Replica has entered leader state.");

    // TODO: Initialize nextIndex for each follower to the local last log index + 1.

    if(TDB_timer_init(&(replica->election_timer)) != TDB_SUCCESS) {
        LOG(TDB_LOG_ERR, "Could not allocate a heartbeat timer for this leader replica.");
        return TDB_ERROR;
    }

    alts[TDB_REPLICA_LEADER_ALT_DATA_KEY].c = replica->data_chan;
    alts[TDB_REPLICA_LEADER_ALT_DATA_KEY].v = &data_attach_msg;
    alts[TDB_REPLICA_LEADER_ALT_DATA_KEY].op = CHANRCV;

    alts[TDB_REPLICA_LEADER_ALT_TIMER_KEY].c = replica->election_timer->alarm_chan;
    alts[TDB_REPLICA_LEADER_ALT_TIMER_KEY].v = &heartbeat_result;
    alts[TDB_REPLICA_LEADER_ALT_TIMER_KEY].op = CHANRCV;

    // TODO: Send initial empty AppendEntries RPCs (heartbeat) to each follower.

    TDB_timer_start(replica->election_timer, default_leader_heartbeat);

    switch(chanalt(alts)) {

        case TDB_REPLICA_LEADER_ALT_DATA_KEY:
            LOG(TDB_LOG_DBG, "This leader received a data message.");

            if(data_attach_msg.msg == NULL) {
                LOG(TDB_LOG_DBG, "Received an attach wrapper without a valid message.");
                break;
            }

            data_msg = data_attach_msg.msg;
            TDB_proto_print_data_msg(data_msg);

            data_msg_resp->success = TDB_PROTO_DATA_R_OK;
            data_msg_resp->err_code = 0;

            chansendp(data_attach_msg.data_reply_chan, data_msg_resp);

            // TODO: Append data messages to local log.

            // Pack and append to local log.
            //TDB_clnt_proto_pack_data_msgpack(client_data_msg, &op_packed_len, &op_packed);
            //TDB_replica_append_to_log(replica, op_packed, op_packed_len);

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

        case TDB_REPLICA_LEADER_ALT_TIMER_KEY:
            LOG(TDB_LOG_DBG, "This leader received a heartbeat alarm.");
            // TODO: Send empty append messages (heartbeat) to each follower.
            break;

        default:
            LOG(TDB_LOG_ERR, "Unknown index received for leader alt array.");
            break;
    }

    return TDB_SUCCESS;
}

/* EOF */
