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

#include "settings.h"
#include "log.h"
#include "common.h"

#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>

/** The debug stream to write log messages to. */
extern FILE* HAVEN_debug_stream;

/** The log level to write messages for. */
extern HAVEN_loglevel HAVEN_debug_level;

int HAVEN_get_local_machine_id(char** hostname)
{
    struct addrinfo hints, *info;
    int gai_result;
    char localname[_POSIX_HOST_NAME_MAX];

    gethostname(localname, _POSIX_HOST_NAME_MAX);

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; /*either IPV4 or IPV6*/
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_CANONNAME;

    if ((gai_result = getaddrinfo(localname, "http", &hints, &info)) != 0) {
        LOG(HAVEN_LOG_ERR, "Failed to get hostname information. %s", gai_strerror(gai_result));
        return HAVEN_ERROR;
    }

    *hostname = (char*) malloc(sizeof(char) * _POSIX_HOST_NAME_MAX);
    strncpy(*hostname, info->ai_canonname, _POSIX_HOST_NAME_MAX);
    freeaddrinfo(info);

    return HAVEN_SUCCESS;
}

int HAVEN_prepare_config_db(HAVEN_ctx_t* ctx)
{
    char* config_db_path = (char*) malloc(sizeof(char) * _POSIX_PATH_MAX);
    int offset;
    int result;

    offset = sprintf(config_db_path, "%s%s", \
            HAVEN_BASE_STATE_DIR, HAVEN_CONFIG_DB_PREFIX);

    if(HAVEN_ensure_directory_exists(config_db_path) != HAVEN_SUCCESS) {
        LOG(HAVEN_LOG_ERR, "Failed to create directory structure when preparing the config DB.");
        return HAVEN_ERROR;
    }

    sprintf(config_db_path+offset, "/%s", ctx->local_id);
    result = HAVEN_init_db(&ctx->config_db, config_db_path);

    free(config_db_path);
    return result;
}

/* EOF */
