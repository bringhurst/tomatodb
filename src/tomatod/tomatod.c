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

#include <ctype.h>
#include <errno.h>
#include <getopt.h>
#include <signal.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "tomatod.h"
#include "log.h"

#include "context.h"
#include "database.h"
#include "bootstrap.h"
#include "routing.h"
#include "settings.h"
#include "task/task.h"

/** The debug stream to write log messages to. */
FILE* TDB_debug_stream;

/** The log level to write messages for. */
TDB_loglevel TDB_debug_level;

// This is where we'll listen and accept for connections so we can launch
// routing tasks.
int TDB_listen_and_accept(TDB_ctx_t* ctx)
{
    int remote_port, accept_fd = 0;
    bool* is_running = (bool*) malloc(sizeof(bool));
    char* remote_addr = (char*) malloc(sizeof(char) * _POSIX_HOST_NAME_MAX);

    *is_running = true;

    ctx->listen_fd = netannounce(TCP, ctx->listen_addr, ctx->listen_port);

    if(fdnoblock(ctx->listen_fd) < 0) {
        LOG(TDB_LOG_ERR, "Failed to set the listening socket to non-blocking. %s", strerror(errno));
        return TDB_ERROR;
    }

    LOG(TDB_LOG_INFO, "Listening on `%s:%d'.", ctx->listen_addr, ctx->listen_port);

    while(*is_running) {
        accept_fd = netaccept(ctx->listen_fd, remote_addr, &remote_port);
        TDB_router_t* router = NULL;

        if(accept_fd < 0) {
            LOG(TDB_LOG_ERR, "Failed to accept a new connection. Attempting to shut down.");
            *is_running = false;
            break;
        }

        if(TDB_init_router(&router, ctx, remote_addr, remote_port, accept_fd) != TDB_SUCCESS) {
            LOG(TDB_LOG_ERR, "Could not proper initialize a new connection router. Attempting to shut down.");
            *is_running = false;
            break;
        }

        taskcreate((void (*)(void*))TDB_routing_task, router, TDB_ROUTER_STACK_SIZE);
    }

    free(is_running);
    return TDB_SUCCESS;
}

// Print the current version.
void TDB_print_version()
{
    fprintf(stdout, "%s-%s\n", PACKAGE_NAME, PACKAGE_VERSION);
}

// Print a usage message.
void TDB_print_usage()
{
    printf("usage: tomatod [-adhpv] [--help] [--version]\n"
           "               [--debug=<fatal,err,warn,info,dbg>]\n"
           "               [-a <addr> | --listen-address=<addr>]\n"
           "               [-p <port> | --listen-port=<port>]\n");
    fflush(stdout);
}

void TDB_signal_handle_SIGINT(int sig)
{
    char  c;

    signal(sig, SIG_IGN);

    printf("SIGINT encountered. Do you really want to quit? [y/n] ");
    c = getchar();

    if(c == 'y' || c == 'Y') {
        // FIXME: shutdown properly.
        exit(EXIT_FAILURE);
    }
    else {
        signal(SIGINT, TDB_signal_handle_SIGINT);
    }

    getchar(); // Get new line character
}

void TDB_install_signal_handlers(void)
{
    struct sigaction sa;

    signal(SIGINT, TDB_signal_handle_SIGINT);

    memset(&sa, 0, sizeof sa);
    sa.sa_handler = SIG_IGN;
    sigaction(SIGPIPE, &sa, NULL);
}

