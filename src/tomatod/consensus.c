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

#include "consensus.h"

#include "log.h"

/** The stream to send log messages to. */
extern FILE* TDB_debug_stream;

/** The log level to output. */
extern TDB_loglevel TDB_debug_level;

int TDB_consensus_exec(TDB_attach_t* client, TDB_db_op_t* op)
{
    TDB_INTENTIONALLY_UNUSED_VARIABLE(client);
    TDB_INTENTIONALLY_UNUSED_VARIABLE(op);

    // TODO: determine how many replicas are in the quorum.
    return TDB_ERROR;
}

/* EOF */
