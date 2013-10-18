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

#include <stdlib.h>
#include <string.h>

#include "context.h"
#include "log.h"

/** The stream to send log messages to. */
extern FILE* TDB_debug_stream;

/** The log level to output. */
extern TDB_loglevel TDB_debug_level;

int TDB_ctx_init(TDB_ctx_t** ctx)
{
    *ctx = (TDB_ctx_t*) malloc(sizeof(TDB_ctx_t));

    if(*ctx == NULL) {
        LOG(TDB_LOG_ERR, "Malloc failed when allocating a context.");
        return TDB_ERROR;
    }

    (*ctx)->listen_addr = (char*) malloc(sizeof(char) * _POSIX_HOST_NAME_MAX);
    strcpy((*ctx)->listen_addr, DEFAULT_LISTEN_ADDRESS);
    (*ctx)->replicas = NULL;

    (*ctx)->listen_port = DEFAULT_LISTEN_PORT;

    return TDB_SUCCESS;
}

void TDB_ctx_free(TDB_ctx_t* ctx)
{
    /* FIXME: properly free this thing. */
    free(ctx);
}

/* EOF */
