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

#include "candidate.h"

/** The stream to send log messages to. */
extern FILE* TDB_debug_stream;

/** The log level to output. */
extern TDB_loglevel TDB_debug_level;

int TDB_replica_candidate(TDB_replica_t* replica)
{
    TDB_msg_append_t append_msg;
    TDB_msg_vote_t vote_msg;

    uint32_t timeout_result = 0;
    unsigned int default_candidate_timeout = 5000;
    static Alt alts[TDB_REPLICA_CANDIDATE_ALT_NK + 1];

    LOG(TDB_LOG_INFO, "Replica has entered candidate state.");

    // TODO: Increment current Term, vote for self.

    if(TDB_timer_init(&(replica->election_timer)) != TDB_SUCCESS) {
        LOG(TDB_LOG_ERR, "Could not allocate an election timer for this candidate replica.");
        return TDB_ERROR;
    }

    alts[TDB_REPLICA_CANDIDATE_ALT_APPEND_KEY].c = replica->append_chan;
    alts[TDB_REPLICA_CANDIDATE_ALT_APPEND_KEY].v = &append_msg;
    alts[TDB_REPLICA_CANDIDATE_ALT_APPEND_KEY].op = CHANRCV;

    alts[TDB_REPLICA_CANDIDATE_ALT_TIMER_KEY].c = replica->election_timer->alarm_chan;
    alts[TDB_REPLICA_CANDIDATE_ALT_TIMER_KEY].v = &timeout_result;
    alts[TDB_REPLICA_CANDIDATE_ALT_TIMER_KEY].op = CHANRCV;

    alts[TDB_REPLICA_CANDIDATE_ALT_VOTE_KEY].c = replica->vote_chan;
    alts[TDB_REPLICA_CANDIDATE_ALT_VOTE_KEY].v = &vote_msg;
    alts[TDB_REPLICA_CANDIDATE_ALT_VOTE_KEY].op = CHANRCV;

    // TODO: Send RequestVote RPCs to all other servers

    TDB_timer_start(replica->election_timer, default_candidate_timeout);

    switch(chanalt(alts)) {

        case TDB_REPLICA_CANDIDATE_ALT_APPEND_KEY:
            LOG(TDB_LOG_DBG, "This candidate received an append message.");

            // TODO: respond to requestor
            // TODO: AppendEntries RPC received from new leader: step down.
            break;

        case TDB_REPLICA_CANDIDATE_ALT_TIMER_KEY:
            LOG(TDB_LOG_DBG, "Received a candidate timeout alarm.");

            // TODO: Increment term
            // TODO: Start new election
            break;

        case TDB_REPLICA_CANDIDATE_ALT_VOTE_KEY:
            LOG(TDB_LOG_DBG, "This candidate received a vote message.");

            // TODO: respond to requestor
            // TODO: If votes received from majority of servers: become leader.
            break;

        default:
            LOG(TDB_LOG_ERR, "Unknown index received for candidate alt array.");
            break;
    }

    // TODO: Discover higher term: step down (where does this go?).

    return TDB_ERROR;
}

/* EOF */
