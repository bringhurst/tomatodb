#ifndef __HAVEN_HAVEN_H
#define  __HAVEN_HAVEN_H

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

#include "config.h"
#include "log.h"

#include <getopt.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <inttypes.h>

#include "havenctl_peer.h"

typedef struct {
    char*  remote_hostname;
    int    remote_port;
} HAVEN_options_t;

void HAVEN_print_version(void);
void HAVEN_print_usage(char** argv);

void HAVEN_handle_cli_args(char** argv, \
                           int optind_local, \
                           int argc);

#endif /* __HAVEN_HAVEN_H */
