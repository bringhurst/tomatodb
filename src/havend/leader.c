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

#include "leader.h"
#include "log.h"

/** The stream to send log messages to. */
extern FILE* HVN_debug_stream;

/** The log level to output. */
extern HVN_loglevel HVN_debug_level;

int HVN_replica_leader(HVN_replica_t* replica)
{
    UT_array* replicas;
    UT_array* log;
    HVN_addr_t* vote;
    uint64_t term;

    LOG(HVN_LOG_INFO, "Replica has entered leader state.");

    // TODO: Skip until client command accept works -- so followers can join the quorum. Initialize nextIndex for each follower to the local last log index + 1.

    // TODO: Skip until client command accept works. Send initial empty AppendEntries RPCs (heartbeat) to each follower.
    //           1. Repeat during idle periods to prevent election timeouts.

    // Accept data commands from clients.
    LOG(HVN_LOG_DBG, "Leader replica is waiting for client data messages.");
    //client_data_msg = chanrecvp(replica->data_chan);
    //HVN_proto_print_data_msg(client_data_msg);
    //LOG(HVN_LOG_DBG, "Leader replica received a data msg.");

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

    return HVN_SUCCESS;
}

/* EOF */
