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

#include "bootstrap.h"

#include "connection.h"
#include "log.h"

#include "task/task.h"

#include <string.h>

/** The stream to send log messages to. */
extern FILE* HVN_debug_stream;

/** The log level to output. */
extern HVN_loglevel HVN_debug_level;

int HVN_bootstrap_listen(HVN_server_t* server)
{
    HVN_connection_t* conn = NULL;
    int remote_port, accept_fd = 0;
    char* remote_addr = (char*) malloc(sizeof(char) * _POSIX_HOST_NAME_MAX);

    // TODO: add conn to server struct so we can free it later on.

    LOG(HVN_LOG_INFO, "Waiting for new connections.");

    while((accept_fd = netaccept(server->listen_fd, remote_addr, &remote_port)) >= 0) {
        conn = (HVN_connection_t*) malloc(sizeof(HVN_connection_t));

        conn->server = server;
        conn->fd = accept_fd;
        conn->remote_port = remote_port;

        /* FIXME: free this (first need to track conn in the server). */
        conn->remote_addr = (char*) malloc(sizeof(char) * _POSIX_HOST_NAME_MAX);
        strncpy(conn->remote_addr, remote_addr, _POSIX_HOST_NAME_MAX);

        LOG(HVN_LOG_INFO, "Accepted connection from `%s' on port `%d'.", \
            remote_addr, remote_port);
        taskcreate((void (*)(void*))HVN_connection_task, conn, HVN_CONNECTION_STACK_SIZE);
    }

    free(remote_addr);
    return -1; /* FIXME */
}

/* EOF */
