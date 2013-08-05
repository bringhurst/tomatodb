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

    action newbie_setup {
        HAVEN_newbie_setup_cb();
    }

    action newbie_listen {
        HAVEN_newbie_listen_cb();
    }

    action newbie_handle_cmd {
        HAVEN_newbie_handle_cmd_cb();
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
        HAVEN_newbie_exit_cb();
    }

    action newbie_error {
        HAVEN_newbie_error_cb();
    }

Newbie := (
        start: (
            CONFIGURE @newbie_setup -> Listening
        ),

        Listening: (
            RECEIVED_CMD @newbie_handle_cmd -> Handling
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

int HAVEN_exec_machine_newbie(HAVEN_newbie_machine_t *machine, HAVEN_ctx_t* ctx)
{
    int *p = NULL;
    int *pe = NULL;
    int *eof = NULL;
    int cs = machine->cs;

    %% write exec;
}

/* EOF */