int TDB_handle_tomatod_cli_args(TDB_ctx_t* ctx, int argc, char* argv[])
{
    int c, option_index = 0;

    static struct option long_options[] = {
        {"debug"          , required_argument, 0, 'd'},
        {"listen-address" , required_argument, 0, 'a'},
        {"listen-port"    , required_argument, 0, 'p'},
        {"help"           , no_argument      , 0, 'h'},
        {"version"        , no_argument      , 0, 'v'},
        {0                , 0                , 0, 0  }
    };

    /* Parse options */
    while((c = getopt_long(argc, argv, "a:d:p:hv", \
                           long_options, &option_index)) != -1) {
        switch(c) {

            case 'a':
                strncpy(ctx->listen_addr, optarg, _POSIX_HOST_NAME_MAX);
                LOG(TDB_LOG_INFO, "Listen address set to `%s'.", ctx->listen_addr);
                break;

            case 'd':

                if(strncmp(optarg, "fatal", 5) == 0) {
                    TDB_debug_level = TDB_LOG_FATAL;
                    LOG(TDB_LOG_INFO, "Debug level set to: fatal");
                }
                else if(strncmp(optarg, "err", 3) == 0) {
                    TDB_debug_level = TDB_LOG_ERR;
                    LOG(TDB_LOG_INFO, "Debug level set to: errors");
                }
                else if(strncmp(optarg, "warn", 4) == 0) {
                    TDB_debug_level = TDB_LOG_WARN;
                    LOG(TDB_LOG_INFO, "Debug level set to: warnings");
                }
                else if(strncmp(optarg, "info", 4) == 0) {
                    TDB_debug_level = TDB_LOG_INFO;
                    LOG(TDB_LOG_INFO, "Debug level set to: info");
                }
                else if(strncmp(optarg, "dbg", 3) == 0) {
                    TDB_debug_level = TDB_LOG_DBG;
                    LOG(TDB_LOG_INFO, "Debug level set to: debug");
                }
                else {
                    LOG(TDB_LOG_INFO, "Debug level `%s' not recognized. " \
                        "Defaulting to `info'.", optarg);
                }

                break;

            case 'p':
                ctx->listen_port = atoi(optarg);
                LOG(TDB_LOG_INFO, "Listen port set to `%d'.", ctx->listen_port);
                break;

            case 'h':
                TDB_print_usage();
                taskexit(EXIT_SUCCESS);
                break;

            case 'v':
                TDB_print_version();
                taskexit(EXIT_SUCCESS);
                break;

            case '?':
            default:

                if(optopt == 'd' || optopt == 'a' || optopt == 'p') {
                    TDB_print_usage();
                    fprintf(stderr, "Option -%c requires an argument.\n", \
                            optopt);
                }
                else if(isprint(optopt)) {
                    TDB_print_usage();
                    fprintf(stderr, "Unknown option `-%c'.\n", optopt);
                }
                else {
                    TDB_print_usage();
                    fprintf(stderr,
                            "Unknown option character `\\x%x'.\n",
                            optopt);
                }

                taskexit(EXIT_FAILURE);
                break;
        }
    }

    return TDB_SUCCESS;
}

void taskmain(int argc, char* argv[])
{
    TDB_ctx_t* ctx = NULL;

    TDB_debug_stream = stdout;
    TDB_debug_level = TDB_LOG_INFO;

    if(TDB_ctx_init(&ctx) != TDB_SUCCESS) {
        LOG(TDB_LOG_ERR, "Could not allocate the primary context.");
        taskexit(EXIT_FAILURE);
    }

    if(TDB_handle_tomatod_cli_args(ctx, argc, argv) != TDB_SUCCESS) {
        LOG(TDB_LOG_ERR, "Failed to properly handle command line arguments.");
        taskexit(EXIT_FAILURE);
    }

    LOG(TDB_LOG_INFO, "Hello! %s-%s is starting up.", \
        PACKAGE_NAME, PACKAGE_VERSION);

    TDB_install_signal_handlers();

    if(TDB_set_process_uuid(ctx) != TDB_SUCCESS) {
        LOG(TDB_LOG_ERR, "Could not create or determine local machine UUID.");
        taskexit(EXIT_FAILURE);
    }

    if(TDB_prepare_settings_db(ctx) != TDB_SUCCESS) {
        LOG(TDB_LOG_ERR, "Could not prepare the local settings database.");
        taskexit(EXIT_FAILURE);
    }

    if(TDB_bootstrap_replicas_from_disk(ctx) != TDB_SUCCESS) {
        LOG(TDB_LOG_ERR, "Could not read existing replicas from disk.");
        taskexit(EXIT_FAILURE);
    }

    if(TDB_listen_and_accept(ctx) != TDB_SUCCESS) {
        LOG(TDB_LOG_ERR, "The primary server loop failed.");
        taskexit(EXIT_FAILURE);
    }

    TDB_db_close(ctx->settings_db);
    TDB_ctx_free(ctx);

    LOG(TDB_LOG_INFO, "Goodbye! %s-%s is shutting down.", \
        PACKAGE_NAME, PACKAGE_VERSION);

    taskexit(EXIT_SUCCESS);
}

/* EOF */
