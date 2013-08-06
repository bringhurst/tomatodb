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
#include <getopt.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <ctype.h>

#include "config.h"
#include "havend.h"
#include "log.h"

#include "common.h"
#include "consensus.h"
#include "database.h"
#include "server.h"
#include "settings.h"

#include "task/task.h"

/** The debug stream to write log messages to. */
FILE* HAVEN_debug_stream;

/** The log level to write messages for. */
HAVEN_loglevel HAVEN_debug_level;

void HAVEN_free_context(HAVEN_ctx_t* ctx)
{
    /* FIXME: properly free this thing. */
    free(ctx);
}

/**
 * Print the current version.
 */
void HAVEN_print_version()
{
    fprintf(stdout, "%s-%s\n", PACKAGE_NAME, PACKAGE_VERSION);
}

/**
 * Print a usage message.
 */
void HAVEN_print_usage(char** argv)
{
    printf("usage: %s [-hv] [--debug=<level>] [--listen-address=<addr>] [--listen-port=<port>]\n", \
           argv[0]);
    fflush(stdout);
}

int HAVEN_handle_havend_cli_args(HAVEN_ctx_t* ctx, int argc, char* argv[])
{
    int c, option_index = 0;

    static struct option long_options[] = {
        {"debug"                , required_argument, 0, 'd'},
        {"listen-address"       , required_argument, 0, 'a'},
        {"listen-port"          , required_argument, 0, 'p'},
        {"help"                 , no_argument      , 0, 'h'},
        {"version"              , no_argument      , 0, 'v'},
        {0                      , 0                , 0, 0  }
    };

    /* Parse options */
    while((c = getopt_long(argc, argv, "d:a:p:hv", \
                           long_options, &option_index)) != -1) {
        switch(c) {
            case 'd':

                if(strncmp(optarg, "fatal", 5) == 0) {
                    HAVEN_debug_level = HAVEN_LOG_FATAL;
                    LOG(HAVEN_LOG_INFO, "Debug level set to: fatal");
                }
                else if(strncmp(optarg, "err", 3) == 0) {
                    HAVEN_debug_level = HAVEN_LOG_ERR;
                    LOG(HAVEN_LOG_INFO, "Debug level set to: errors");
                }
                else if(strncmp(optarg, "warn", 4) == 0) {
                    HAVEN_debug_level = HAVEN_LOG_WARN;
                    LOG(HAVEN_LOG_INFO, "Debug level set to: warnings");
                }
                else if(strncmp(optarg, "info", 4) == 0) {
                    HAVEN_debug_level = HAVEN_LOG_INFO;
                    LOG(HAVEN_LOG_INFO, "Debug level set to: info");
                }
                else if(strncmp(optarg, "dbg", 3) == 0) {
                    HAVEN_debug_level = HAVEN_LOG_DBG;
                    LOG(HAVEN_LOG_INFO, "Debug level set to: debug");
                }
                else {
                    LOG(HAVEN_LOG_INFO, "Debug level `%s' not recognized. " \
                        "Defaulting to `info'.", optarg);
                }

                break;

            case 'h':
                HAVEN_print_usage(argv);
                taskexit(EXIT_SUCCESS);
                break;

            case 'v':
                HAVEN_print_version();
                taskexit(EXIT_SUCCESS);
                break;

            case '?':
            default:

                if(optopt == 'd' || optopt == 'a' || optopt == 'p') {
                    HAVEN_print_usage(argv);
                    fprintf(stderr, "Option -%c requires an argument.\n", \
                            optopt);
                }
                else if(isprint(optopt)) {
                    HAVEN_print_usage(argv);
                    fprintf(stderr, "Unknown option `-%c'.\n", optopt);
                }
                else {
                    HAVEN_print_usage(argv);
                    fprintf(stderr,
                            "Unknown option character `\\x%x'.\n",
                            optopt);
                }

                taskexit(EXIT_FAILURE);
                break;
        }
    }

    return HAVEN_SUCCESS;
}

void taskmain(int argc, char* argv[])
{
    HAVEN_ctx_t* ctx = (HAVEN_ctx_t*) malloc(sizeof(HAVEN_ctx_t));

    HAVEN_debug_stream = stdout;
    HAVEN_debug_level = HAVEN_LOG_INFO;

    LOG(HAVEN_LOG_INFO, "Hello! %s-%s is starting up.", \
        PACKAGE_NAME, PACKAGE_VERSION);

    if(HAVEN_get_local_machine_uuid(ctx) != HAVEN_SUCCESS) {
        LOG(HAVEN_LOG_ERR, "Could not create or determine local machine UUID.");
        taskexit(EXIT_FAILURE);
    }

    if(HAVEN_prepare_settings_db(ctx) != HAVEN_SUCCESS) {
        LOG(HAVEN_LOG_ERR, "Could not prepare the local settings database.");
        taskexit(EXIT_FAILURE);
    }

    if(HAVEN_handle_havend_cli_args(ctx, argc, argv) != HAVEN_SUCCESS) {
        LOG(HAVEN_LOG_ERR, "Failed to properly handle command line arguments.");
        taskexit(EXIT_FAILURE);
    }

    if(HAVEN_init_server_loop(ctx) != HAVEN_SUCCESS) {
        LOG(HAVEN_LOG_ERR, "The primary server loop failed.");
        taskexit(EXIT_FAILURE);
    }

    HAVEN_close_db(ctx->settings_db);
    HAVEN_free_context(ctx);

    LOG(HAVEN_LOG_INFO, "Goodbye! %s-%s is shutting down.", \
        PACKAGE_NAME, PACKAGE_VERSION);

    taskexit(EXIT_SUCCESS);
}

/* EOF */
