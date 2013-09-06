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
extern FILE* HVN_debug_stream;

/** The log level to output. */
extern HVN_loglevel HVN_debug_level;

int HVN_replica_candidate(HVN_replica_t* replica)
{
    LOG(HVN_LOG_INFO, "Replica has entered candidate state.");

    HVN_INTENTIONALLY_UNUSED_VARIABLE(replica);

    // TODO: Increment current Term, vote for self.

    // TODO: Reset election timeout.

    // TODO: Send RequestVote RPCs to all other servers, wait for either:
    //           1. Votes received from majority of servers: become leader.
    //           2. AppendEntries RPC received from new leader: step down.
    //           3. Election timeout elapses without election resolution:
    //               a. Increment term, state new election.
    //           4. Discover higher term: step down.

    return HVN_ERROR;
}

/* EOF */
