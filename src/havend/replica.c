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

#include "replica.h"
#include "log.h"

/** The stream to send log messages to. */
extern FILE* HVN_debug_stream;

/** The log level to output. */
extern HVN_loglevel HVN_debug_level;

// TODO: functions to provide replica functionality. Replicas are defined as
// followers, candidates, and leaders participating in a consensus quorum.

int HVN_replica_bootstrap_location()
{
    // TODO: determine what the path should be.
    // TODO: call HVN_replica_bootstrap_leader()

    LOG(HVN_LOG_ERR, "Bootstrap location is not implemented.");
    return HVN_ERROR;
}

int HVN_replica_bootstrap_leader()
{
    // TODO: check for a valid path.
    // TODO: if applicable, register with location quorum.
    // TODO: initialize key value store.
    // TODO: initialize consensus scheme.
    // TODO: initialize data command connection queue.
    LOG(HVN_LOG_ERR, "Bootstrap leader is not implemented.");
    return HVN_ERROR;
}

int HVN_replica_bootstrap_follower()
{
    // TODO: query location quorum to find location leader.
    // TODO: request snapshot from location leader.
    // TODO: initialize consensus scheme.
    LOG(HVN_LOG_ERR, "Bootstrap follower is not implemented.");
    return HVN_ERROR;
}

/* EOF */
