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

#include "routing.h"
#include "log.h"

#include "common.h"

/** The debug stream to write log messages to. */
extern FILE* HAVEN_debug_stream;

/** The log level to write messages for. */
extern HAVEN_loglevel HAVEN_debug_level;

//
// This is the router for all new incoming connections. The overall flow
// for what we're doing here is as follows:
//
// First, we'll read the first message that comes in from the new connection.
//
// If a server ID is in the request, we'll attempt to lookup the server task
// in the context routing hash table. If the server task exists in the hash
// table, we'll create a new connection task and add it to the server's
// connection list.
//
// If the server ID is not in the context's routing hash table, we respond to
// the new connection with a suggestion to ask the location quorum where the
// server may be located at. If possible, we should send the address and port
// number of the last known location quorum leader.
//
// If the client doesn't send a server ID at all, we'll then try to determine
// what the client wants to do. The remaining options are:
//
//   1. Become a client-server instance.
//   2. Simply want help to find the address and port of the location quorum
//      leader
//   3. Become a new location quorum leader.
//
int HAVEN_routing_task(HAVEN_router_t* router)
{
    LOG("Started routing task.");
    return HAVEN_SUCCESS;
}

/* EOF */
