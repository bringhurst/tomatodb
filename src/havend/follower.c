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
#include "pack/append_msg.h"
#include "pack/vote_msg.h"
#include "task/task.h"

#include "follower.h"

/** The stream to send log messages to. */
extern FILE* HVN_debug_stream;

/** The log level to output. */
extern HVN_loglevel HVN_debug_level;

int HVN_replica_follower(HVN_replica_t* replica)
{
    HVN_msg_append_t append_msg;
    HVN_msg_vote_t vote_msg;

    uint32_t timeout_result = 0;
    unsigned int default_follower_timeout = 5000;
    static Alt alts[HVN_REPLICA_FOLLOWER_ALT_NK + 1];

    LOG(HVN_LOG_INFO, "Replica has entered follower state.");

    if(HVN_timer_init(&(replica->election_timer)) != HVN_SUCCESS) {
        LOG(HVN_LOG_ERR, "Could not allocate an election timer for this follower replica.");
        return HVN_ERROR;
    }

    alts[HVN_REPLICA_FOLLOWER_ALT_APPEND_KEY].c = replica->append_chan;
    alts[HVN_REPLICA_FOLLOWER_ALT_APPEND_KEY].v = &append_msg;
    alts[HVN_REPLICA_FOLLOWER_ALT_APPEND_KEY].op = CHANRCV;

    alts[HVN_REPLICA_FOLLOWER_ALT_TIMER_KEY].c = replica->election_timer->alarm_chan;
    alts[HVN_REPLICA_FOLLOWER_ALT_TIMER_KEY].v = &timeout_result;
    alts[HVN_REPLICA_FOLLOWER_ALT_TIMER_KEY].op = CHANRCV;

    alts[HVN_REPLICA_FOLLOWER_ALT_VOTE_KEY].c = replica->vote_chan;
    alts[HVN_REPLICA_FOLLOWER_ALT_VOTE_KEY].v = &vote_msg;
    alts[HVN_REPLICA_FOLLOWER_ALT_VOTE_KEY].op = CHANRCV;

    HVN_timer_start(replica->election_timer, default_follower_timeout);

    switch(chanalt(alts)) {

        case HVN_REPLICA_FOLLOWER_ALT_APPEND_KEY:
            LOG(HVN_LOG_DBG, "This follower received an append message.");
            // TODO: respond to requestor
            break;

        case HVN_REPLICA_FOLLOWER_ALT_TIMER_KEY:
            LOG(HVN_LOG_DBG, "Received a follower timeout alarm.");

            // TODO: Convert to candidate if election timeout elapses without either
            //           1. Receiving valid AppendEntries RPC, or
            //           2. Granting vote to candidate.

            break;

        case HVN_REPLICA_FOLLOWER_ALT_VOTE_KEY:
            LOG(HVN_LOG_DBG, "This follower received a vote message.");
            // TODO: respond to requestor
            break;

        default:
            LOG(HVN_LOG_ERR, "Unknown index received for follower alt array.");
            break;
    }

    return HVN_ERROR;
}

/* EOF */
