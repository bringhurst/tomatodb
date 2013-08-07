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

#include "newbie_machine.h"

%%{
    machine Newbie;
    import "newbie_events.h";

    alphtype int;
    access machine->;

    action newbie_listen {
        HAVEN_newbie_listen(server);
    }

    action newbie_become_client {
        fgoto Client::RegisteringWithLocationService;
    }

    action newbie_become_location {
        fgoto Consensus::StartingLocationService;
    }

    action newbie_join_existing {
        fgoto Consensus::JoiningExistingQuorum;
    }

    action newbie_exit {
        HAVEN_newbie_exit(server);
    }

    action newbie_error {
        HAVEN_newbie_error();
    }

Newbie := (
        start: (
            CONFIGURE @newbie_listen -> Handling
        ),

        Handling: (
            JOIN_EXISTING_CMD @newbie_join_existing |
            BECOME_CLIENT_CMD @newbie_become_client |
            BECOME_LOCATION_CMD @newbie_become_location
        )

    ) <err(newbie_error);

    include Client "client.rl";
    include Consensus "consensus.rl";
}%%

%% write data;

int HAVEN_init_machine_newbie(HAVEN_newbie_machine_t *machine, HAVEN_state_actions_t *actions)
{
    int cs;
    %% write init;
}

int HAVEN_exec_machine_newbie(HAVEN_newbie_machine_t *machine, HAVEN_server_t* server)
{
    int *p = NULL;
    int *pe = NULL;
    int *eof = NULL;
    int cs = machine->cs;

    %% write exec;
}

/* EOF */
