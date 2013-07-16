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

#include "havenctl.h"

/** The debug stream to write log messages to. */
FILE* HAVEN_debug_stream;

/** The log level to write messages for. */
HAVEN_loglevel HAVEN_debug_level;

/** Options specified by the user. */
HAVEN_options_t HAVEN_user_opts;

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
    printf("usage: %s [--version] [--debug=<level>] [--server=<remote>] [--port=<remote>]\n", argv[0]);
    printf("                <command> [<args>]\n\n");

    printf("The most commonly used %s commands are:\n", argv[0]);
    printf("   config     (unimplemented)Modify operational configuration\n");
    printf("   data       (unimplemented)Manually send a command to the datastore\n");
    printf("   info       (unimplemented)Display a summary of the server\n");
    printf("   peer       Modify the list of peers a server interacts with\n");
    printf("   query      (unimplemented)Request operational statistics\n");
    printf("   shutdown   (unimplemented)Cleanly shutdown a server\n\n");

    printf("See '%s help <command>' for more information on a specific command.\n", argv[0]);

    fflush(stdout);
}

/**
 * Parse the user provided command.
 */
void HAVEN_handle_cli_args(char** argv, \
                           int optind_local, \
                           int argc)
{
    int num_args = argc - optind_local;

    if(argv == NULL || num_args < 1) {
        HAVEN_print_usage(argv);
        LOG(HAVEN_LOG_ERR, "You must specify a command.");
        exit(EXIT_FAILURE);
    }

    if(strcasecmp(argv[optind_local], "config") == 0) {
        LOG(HAVEN_LOG_INFO, "The `config' command is not implemented yet. Sorry.");
    }
    else if(strcasecmp(argv[optind_local], "data") == 0) {
        LOG(HAVEN_LOG_INFO, "The `data' command is not implemented yet. Sorry.");
    }
    else if(strcasecmp(argv[optind_local], "help") == 0) {
        LOG(HAVEN_LOG_INFO, "The `help' command is not implemented yet. Sorry.");
    }
    else if(strcasecmp(argv[optind_local], "info") == 0) {
        LOG(HAVEN_LOG_INFO, "The `info' command is not implemented yet. Sorry.");
    }
    else if(strcasecmp(argv[optind_local], "peer") == 0) {
        char** peers = (char**) malloc(sizeof(char*) * (num_args - 1));
        for(int i = 0; i < num_args - 1; i++) {
            peers[i] = argv[optind_local + 2 + i];
        }

        HAVEN_handle_cli_peer(argv[optind_local+1], num_args - 2, peers);
    }
    else if(strcasecmp(argv[optind_local], "query") == 0) {
        LOG(HAVEN_LOG_INFO, "The `query' command is not implemented yet. Sorry.");
    }
    else if(strcasecmp(argv[optind_local], "shutdown") == 0) {
        LOG(HAVEN_LOG_INFO, "The `shutdown' command is not implemented yet. Sorry.");
    }
    else {
        HAVEN_print_usage(argv);
        LOG(HAVEN_LOG_ERR, "Unrecognized command `%s'.", argv[optind_local]);
    }

    return;
}

int main(int argc, \
         char** argv)
{
    int c;
    int option_index = 0;

    /** Use localhost as the default host to contact. */
    HAVEN_user_opts.remote_hostname = (char*)"localhost";

    /** Use 3300 as the default port. */
    HAVEN_user_opts.remote_port = 3300;

    /** Setup the default log output. */
    HAVEN_debug_stream = stdout;

    /* Show debug log messages. */
    HAVEN_debug_level = HAVEN_LOG_INFO;

    static struct option long_options[] = {
        {"debug"  , required_argument, 0, 'd'},
        {"port"   , required_argument, 0, 'p'},
        {"server" , required_argument, 0, 's'},
        {"help"   , no_argument      , 0, 'h'},
        {"version", no_argument      , 0, 'v'},
        {0        , 0                , 0, 0  }
    };

    /* Parse options */
    while((c = getopt_long(argc, argv, "p:hvd:s:", \
                           long_options, &option_index)) != -1) {
        switch(c) {

            case 'p':
                HAVEN_user_opts.remote_port = atoi(optarg);
                LOG(HAVEN_LOG_INFO, "Port set to `%d'.", HAVEN_user_opts.remote_port);
                break;

            case 'h':
                HAVEN_print_usage(argv);
                exit(EXIT_SUCCESS);
                break;

            case 'v':
                HAVEN_print_version();
                exit(EXIT_SUCCESS);
                break;

            case 's':
                HAVEN_user_opts.remote_hostname = optarg;
                LOG(HAVEN_LOG_INFO, "Connecting to host `%s'.", HAVEN_user_opts.remote_hostname);
                break;

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

            case '?':
            default:

                if(optopt == 'p' || optopt == 'd' || optopt == 's') {
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

                exit(EXIT_FAILURE);
                break;
        }
    }

    HAVEN_handle_cli_args(argv, optind, argc);
    exit(EXIT_SUCCESS);
}

/* EOF */
