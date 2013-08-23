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
extern FILE* HVN_debug_stream;

/** The log level to output. */
extern HVN_loglevel HVN_debug_level;

int HVN_ctx_init(HVN_ctx_t** ctx)
{
    *ctx = (HVN_ctx_t*) malloc(sizeof(HVN_ctx_t));

    if(*ctx == NULL) {
        LOG(HVN_LOG_ERR, "Malloc failed when allocating a context.");
        return HVN_ERROR;
    }

    (*ctx)->listen_addr = (char*) malloc(sizeof(char) * _POSIX_HOST_NAME_MAX);
    strcpy((*ctx)->listen_addr, DEFAULT_LISTEN_ADDRESS);

    (*ctx)->listen_port = DEFAULT_LISTEN_PORT;
    (*ctx)->location_addrs = NULL;

    return HVN_SUCCESS;
}

void HVN_ctx_free(HVN_ctx_t* ctx)
{
    /* FIXME: properly free this thing. */
    free(ctx);
}

/* EOF */
