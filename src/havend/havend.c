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
#include "routing.h"
#include "settings.h"

#include "task/task.h"

/** The debug stream to write log messages to. */
FILE* HVN_debug_stream;

/** The log level to write messages for. */
HVN_loglevel HVN_debug_level;

int HVN_context_init(HVN_ctx_t** ctx)
{
    *ctx = (HVN_ctx_t*) malloc(sizeof(HVN_ctx_t));

    if(*ctx == NULL) {
        LOG(HVN_LOG_ERR, "Malloc failed when allocating a context.");
        return HVN_ERROR;
    }

    (*ctx)->listen_addr = (char*) malloc(sizeof(char) * _POSIX_HOST_NAME_MAX);
    strcpy((*ctx)->listen_addr, DEFAULT_LISTEN_ADDRESS);

    (*ctx)->listen_port = DEFAULT_LISTEN_PORT;

    return HVN_SUCCESS;
}

void HVN_context_free(HVN_ctx_t* ctx)
{
    /* FIXME: properly free this thing. */
    free(ctx);
}

/**
 * Print the current version.
 */
void HVN_print_version()
{
    fprintf(stdout, "%s-%s\n", PACKAGE_NAME, PACKAGE_VERSION);
}

/**
 * Print a usage message.
 */
void HVN_print_usage()
{
    printf("usage: havend [-hv] [--debug=<fatal,err,warn,info,dbg>]\n"
           "              [-a <addr> | --listen-address=<addr>]\n"
           "              [-p <port> | --listen-port=<port>]\n");
    fflush(stdout);
}

int HVN_handle_havend_cli_args(HVN_ctx_t* ctx, int argc, char* argv[])
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
                LOG(HVN_LOG_INFO, "Listen address set to `%s'.", ctx->listen_addr);
                break;

            case 'd':

                if(strncmp(optarg, "fatal", 5) == 0) {
                    HVN_debug_level = HVN_LOG_FATAL;
                    LOG(HVN_LOG_INFO, "Debug level set to: fatal");
                }
                else if(strncmp(optarg, "err", 3) == 0) {
                    HVN_debug_level = HVN_LOG_ERR;
                    LOG(HVN_LOG_INFO, "Debug level set to: errors");
                }
                else if(strncmp(optarg, "warn", 4) == 0) {
                    HVN_debug_level = HVN_LOG_WARN;
                    LOG(HVN_LOG_INFO, "Debug level set to: warnings");
                }
                else if(strncmp(optarg, "info", 4) == 0) {
                    HVN_debug_level = HVN_LOG_INFO;
                    LOG(HVN_LOG_INFO, "Debug level set to: info");
                }
                else if(strncmp(optarg, "dbg", 3) == 0) {
                    HVN_debug_level = HVN_LOG_DBG;
                    LOG(HVN_LOG_INFO, "Debug level set to: debug");
                }
                else {
                    LOG(HVN_LOG_INFO, "Debug level `%s' not recognized. " \
                        "Defaulting to `info'.", optarg);
                }

                break;

            case 'p':
                ctx->listen_port = atoi(optarg);
                LOG(HVN_LOG_INFO, "Listen port set to `%d'.", ctx->listen_port);
                break;

            case 'h':
                HVN_print_usage();
                taskexit(EXIT_SUCCESS);
                break;

            case 'v':
                HVN_print_version();
                taskexit(EXIT_SUCCESS);
                break;

            case '?':
            default:

                if(optopt == 'd' || optopt == 'a' || optopt == 'p') {
                    HVN_print_usage();
                    fprintf(stderr, "Option -%c requires an argument.\n", \
                            optopt);
                }
                else if(isprint(optopt)) {
                    HVN_print_usage();
                    fprintf(stderr, "Unknown option `-%c'.\n", optopt);
                }
                else {
                    HVN_print_usage();
                    fprintf(stderr,
                            "Unknown option character `\\x%x'.\n",
                            optopt);
                }

                taskexit(EXIT_FAILURE);
                break;
        }
    }

    return HVN_SUCCESS;
}

void taskmain(int argc, char* argv[])
{
    HVN_ctx_t* ctx = NULL;

    HVN_debug_stream = stdout;
    HVN_debug_level = HVN_LOG_INFO;

    if(HVN_handle_havend_cli_args(ctx, argc, argv) != HVN_SUCCESS) {
        LOG(HVN_LOG_ERR, "Failed to properly handle command line arguments.");
        taskexit(EXIT_FAILURE);
    }

    LOG(HVN_LOG_INFO, "Hello! %s-%s is starting up.", \
        PACKAGE_NAME, PACKAGE_VERSION);

    if(HVN_context_init(&ctx) != HVN_SUCCESS) {
        LOG(HVN_LOG_ERR, "Could not allocate the primary context.");
        taskexit(EXIT_FAILURE);
    }

    if(HVN_set_process_uuid(ctx) != HVN_SUCCESS) {
        LOG(HVN_LOG_ERR, "Could not create or determine local machine UUID.");
        taskexit(EXIT_FAILURE);
    }

    if(HVN_prepare_settings_db(ctx) != HVN_SUCCESS) {
        LOG(HVN_LOG_ERR, "Could not prepare the local settings database.");
        taskexit(EXIT_FAILURE);
    }

    if(HVN_listen_and_accept(ctx) != HVN_SUCCESS) {
        LOG(HVN_LOG_ERR, "The primary server loop failed.");
        taskexit(EXIT_FAILURE);
    }

    HVN_close_db(ctx->settings_db);
    HVN_context_free(ctx);

    LOG(HVN_LOG_INFO, "Goodbye! %s-%s is shutting down.", \
        PACKAGE_NAME, PACKAGE_VERSION);

    taskexit(EXIT_SUCCESS);
}

/* EOF */
