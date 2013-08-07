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

%%{
    machine Consensus;
    import "events.h";

    alphtype int;
    access state->;

    action create_new      { TRANSITION(HAVEN_consensus_create_quorum_as_leader); }
    action create_location { TRANSITION(HAVEN_consensus_create_location_quorum); }
    action leave           { TRANSITION(HAVEN_consensus_leave_quorum); }
    action shutdown        { TRANSITION(HAVEN_consensus_shutdown_server); }

    action error { HAVEN_state_error(); }

Consensus := (

        start: (
            CREATE_NEW @create_new -> CreatingNew  |
            CREATE_NEW_LOCATION @create_location -> CreatingNewLocation  |
            JOIN_EXISTING -> JoiningExisting
        ),

        CreatingNewLocation: (
            CREATED_NEW_LOCATION -> Coordinating
        ),

        CreatingNew: (
            CREATED_NEW -> Coordinating
        ),

        JoiningExisting: (
            JOINED_EXISTING -> Following
        ),

        Following: (
            HEARTBEAT_TIMEOUT -> Campaigning |
            HANDLE_LOG_APPEND -> Following |
            LEAVE @leave -> Leaving
        ),

        Campaigning: (
            ELECTION_TIMEOUT -> Campaigning |
            ELECTION_SUCCESS -> Coordinating |
            DISCOVERED_HIGHER_TERM -> Following
        ),

        Coordinating: (
            DISCOVERED_HIGHER_TERM -> Following
        ),

        Leaving: (
            GONE @shutdown
        )

    ) <err(error);

}%%

#include "consensus_state.h"

%% write data;

int HAVEN_init_consensus_state(HAVEN_state_t *state, HAVEN_state_actions_t *actions)
{
    int cs;

    %% write init;
}

int HAVEN_exec_consensus_state(HAVEN_state_t *state, int event, HAVEN_server_t* server)
{
    int event_queue[2] = {0};
    event_queue[0] = event;
    int next = 0;

    const int *p = event_queue;
    const int *pe = p+1;
    const int *eof = event == GONE ? pe : NULL;

    %% write exec;

    return next;
}

/* EOF */
