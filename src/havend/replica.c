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

// Bootstrap a new location leader replica in the specified context. Set the
// UUID of the new replica to the uuid pointer.
int HVN_replica_bootstrap_location(HVN_replica_t* replica, HVN_ctx_t* ctx, uuid_t* uuid)
{
    if(ctx->location_addrs != NULL) {
        LOG(HVN_LOG_ERR, "A location leader already exists.");
        //FIXME: allow override?
        return HVN_ERROR;
    } else {
        utarray_new(replica->quorum_addrs, &ut_str_icd);
        ctx->location_addrs = replica->quorum_addrs;
    }

    return HVN_replica_bootstrap_leader(replica, ctx, uuid, HVN_REPLICA_LOCATION_KEY);
}

// Bootstrap a new leader replica in the specified context. Set the UUID of
// the new replica to the uuid pointer.
int HVN_replica_bootstrap_leader(HVN_replica_t* replica, HVN_ctx_t* ctx, uuid_t* uuid, const char* path_key)
{
    LOG(HVN_LOG_INFO, "Bootstrapping a replica leader on interface `%s:%d'.", \
            ctx->listen_addr, ctx->listen_port);

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
