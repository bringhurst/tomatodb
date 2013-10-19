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

#include <getopt.h>
#include <stdlib.h>
#include <string.h>

#include "cmd.h"
#include "config.h"
#include "log.h"
#include "repl.h"
#include "task/task.h"
#include "tomato.h"

/** The loglevel that this instance will output. */
TDB_loglevel TDB_debug_level;

/** Where debug output should go. */
FILE* TDB_debug_stream;

/**
 * Print the current version.
 */
void TDB_print_version(void)
{
    fprintf(stdout, "%s-%s (client)\n", PACKAGE_NAME, PACKAGE_VERSION);
}

/**
 * Print a usage message.
 */
void TDB_print_usage(char** argv)
{
    printf("usage: %s [dhspv] [command...]\n", argv[0]);
    fflush(stdout);
}

void taskmain(int argc, char* argv[])
{
    int c;
    int option_index = 0;

    char line_buffer[TDB_MAX_CMD_LEN];
    int line_offset;
    int line_to_write;
    int line_written;


    static struct option long_options[] = {
        {"debug"                , required_argument, 0, 'd'},
        {"help"                 , no_argument      , 0, 'h'},
        {"server"               , required_argument, 0, 's'},
        {"port"                 , required_argument, 0, 'p'},
        {"version"              , no_argument      , 0, 'v'},
        {0                      , 0                , 0, 0  }
    };

    /* Parse options */
    while((c = getopt_long(argc, argv, "+d:hs:p:v", \
                           long_options, &option_index)) != -1) {
        switch(c) {

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

            case 'h':
                TDB_print_usage(argv);
                exit(EXIT_SUCCESS);
                break;

            case 'p':
                printf("Option p is not implemented yet.\n");
                break;

            case 's':
                printf("Option s is not implemented yet.\n");
                break;

            case 'v':
                TDB_print_version();
                exit(EXIT_SUCCESS);
                break;

            case '?':
            default:

                if(optopt == 'd' || optopt == 'p' || optopt == 's') {
                    TDB_print_usage(argv);
                    fprintf(stderr, "Option -%c requires an argument.\n", \
                            optopt);
                }
                else if(isprint(optopt)) {
                    TDB_print_usage(argv);
                    fprintf(stderr, "Unknown option `-%c'.\n", optopt);
                }
                else {
                    TDB_print_usage(argv);
                    fprintf(stderr,
                            "Unknown option character `\\x%x'.\n",
                            optopt);
                }

                exit(EXIT_FAILURE);
                break;
        }
    }

    // TODO: if server info specified, connect to server.

    if(argv[optind] != NULL) {
        line_buffer[0] = 0;
        line_offset = 0;

        while(argv++,--argc) {
            line_to_write = TDB_MAX_CMD_LEN - line_offset;
            line_written = snprintf(line_buffer + line_offset, line_to_write, "%s ", *argv);

            if(line_to_write < line_written) {
                break;
            }

            line_offset += line_written;
        }

        taskcreate((void (*)(void*)) TDB_cmd_task, (void*) line_buffer, TDB_CMD_STACK_SIZE);
        taskyield();
    } else {
        TDB_repl_start();
    }
}

/* EOF */
