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

#include <limits.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>

#include "havend.h"
#include "log.h"

#include "common.h"
#include "consensus.h"
#include "database.h"
#include "server.h"
#include "settings.h"

/** The debug stream to write log messages to. */
FILE* HAVEN_debug_stream;

/** The log level to write messages for. */
HAVEN_loglevel HAVEN_debug_level;

void HAVEN_free_context(HAVEN_ctx_t* ctx)
{
    /* FIXME: properly free this thing. */
    free(ctx);
}

int main(void) {
    HAVEN_ctx_t* ctx = \
        (HAVEN_ctx_t*) malloc(sizeof(HAVEN_ctx_t));

    HAVEN_debug_stream = stdout;
    HAVEN_debug_level = HAVEN_LOG_DBG;

    if(HAVEN_get_local_machine_id(&ctx->local_id) != HAVEN_SUCCESS) {
        LOG(HAVEN_LOG_ERR, "Could not determine local machine ID.");
        exit(EXIT_FAILURE);
    }

    LOG(HAVEN_LOG_INFO, "Using `%s' for local machine id.", ctx->local_id);

    HAVEN_prepare_settings_db(ctx);
    HAVEN_init_server_loop(ctx);

    HAVEN_close_db(ctx->config_db);
    HAVEN_free_context(ctx);

    exit(EXIT_SUCCESS);
}

/* EOF */
