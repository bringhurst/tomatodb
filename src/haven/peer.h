#ifndef __HAVEN_HAVEN_PEER_H
#define __HAVEN_HAVEN_PEER_H

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

#include "haven.h"
#include "admin.pb-c.h"

#include <strings.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <netdb.h>

int HAVEN_handle_modify_peers(int* socket, \
                              size_t number_of_peers, \
                              char** peer_hostnames);

void HAVEN_handle_cli_peer(char* command, \
                           size_t number_of_peers, \
                           char** peer_hostnames);

void HAVEN_set_address(char* hostname, \
                       short port, \
                       struct sockaddr_in* sap);

int HAVEN_tcp_client(char* hostname, \
                     short port);

#endif /* __HAVEN_HAVEN_PEER_H */
