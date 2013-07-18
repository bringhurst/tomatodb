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

#include "havenctl_peer.h"

/** The strem to send log messages to. */
extern FILE* HAVEN_debug_stream;

/** The log level to output. */
extern HAVEN_loglevel HAVEN_debug_level;

/** Options specified by the user. */
extern HAVEN_options_t HAVEN_user_opts;

int HAVEN_handle_modify_peers(int* socket, \
                              size_t number_of_peers, \
                              char** peer_hostnames)
{
    /****
        Haven__Admin__ModifyPeers msg = HAVEN__ADMIN__MODIFY_PEERS__INIT;
        msg.n_peers = number_of_peers;

        LOG(HAVEN_LOG_INFO, "Total peers attempting to add is `%zu'.", number_of_peers);
        msg.peers = (Haven__Admin__ModifyPeers__Peer **) malloc(number_of_peers * sizeof(Haven__Admin__ModifyPeers__Peer*));

        for(size_t i = 1; i <= number_of_peers; i++) {
            LOG(HAVEN_LOG_INFO, "Attempting to add peer `%s'.", argv[optind_local + 1 + i]);
            Haven__Admin__ModifyPeers__Peer peer = HAVEN__ADMIN__MODIFY_PEERS__PEER__INIT;

            peer.hostname = argv[optind_local + 1 + i];
            peer.port = 3300;

//printf("got here");

            msg.peers[i-1] = &peer;
        }

        size_t len = haven__admin__modify_peers__get_packed_size(&msg);
        void* buf = malloc(len);

        haven__admin__modify_peers__pack(&msg, buf);

        LOG(HAVEN_LOG_DBG, "Writing %zu serialized bytes", len);
                fwrite (buf, len, 1, stdout); 

        int socket = HAVEN_tcp_client(hostname, port);
        int bytes_sent = send(socket, buf, len, 0);

        LOG(HAVEN_LOG_DBG, "Sent %d bytes to server.", bytes_sent);

        close(socket);
        free(msg.peers);
        free(buf);
        ***/
    return -1;
}

void HAVEN_handle_cli_peer(char* command, \
                           size_t number_of_peers, \
                           char** peer_hostnames)
{
    if(command == NULL) {
        LOG(HAVEN_LOG_ERR, "Not enough arguments. You must specify add or remove. Please see peer help.");
        exit(EXIT_FAILURE);
    }

    LOG(HAVEN_LOG_DBG, "Number of peers is %zu.", number_of_peers);
    LOG(HAVEN_LOG_DBG, "Command is peer %s.", command);
    for(size_t i = 0; i < number_of_peers; i++) {
        LOG(HAVEN_LOG_DBG, "Using peer `%s'.", peer_hostnames[i]);
    }

    if(strcasecmp(command, "add") == 0) {
        int socket = HAVEN_tcp_client(HAVEN_user_opts.remote_hostname, HAVEN_user_opts.remote_port);
        HAVEN_handle_modify_peers(&socket, number_of_peers, peer_hostnames);
        close(socket);
    }
    else if(strcasecmp(command, "remove") == 0) {
        LOG(HAVEN_LOG_INFO, "The `peer remove' command is not implemented yet. Sorry.");
    }
    else {
        LOG(HAVEN_LOG_ERR, "Unrecognized argument. You must specify add or remove. Please see peer help.");
    }
}

void HAVEN_set_address(char* hostname, \
                       short port, \
                       struct sockaddr_in* sap)
{
    struct hostent* hp;

    bzero(sap, sizeof(*sap));
    sap->sin_family = AF_INET;

    if(hostname != NULL) {
        if(!inet_aton(hostname, &sap->sin_addr)) {
            hp = gethostbyname(hostname);

            if(hp == NULL) {
                LOG(HAVEN_LOG_ERR, "Unknown host `%s'. Exiting.", hostname);
                exit(EXIT_FAILURE);
            }

            sap->sin_addr = *(struct in_addr*) hp->h_addr;
        }
    }
    else {
        sap->sin_addr.s_addr = htonl(INADDR_ANY);
    }

    sap->sin_port = htons(port);
}

int HAVEN_tcp_client(char* hostname, \
                     short port)
{
    struct sockaddr_in peer;
    int s;

    HAVEN_set_address(hostname, port, &peer);
    s = socket(AF_INET, SOCK_STREAM, 0);

    if(s < 0) {
        LOG(HAVEN_LOG_ERR, "Could not create a socket. %s", strerror(errno));
        return -1;
    }

    if(connect(s, (struct sockaddr*) &peer, sizeof(peer))) {
        LOG(HAVEN_LOG_ERR, "Could not connect to the specified host. %s", strerror(errno));
        return -1;
    }

    return s;
}

/* EOF */
